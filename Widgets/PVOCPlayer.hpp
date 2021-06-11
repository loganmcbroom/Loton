#pragma once

#include <QWidget>
#include <QPushButton>

class PVOCData;
class SpectrographIconModel;

class PVOCPlayerModel : public QObject
{ Q_OBJECT
	friend class PVOCPlayerView;

public:
	PVOCPlayerModel();

public slots:
	void setPVOC( std::shared_ptr<PVOCData> );

private:
	std::unique_ptr<SpectrographIconModel> spectroModel;
	std::shared_ptr<PVOCData> flan;
};

class PVOCPlayerView : public QWidget
{
public:
	PVOCPlayerView( PVOCPlayerModel * model );

private:
	PVOCPlayerModel * model;
	//QPushButton * playButton; //managed by layout
	QPushButton * saveButton; //managed by layout
};
