#pragma once

#include <QWidget>
#include <QPushButton>
#include <QAudio>

class PVOCData;
class QAudioOutput;
class QBuffer;
class SpectrographIconModel;
class AudioData;
class PVOCToAudioConverter;

class PVOCPlayerModel : public QObject
{ Q_OBJECT
	friend class PVOCPlayerView;

	bool playing() const;

public:
	PVOCPlayerModel();

public slots:
	void setPVOC( std::shared_ptr<PVOCData> );
	void play();
	void stop();
	void handlePlayerStateChange( QAudio::State );

signals:
	void startedPlaying();
	void stoppedPlaying();

private:
	mutable std::unique_ptr<QAudioOutput> output;
	mutable QBuffer * buffer;
	mutable std::unique_ptr<QByteArray> array;

	std::unique_ptr<SpectrographIconModel> spectroModel;
	std::shared_ptr<PVOCData> pvocData;
	std::shared_ptr<AudioData> audioData;
	std::unique_ptr<PVOCToAudioConverter> converter;
	std::shared_ptr<std::atomic<bool>> canceller;
};

class PVOCPlayerView : public QWidget
{
public:
	PVOCPlayerView( PVOCPlayerModel * model );

private:
	PVOCPlayerModel * model;
	QPushButton * playButton; //managed by layout
	QPushButton * saveButton; //managed by layout
};
