#pragma once

#include <QLabel>
#include <QFuture>
#include <QFutureWatcher>

#include <flan/Graph.h>

class AudioData;

class WaveformIconModel : public QObject
{ Q_OBJECT
friend class WaveformIconView;

public:
	WaveformIconModel();

	void setAudio( std::shared_ptr<AudioData> in );

signals:
	void dataUpdated( std::shared_ptr<AudioData> );

private:
	std::shared_ptr<AudioData> audio;
};

class WaveformIconView : public QLabel
{ Q_OBJECT
public:
	WaveformIconView( WaveformIconModel * model );

	QImage getIcon() const;

signals:
	void dataUpdated();

public slots:
	void updateData( std::shared_ptr<AudioData> );
	void handleBitmapFinished();

private:
	virtual void resizeEvent( QResizeEvent * ) override;
	//virtual void wheelEvent( QWheelEvent *event ) override;
	void mousePressEvent( QMouseEvent * event ) override;


	WaveformIconModel * model;
	QFuture<flan::Graph> icon;
	QFutureWatcher<flan::Graph> watcher;
	float tStart;
	float tEnd;

	std::shared_ptr<std::atomic<bool>> canceller;
};
