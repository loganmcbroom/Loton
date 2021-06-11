#pragma once

#include <QLabel>
#include <QFuture>
#include <QFutureWatcher>

#include <flan/Graph.h>

class PVOCData;

class SpectrographIconModel : public QObject
{ Q_OBJECT
	friend class SpectrographIconView;
public:
	SpectrographIconModel();

	void setPVOC( std::shared_ptr<PVOCData> in );

signals:
	void dataUpdated( std::shared_ptr<PVOCData> );

private:
	std::shared_ptr<PVOCData> flan;
};

class SpectrographIconView : public QLabel
{ Q_OBJECT
public:
	SpectrographIconView( SpectrographIconModel * model );

	QImage getIcon() const;

public slots:
	void updateData( std::shared_ptr<PVOCData> );
	void onDataUpdated();

private:
	virtual void resizeEvent( QResizeEvent *event ) override;
	virtual void mousePressEvent( QMouseEvent * event ) override;

	SpectrographIconModel * model;
	QFuture<flan::Graph> icon;
	QFutureWatcher<flan::Graph> watcher;

	std::shared_ptr<std::atomic<bool>> canceller;
};
