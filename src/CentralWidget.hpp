#pragma once

#include <QStackedWidget>

namespace QtNodes { class FlowView; };
class LotonFlowScene;
class SettingsMenu;
class CentralWidget;

class SampleExplorer;
class ProcessExplorer;
class NodeControlPanel;
class LotonHelpWidget;

class CentralWidgetManager : public QStackedWidget
{ Q_OBJECT
public:
	CentralWidgetManager( QWidget * parent = nullptr );

	CentralWidget * centralWidget;
	SettingsMenu * settingsMenu;

public slots:
	void showCentralWidget();
	void showSettingsMenu();
	void toggleHelp();
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
	LotonFlowScene * flowScene;
	LotonHelpWidget * helpWidget;
};
