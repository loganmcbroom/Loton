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
#include "Widgets/LotonHelpWidget.hpp"

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

void CentralWidgetManager::toggleHelp()
	{
	centralWidget->helpWidget->setHidden( ! centralWidget->helpWidget->isHidden() );
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

	flowScene = new LotonFlowScene( registerDataModels(), mainSplitter );

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

	auto * gridHelpSplitter = new QSplitter( Qt::Vertical );
	mainSplitter->addWidget( gridHelpSplitter  );
	gridHelpSplitter->setHandleWidth( 4 );
	gridHelpSplitter->setMinimumWidth( 256 );

	flowView = new FlowView( flowScene );
	gridHelpSplitter->addWidget( flowView );

	helpWidget = new LotonHelpWidget();
	gridHelpSplitter->addWidget( helpWidget );
	gridHelpSplitter->setSizes( { 100000, 30000 } );

	mainSplitter->setSizes( {10000, 30000} );
    }
