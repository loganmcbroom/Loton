#pragma once

#include "NodeDataTypes/AudioData.hpp"
#include "AudioProcessModels/AudioProcessModel.hpp"

class AudioFeedbackProcessModel : public AudioProcessModel
{ Q_OBJECT
public:
	AudioFeedbackProcessModel();
	virtual ~AudioFeedbackProcessModel() override;

	void processWithFeedback( std::function<flan::Audio ( flan::Audio::Mod )> process );

	bool isFeedbackEnabled() const;

private:
	void haltThread();
	int feedbackInPort() const;
	void setInData( std::shared_ptr<QtNodes::NodeData> data, PortIndex ) override;
	std::shared_ptr<QtNodes::NodeData> outData( PortIndex = 0 ) override;
	std::shared_ptr<QtNodes::NodeData> fbOut;
	std::shared_ptr<QtNodes::NodeData> timeOut;
	std::unique_ptr<QThread> workerThread;

	virtual void loaded() override;
	virtual void outputConnectionCreated( PortIndex ) override;
	virtual void outputConnectionDeleted( PortIndex ) override;
	virtual void inputConnectionCreated( PortIndex ) override;
	virtual void inputConnectionDeleted( PortIndex ) override;
	bool feedbackOutConnected;
	bool feedbackInConnected;

public slots:
	void handleFeedbackFromWorker( std::shared_ptr<QtNodes::NodeData> audio, std::shared_ptr<QtNodes::NodeData> time );
	void handleResultFromWorker( std::shared_ptr<QtNodes::NodeData> audio );
	void updateData();

signals:
	void sendFeedbackFromGUI( std::shared_ptr<QtNodes::NodeData> a );
	void feedbackCancelled();
	void runWorker();
};

