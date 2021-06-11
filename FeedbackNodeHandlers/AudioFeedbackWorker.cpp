#include "AudioFeedbackWorker.hpp"

#include <QEventLoop>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

Q_DECLARE_METATYPE( std::shared_ptr<QtNodes::NodeData> )

AudioFeedbackWorker::AudioFeedbackWorker( std::function<flan::Audio ( flan::Audio::Mod )> _f )
	: f( _f )
	{
	qRegisterMetaType<std::shared_ptr<QtNodes::NodeData>>();
	}

void AudioFeedbackWorker::doWork()
	{
	auto audioOut = std::make_shared<AudioData>( f( [this]( flan::Audio a, float t )
		{
		emit sendFeedback( std::make_shared<AudioData>( a ), std::make_shared<NumberData>( t ) );

		QEventLoop eventLoop;

		QObject::connect( this, &AudioFeedbackWorker::feedbackRecieved, &eventLoop, &QEventLoop::quit );
		QObject::connect( this, &AudioFeedbackWorker::feedbackCancelled, &eventLoop, &QEventLoop::quit );

		eventLoop.exec();

		if( audioFBIn )
			return audioFBIn->audio;
		else
			return flan::Audio();
		} ) );
	emit sendResult( audioOut );
	}

void AudioFeedbackWorker::handleFeedback( std::shared_ptr<QtNodes::NodeData> a )
	{
	Q_ASSERT( a );
	audioFBIn = std::dynamic_pointer_cast<AudioData>( a );
	emit feedbackRecieved();
	}


