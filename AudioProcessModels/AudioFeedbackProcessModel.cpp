#include "AudioFeedbackProcessModel.hpp"

#include <nodes/Node>

#include "FeedbackNodeHandlers/AudioFeedbackWorker.hpp"
#include "Settings.hpp"

AudioFeedbackProcessModel::AudioFeedbackProcessModel()
	: fbOut( nullptr )
	, timeOut( nullptr )
	, workerThread( nullptr )
	, delayTimer()
	, feedbackOutConnected( false )
	, feedbackInConnected( false )
	, timeOutConnected( false )
	{
	delayTimer.setSingleShot( true );
	QObject::connect( &delayTimer, &QTimer::timeout, this, &AudioFeedbackProcessModel::updateData );
	}

AudioFeedbackProcessModel::~AudioFeedbackProcessModel()
	{
	haltThread();
	}

void AudioFeedbackProcessModel::processWithFeedback( std::function<flan::Audio ( flan::Audio::Mod )> process )
	{
	workerThread.reset( new QThread );
	AudioFeedbackWorker * worker = new AudioFeedbackWorker( process ); // deleteLater ownership
	worker->moveToThread( workerThread.get() );
	QObject::connect( workerThread.get(), &QThread::finished, worker, &QObject::deleteLater );

	QObject::connect( worker, &AudioFeedbackWorker::sendResult, this, &AudioFeedbackProcessModel::handleResultFromWorker );
	QObject::connect( worker, &AudioFeedbackWorker::sendFeedback, this, &AudioFeedbackProcessModel::handleFeedbackFromWorker );
	QObject::connect( worker, &AudioFeedbackWorker::sendWipe, this, &AudioFeedbackProcessModel::handleWipeFromWorker );
	QObject::connect( this, &AudioFeedbackProcessModel::sendFeedbackFromGUI, worker, &AudioFeedbackWorker::handleFeedback );
	QObject::connect( this, &AudioFeedbackProcessModel::runWorker, worker, &AudioFeedbackWorker::doWork );
	QObject::connect( this, &AudioFeedbackProcessModel::quitWorker, worker, &AudioFeedbackWorker::quit );

	workerThread->start();
	emit runWorker();
	}

void AudioFeedbackProcessModel::haltThread()
	{
	if( canceller ) canceller->store( true );
	canceller.reset( new std::atomic<bool>( false ) );

	if( workerThread )
		{
		emit quitWorker();
		workerThread->quit();
		workerThread->wait();
		workerThread.reset();
		}

	timeoutTimer.reset( new QTimer );
	}

int AudioFeedbackProcessModel::feedbackInPort() const
	{
	return nPorts( PortType::In ) - 1;
	}

// This is called when a loop needs to be started
void AudioFeedbackProcessModel::updateData()
	{
	ins.resize( nPorts( PortType::In ) );

	haltThread();

	if( hasWipedInput() ) return;

	timeoutTimer->setSingleShot( true );
	timeoutTimer->setInterval( (int) Settings::processTimeout() );
	QObject::connect( timeoutTimer.get(), &QTimer::timeout, [this, c = canceller]()
		{
		if( !c )
			{
			setValidationState( QtNodes::NodeValidationState::Error, "Process timed out." );
			emit quitWorker();
			}
		*c = true;
		} );

	// If we recieve feedback, reset the timeout timer
	QObject::connect( this, &::AudioFeedbackProcessModel::sendFeedbackFromGUI, timeoutTimer.get(), [t = timeoutTimer.get()]( std::shared_ptr<QtNodes::NodeData> )
		{
		t->start();
		} );		
	timeoutTimer->start();

	bool accepted = process();

	if( accepted )
		{
		setValidationState( QtNodes::NodeValidationState::Valid, "" );
		emit computingStarted();
		}
	else
		{
		setValidationState( QtNodes::NodeValidationState::Warning, "Missing inputs" );
		emit dataUpdated( 0 );
		}
	}

void AudioFeedbackProcessModel::inputsUpdated( std::shared_ptr<NodeData> data, PortIndex i )
	{
	if( i == feedbackInPort() )
		{
		if( data )
			emit sendFeedbackFromGUI( std::static_pointer_cast<LotonNodeData>( data ) ); // Send processed data to worker thread

		// If not processing, start
		if( ! workerThread )
			updateData();
		}
	else
		{ // Start new process
		// Trigger computation off timer with interval depending on if a process is already running
		// This is to stop a crash from the timeOut being connected to an input through a function node
		delayTimer.stop();
		delayTimer.setInterval( workerThread? 100 : 0 );
		delayTimer.start();
		}
	}

void AudioFeedbackProcessModel::wipeOutputs( PortIndex i )
	{
	if( i == feedbackInPort() )
		{
		//wipe recieved
		emit sendFeedbackFromGUI( makeWipe() );
		}
	else
		{
		FlanProcessModel::wipeOutputs( i );
		fbOut = makeWipe();
		timeOut = makeWipe();
		emit dataUpdated( 1 );
		emit dataUpdated( 2 );
		}
	}

std::shared_ptr<NodeData> AudioFeedbackProcessModel::outData( PortIndex i )
	{
	if( i == 0 )
		return FlanProcessModel::outData( i );
	else if( i == 1 )
		return fbOut;
	else
		return timeOut;
	}

bool AudioFeedbackProcessModel::isFeedbackEnabled() const
	{
	if( ins.empty() ) return false;
	return feedbackOutConnected && feedbackInConnected;
	}

void AudioFeedbackProcessModel::loaded()
	{
	feedbackInConnected = !parent->nodeState().connections( PortType::In, feedbackInPort() ).empty();
	feedbackOutConnected = !parent->nodeState().connections( PortType::Out, 1 ).empty();
	timeOutConnected = !parent->nodeState().connections( PortType::Out, 2 ).empty();
	}

void AudioFeedbackProcessModel::outputConnectionCreated( PortIndex i )
	{
	if( i == 1 )
		{
		const bool fb = isFeedbackEnabled();
		feedbackOutConnected = true;
		if( fb != isFeedbackEnabled() )
			updateData();
		}
	if( i == 2 )
		{
		timeOutConnected = true;
		if( isFeedbackEnabled() )
			updateData();
		}
	}

void AudioFeedbackProcessModel::outputConnectionDeleted( PortIndex i )
	{
	if( i == 1 )
		{
		const bool fb = isFeedbackEnabled();
		feedbackOutConnected = false;
		if( fb != isFeedbackEnabled() )
			updateData();
		}
	if( i == 2 )
		{
		timeOutConnected = false;
		if( isFeedbackEnabled() )
			updateData();
		}
	}

void AudioFeedbackProcessModel::inputConnectionCreated( PortIndex i )
	{
	if( i == feedbackInPort() )
		{
		//const bool fb = isFeedbackEnabled();
		feedbackInConnected = true;
		//if( fb != isFeedbackEnabled() )
		//	updateData();
		}
	}

void AudioFeedbackProcessModel::inputConnectionDeleted( PortIndex i )
	{
	FlanProcessModel::inputConnectionDeleted( i );

	if( i == feedbackInPort() )
		{
		const bool fb = isFeedbackEnabled();
		feedbackInConnected = false;
		if( fb != isFeedbackEnabled() )
			updateData();
		}
	}

void AudioFeedbackProcessModel::handleFeedbackFromWorker( std::shared_ptr<LotonNodeData> a, std::shared_ptr<LotonNodeData> t )
	{
	fbOut = a;
	timeOut = t;
	emit dataUpdated( 1 );
	emit dataUpdated( 2 );
	}

void AudioFeedbackProcessModel::handleResultFromWorker( std::shared_ptr<LotonNodeData> audio )
	{
	out = audio;
	emit computingFinished();
	emit dataUpdated( 0 );
	haltThread();
	}

void AudioFeedbackProcessModel::handleWipeFromWorker( int port )
	{
	if( port == 1 ) // Fb audio port
		{
		fbOut = makeWipe();
		emit dataUpdated( 1 );
		}
	else if( port == 2 ) // Fb time port
		{
		if( timeOutConnected )
			{
			timeOut = makeWipe();
			emit dataUpdated( 2 );
			}
		else emit sendFeedbackFromGUI( makeWipe() );
		}
	}

