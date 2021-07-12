#include "AudioFeedbackWorker.hpp"

#include <QEventLoop>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

Q_DECLARE_METATYPE( std::shared_ptr<LotonNodeData> )

AudioFeedbackWorker::AudioFeedbackWorker( std::function<flan::Audio ( flan::Audio::Mod )> _f )
	: f( _f )
	{
	qRegisterMetaType<std::shared_ptr<LotonNodeData>>();
	}

void AudioFeedbackWorker::doWork()
	{
	auto audioOut = std::make_shared<AudioData>( f( [this]( flan::Audio a, float t )
		{
		QTimer delayTimer;
		delayTimer.setSingleShot( true );

		QEventLoop eventLoop;
		QObject::connect( this, &AudioFeedbackWorker::feedbackRecieved, &eventLoop, &QEventLoop::quit );
		QObject::connect( &delayTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit );

		// Send audio wipe, wait
		emit sendWipe( 1 );
		delayTimer.start( 300 );
		eventLoop.exec();
		delayTimer.stop();
		if( quitFlag ) return flan::Audio();

		// If time connection, send time wipe, wait
		emit sendWipe( 2 ); // If there is no time connection, the node will immediately return
		delayTimer.start( 300 );
		eventLoop.exec();
		delayTimer.stop();
		if( quitFlag ) return flan::Audio();

		// Send audio signal, if time connection, send time signal, wait
		emit sendFeedback( std::make_shared<AudioData>( a ), std::make_shared<NumberData>( t ) );
		eventLoop.exec();
		if( quitFlag ) return flan::Audio();

		// Return result to flan
		if( audioFBIn )
			return audioFBIn->audio;
		else
			return flan::Audio();
		} ) );
	emit sendResult( audioOut );
	}

void AudioFeedbackWorker::handleFeedback( std::shared_ptr<LotonNodeData> a )
	{
	Q_ASSERT( a );
	if( ! a->wipe() )
		audioFBIn = std::dynamic_pointer_cast<AudioData>( a );
	emit feedbackRecieved();
	}

void AudioFeedbackWorker::quit()
	{
	quitFlag = true;
	emit feedbackRecieved();
	}


