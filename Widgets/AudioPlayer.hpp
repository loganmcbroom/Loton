#pragma once

#include <QWidget>
#include <QAudio>

class QAudioOutput;
class QBuffer;
class WaveformIconModel;
class WaveformIconView;
class AudioData;
class QPushButton;

class AudioPlayerModel : public QObject
{ Q_OBJECT
	friend class AudioPlayerView;

public:
	AudioPlayerModel();

	bool playing() const;

public slots:
	void setAudio( std::shared_ptr<AudioData> );
	void play();
	//void pause() const;
	void stop();

	void handlePlayerStateChange( QAudio::State );
	void handleNotify();

signals:
	void startedPlaying();
	void stoppedPlaying();

private:
	mutable std::unique_ptr<QAudioOutput> output;
	mutable QBuffer * buffer;
	mutable std::unique_ptr<QByteArray> array;


	std::unique_ptr<WaveformIconModel> waveformModel;
	std::shared_ptr<AudioData> audio;
};

class AudioPlayerView : public QWidget
{
public:
	AudioPlayerView( AudioPlayerModel * model );

	//void paintEvent( QPaintEvent * event ) override;
	//void resizeEvent(QResizeEvent *event) override;

private:
	AudioPlayerModel * model;

	//gfx
	WaveformIconView * waveform; //managed by layout
	QImage overlay;
	QPushButton * playButton; //managed by layout
	QPushButton * saveButton; //managed by layout
};


