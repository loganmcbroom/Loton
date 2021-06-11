#pragma once

#include <functional>
#include <QObject>
#include <QTimer>
#include "NodeDataTypes/AudioData.hpp"

class AudioData;

class AudioFeedbackWorker : public QObject
{ Q_OBJECT
public:
	AudioFeedbackWorker( std::function<flan::Audio ( flan::Audio::Mod )> process );

signals:
	void sendResult( std::shared_ptr<QtNodes::NodeData> audio );
	void sendFeedback( std::shared_ptr<QtNodes::NodeData> audio, std::shared_ptr<QtNodes::NodeData> time );
	void feedbackRecieved();
	void feedbackCancelled();

public slots:
	void doWork();
	void handleFeedback( std::shared_ptr<QtNodes::NodeData> audio );

private:
	std::shared_ptr<AudioData> audioFBIn;
	std::function<flan::Audio ( flan::Audio::Mod )> f;
};
