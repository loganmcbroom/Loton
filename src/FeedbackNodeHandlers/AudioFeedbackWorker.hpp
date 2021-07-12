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
	void sendResult( std::shared_ptr<LotonNodeData> audio );
	void sendFeedback( std::shared_ptr<LotonNodeData> audio, std::shared_ptr<LotonNodeData> time );
	void sendWipe( int );
	void feedbackRecieved();

public slots:
	void doWork();
	void handleFeedback( std::shared_ptr<LotonNodeData> audio );
	void quit();

private:
	std::shared_ptr<AudioData> audioFBIn;
	std::function<flan::Audio ( flan::Audio::Mod )> f;
	bool quitFlag = false;
};
