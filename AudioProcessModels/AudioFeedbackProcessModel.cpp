#include "AudioFeedbackProcessModel.hpp"

#include <nodes/Node>

#include "FeedbackNodeHandlers/AudioFeedbackWorker.hpp"
#include "Settings.hpp"

AudioFeedbackProcessModel::AudioFeedbackProcessModel()
	: fbOut( nullptr )
	, timeOut( nullptr )
	, workerThread( nullptr )
	, feedbackOutConnected( false )
	, feedbackInConnected( false )
	{
	}

AudioFeedbackProcessModel::~AudioFeedbackProcessModel()
	{
	haltThread();
	}

void AudioFeedbackProcessModel::processWithFeedback( std::function<flan::Audio ( flan::Audio::Mod )> process )
	{
	haltThread();
	workerThread.reset( new QThread );
	AudioFeedbackWorker * worker = new AudioFeedbackWorker( process ); // deleteLater ownership
	worker->moveToThread( workerThread.get() );
	QObject::connect( workerThread.get(), &QThread::finished, worker, &QObject::deleteLater );

	QObject::connect( worker, &AudioFeedbackWorker::sendResult, this, &AudioFeedbackProcessModel::handleResultFromWorker );
	QObject::connect( worker, &AudioFeedbackWorker::sendFeedback, this, &AudioFeedbackProcessModel::handleFeedbackFromWorker );
	QObject::connect( this, &AudioFeedbackProcessModel::sendFeedbackFromGUI, worker, &AudioFeedbackWorker::handleFeedback );
	QObject::connect( this, &AudioFeedbackProcessModel::feedbackCancelled, worker, &AudioFeedbackWorker::feedbackCancelled );
	QObject::connect( this, &AudioFeedbackProcessModel::runWorker, worker, &AudioFeedbackWorker::doWork );

	workerThread->start();
	emit runWorker();
	}

void AudioFeedbackProcessModel::haltThread()
	{
	if( workerThread )
		{
		workerThread->quit();
		workerThread->wait();
		workerThread.reset();
		}
	}

int AudioFeedbackProcessModel::feedbackInPort() const
	{
	return nPorts( PortType::In ) - 1;
	}

void AudioFeedbackProcessModel::updateData()
	{
	ins.resize( nPorts( PortType::In ) );

	emit feedbackCancelled();

	if( canceller ) canceller->store( true );
	canceller.reset( new std::atomic<bool>( false ) );

	haltThread();

	timeoutTimer.reset( new QTimer );
	timeoutTimer->setSingleShot( true );
	timeoutTimer->setInterval( (int) Settings::processTimeout() );
	QObject::connect( timeoutTimer.get(), &QTimer::timeout, [this, c = canceller]()
		{
		if( !c )
			{
			setValidationState( QtNodes::NodeValidationState::Error, "Process timed out." );
			emit feedbackCancelled();
			}
		*c = true;
		} );
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

void AudioFeedbackProcessModel::setInData( std::shared_ptr<NodeData> data, PortIndex i )
	{
	ins.resize( nPorts( PortType::In ) );
	ins[i] = data;

	if( i == feedbackInPort() )
		{
		if( data )
			emit sendFeedbackFromGUI( data );

		// if not processing, start
		if( ! workerThread )
			updateData();
		}
	else
		{
		updateData();
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
	if( i == 2 && isFeedbackEnabled() )
		updateData();
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
	if( i == 2 && isFeedbackEnabled() )
		updateData();
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

void AudioFeedbackProcessModel::handleFeedbackFromWorker( std::shared_ptr<NodeData> a, std::shared_ptr<NodeData> t )
	{
	fbOut = a;
	timeOut = t;
	emit dataUpdated( 1 );
	emit dataUpdated( 2 );
	}

void AudioFeedbackProcessModel::handleResultFromWorker( std::shared_ptr<NodeData> audio )
	{
	out = audio;
	emit computingFinished();
	emit dataUpdated( 0 );
	haltThread();
	}

