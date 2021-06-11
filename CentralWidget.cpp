#include "CentralWidget.hpp"

#include <QtWidgets>

#include <nodes/NodeData>
#include <nodes/FlowView>

#include "Registration.hpp"
#include "LotonFlowScene.hpp"
#include "Settings.hpp"
#include "NodeControlPanel.hpp"

#include "Widgets/SampleExplorer.hpp"
#include "Widgets/ProcessExplorer.hpp"
#include "Widgets/SettingsMenu.hpp"

using namespace QtNodes;

//==============================================================================
// CentralWidgetManager

CentralWidgetManager::CentralWidgetManager( QWidget * parent )
	: QStackedWidget( parent )
	, centralWidget( new CentralWidget )
	, settingsMenu( new SettingsMenu )
	{
	addWidget( centralWidget );
	addWidget( settingsMenu );
	showCentralWidget();

	connect( settingsMenu, &SettingsMenu::done,
			this, &CentralWidgetManager::showCentralWidget );
	}

void CentralWidgetManager::showCentralWidget()
	{
	setCurrentWidget( centralWidget );
	}

void CentralWidgetManager::showSettingsMenu()
	{
	setCurrentWidget( settingsMenu );
	}

//==============================================================================
// CentralWidget

CentralWidget::CentralWidget( QWidget * parent )
	: QWidget( parent )
	{
	setContentsMargins( 4, 0, 2, 0 );

	setLayout( new QVBoxLayout );
	layout()->setContentsMargins( 0, 0, 0, 0 );

	auto * mainSplitter = new QSplitter( Qt::Horizontal );
	layout()->addWidget( mainSplitter );
	mainSplitter->setHandleWidth( 4 );
	mainSplitter->setChildrenCollapsible( false );

	flowScene = new XFlowScene( registerDataModels(), mainSplitter );
	flowView = new FlowView( flowScene );

	auto * sidebarSplitter = new QSplitter( Qt::Vertical );
	mainSplitter->addWidget( sidebarSplitter );
	sidebarSplitter->setHandleWidth( 4 );
	sidebarSplitter->setMinimumWidth( 256 );

	sampleExplorer = new SampleExplorer;
	sidebarSplitter->addWidget( sampleExplorer );

	processTree = new ProcessExplorer( flowScene );
	sidebarSplitter->addWidget( processTree );

	controlPanel = new NodeControlPanel;
	sidebarSplitter->addWidget( controlPanel );

	sidebarSplitter->setSizes( { 10000, 10000, 20000 } );

	mainSplitter->addWidget( flowView );
	flowView->setMinimumWidth( 256 );

	mainSplitter->setSizes( {10000, 30000} );
    }
