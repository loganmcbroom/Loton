#pragma once

#include <QStackedWidget>

namespace QtNodes { class FlowView; };
class XFlowScene;
class SettingsMenu;
class CentralWidget;

class SampleExplorer;
class ProcessExplorer;
class NodeControlPanel;

class CentralWidgetManager : public QStackedWidget
{ Q_OBJECT
public:
	CentralWidgetManager( QWidget * parent = nullptr );

	CentralWidget * centralWidget;
	SettingsMenu * settingsMenu;

public slots:
	void showCentralWidget();
	void showSettingsMenu();
};


class CentralWidget : public QWidget
{ Q_OBJECT
public:
	explicit CentralWidget( QWidget * parent = nullptr );

	void settings();

	// Memory is handled by qt for these
	SampleExplorer * sampleExplorer;
	ProcessExplorer * processTree;
	NodeControlPanel * controlPanel;
	QtNodes::FlowView * flowView;
	XFlowScene * flowScene;
};
