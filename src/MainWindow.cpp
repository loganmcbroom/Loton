#include "MainWindow.hpp"

#include <QtWidgets>

//#include <fftw3.h>

#include "CentralWidget.hpp"
#include "Widgets/SettingsMenu.hpp"

#include "Widgets/SampleExplorer.hpp"
#include "LotonFlowScene.hpp"
#include "nodes/FlowView"
#include "NodeControlPanel.hpp"
#include "Settings.hpp"
#include "LotonController.hpp"
#include "LotonNodeModel.hpp"
#include <nodes/Node>
#include "Widgets/ProcessExplorer.hpp"
#include "Widgets/LotonHelpWidget.hpp"


MainWindow * MainWindow::instance = nullptr;
LotonNodeModel * MainWindow::activeNode()
	{
	return getInstance()->centralWidgetManager->centralWidget->controlPanel->activeNode();
	}
void MainWindow::setActiveNode( LotonNodeModel * newPanel )
	{
	auto cw = getInstance()->centralWidgetManager->centralWidget;
	cw->controlPanel->setActiveNode( newPanel );
	cw->helpWidget->setActiveNode( newPanel );
	}

MainWindow::MainWindow()
	: centralWidgetManager( new CentralWidgetManager( this ) )
    {
	setCentralWidget( centralWidgetManager );

    createActions();
    createMenus();

	centralWidgetManager->settingsMenu->addButton( 0, "Samples", sampleFolderSelectAct );
	centralWidgetManager->settingsMenu->addButton( 0, "Projects", projectFolderSelectAct );

    statusBar()->show();

	QObject::connect( Settings::get(), &Settings::paletteUpdated, this, [this]()
		{
		paintAs( Settings::palette() );
		});

	paintAs( Settings::palette() );

    setWindowTitle( "Loton" );
	resize( 1000, 700 );
    showMaximized();

	auto * undoStack = LotonController::manager().undoStack.get();
	centralWidgetManager->centralWidget->flowScene->undoStack = undoStack;

	QObject::connect( centralWidgetManager->centralWidget->flowScene, &QtNodes::FlowScene::loading, this, [this]( const QJsonObject & )
		{
		setWindowTitle( QString( "Loton - " ) + QDir( Settings::currentProjectPath() ).dirName() );
		});
	QObject::connect( undoStack, &QUndoStack::cleanChanged, this, [this]( bool ){ updateTitle(); });

	// If the current project doesn't exist or isn't a flow file
	if( ! QFileInfo( Settings::currentProjectPath() ).exists() || QFileInfo( Settings::currentProjectPath() ).completeSuffix() != "flow" )
		{
		if( ! Settings::currentProjectPath().isEmpty() )
			QMessageBox::warning( this, "Project Error",
				"The last open project could not be found." );
		Settings::currentProjectPath() = QString();
		}
	else
		{
		centralWidgetManager->centralWidget->flowScene->load( Settings::currentProjectPath() );
		}

	LotonController::manager().undoStack->setClean();
    }

void MainWindow::open( QString filename )
    {
	const bool accepted = maybeSave();
	if( !accepted ) return;

	if( filename.isNull() || ! QFileInfo( filename ).exists() )
		filename = QFileDialog::getOpenFileName( nullptr,
								 "Open Flow Scene",
								 Settings::projectsPath(),
								 "Flow Scene Files (*.flow)" );
	if( ! filename.isNull() )
		{
		centralWidgetManager->centralWidget->flowScene->load( filename );
		LotonController::manager().undoStack.get()->setClean();
		Settings::currentProjectPath() = filename;
		}
	updateTitle();
    }

bool MainWindow::save()
    {
	if( Settings::currentProjectPath().isNull() )
		return saveAs();

	bool saved = centralWidgetManager->centralWidget->flowScene->save( Settings::currentProjectPath() );
	if( saved )
		{
		LotonController::manager().undoStack->setClean();
		if( Settings::get() )
			Settings::get()->saveToRegistry();
		}
	return saved;
    }

bool MainWindow::saveAs()
    {
	QString filename = QFileDialog::getSaveFileName( this,
								   "Save Flow Scene",
								   Settings::projectsPath(),
								   "Flow Scene Files (*.flow)");

	if( filename.isNull() )
		return false;
	else
		Settings::currentProjectPath() = filename;

	if( save() )
		{
		open( Settings::currentProjectPath() );
		return true;
		}
	else return false;
    }

void MainWindow::newProject()
	{
	const bool accepted = maybeSave();
	if( !accepted ) return;

	setActiveNode( nullptr );
	LotonController::manager().undoStack->clear();
	centralWidgetManager->centralWidget->flowScene->clearScene();
	Settings::currentProjectPath() = QString();
	updateTitle();
	}

void MainWindow::duplicateNode()
	{
	LotonNodeModel * n = getInstance()->centralWidgetManager->centralWidget->controlPanel->activeNode();
	if( n )
		{
		auto scene = centralWidgetManager->centralWidget->flowScene;
		QtNodes::Node & newNode = scene->createNodeFromName( n->name(), n->parent->nodeGraphicsObject().scenePos() + QPointF( 0, n->parent->nodeGeometry().height() + 25 ) );
		newNode.nodeGraphicsObject().scene()->clearSelection();
		newNode.nodeGraphicsObject().setSelected( true );
		getInstance()->centralWidgetManager->centralWidget->controlPanel->setActiveNode( dynamic_cast<LotonNodeModel *>( newNode.nodeDataModel() ) );
		}
	}

void MainWindow::resetZoom()
	{
	auto v = centralWidgetManager->centralWidget->flowScene->views()[0];
	float s = v->transform().m11();
	v->scale( 1.0 / s, 1.0 / s );
	}

void MainWindow::findNode()
	{
	centralWidgetManager->centralWidget->processTree->engage();
	}

void MainWindow::settings()
    {
	centralWidgetManager->showSettingsMenu();
    }

void MainWindow::undo()
	{
	LotonController::manager().undoStack->undo();
	}

void MainWindow::redo()
	{
	LotonController::manager().undoStack->redo();
	}

void MainWindow::sampleFolderSelect()
	{
	QString & sampleRoot = Settings::samplesPath();
	auto path = QFileDialog::getExistingDirectory( this, "Select Root Directory", sampleRoot,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
	if( path == "" ) return;

	auto mw = MainWindow::getInstance();
	mw->centralWidgetManager->centralWidget->sampleExplorer->setRootPath( path );
	//auto * proxy = dynamic_cast<QSortFilterProxyModel *>( tree->model() );
	sampleRoot = path;
	}

void MainWindow::projectFolderSelect()
	{
	QString & root = Settings::projectsPath();
	auto dir = QFileDialog::getExistingDirectory( this, "Project Root Directory", root,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
	if( dir == "" ) return;

	root = dir;
	}

void MainWindow::updateTitle()
	{
	if( Settings::get() )
		{
		QString dirName = QDir( Settings::currentProjectPath() ).dirName();
		const bool isClean = LotonController::manager().undoStack->isClean();
		setWindowTitle( QString( "Loton - " ) + QString( isClean? "" : "*" ) + dirName );
		}
	}

void MainWindow::createMixNode()
	{
	createNode( "Audio Mix" );
	}

void MainWindow::createJoinNode()
	{
	createNode( "Audio Join" );
	}

void MainWindow::createCutNode()
	{
	createNode( "Audio Cut" );
	}

void MainWindow::createSetVolumeNode()
	{
	createNode( "Audio Set Volume" );
	}

void MainWindow::createPanNode()
	{
	createNode( "Audio Pan" );
	}

void MainWindow::createNode( QString nodeName )
	{
	auto scene = centralWidgetManager->centralWidget->flowScene;
	auto view = centralWidgetManager->centralWidget->flowView;
	const QPointF pos = view->mapToScene( QPoint( view->width() / 2, view->height() / 2 ) );
	scene->createNodeFromName( nodeName, pos );
	}

void MainWindow::closeEvent( QCloseEvent * event )
    {
	if( ! LotonController::manager().undoStack->isClean() )
        {
        if( maybeSave() )
            {
            event->accept();
            }
        else event->ignore();
        }
    else event->accept();
    }

bool MainWindow::maybeSave()
    {
	if( LotonController::manager().undoStack->isClean() )
		return true;

	const QMessageBox::StandardButton message
        = QMessageBox::warning( this, "Save Changes?",
                               "The project has been modified.\n"
                               "Do you want to save your changes?",
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	switch( message )
        {
        case QMessageBox::Save:
            return save();
        case QMessageBox::Cancel:
            return false;
        default:
            break;
        }
    return true;
    }

void MainWindow::createActions()
    {
    openAct = new QAction( "&Open...", this );
    openAct->setShortcuts( QKeySequence::Open );
    openAct->setStatusTip( "Open an existing file" );
	connect( openAct, &QAction::triggered, this, [this](){ open(); } );

    saveAct = new QAction( "&Save", this );
    saveAct->setShortcuts( QKeySequence::Save );
    saveAct->setStatusTip( "Save the document to disk" );
	connect( saveAct, &QAction::triggered, this, &MainWindow::save );

	saveAsAct = new QAction( "&Save As", this );
	saveAsAct->setShortcuts( QKeySequence::SaveAs );
	saveAsAct->setStatusTip( "Save the document to disk" );
	connect( saveAsAct, &QAction::triggered, this, &MainWindow::saveAs );

	newAct = new QAction( "&New", this );
	newAct ->setShortcuts( QKeySequence::New );
	newAct ->setStatusTip( "Create new project" );
	connect( newAct, &QAction::triggered, this, &MainWindow::newProject );

	duplicateAct = new QAction( "&Duplicate", this );
	duplicateAct->setShortcuts( { Qt::CTRL + Qt::Key_D } );
	duplicateAct->setStatusTip( "Duplicate node" );
	connect( duplicateAct, &QAction::triggered, this, &MainWindow::duplicateNode );

	resetZoomAct = new QAction( "&Reset Zoom", this );
	resetZoomAct->setShortcuts( { Qt::CTRL + Qt::Key_0 } );
	resetZoomAct->setStatusTip( "Reset flow view zoom" );
	connect( resetZoomAct, &QAction::triggered, this, &MainWindow::resetZoom );

	findNodeAct = new QAction( "&Find Node", this );
	findNodeAct->setShortcuts( QKeySequence::Find );
	findNodeAct->setStatusTip( "Move curser to node finder" );
	connect( findNodeAct, &QAction::triggered, this, &MainWindow::findNode );

    settingsAct = new QAction( "&Settings", this );
    settingsAct->setShortcuts( QKeySequence::Preferences );
    settingsAct->setStatusTip( "Change settings" );
	connect( settingsAct, &QAction::triggered, this, &MainWindow::settings );

	undoAct = new QAction( "&Undo", this );
	undoAct->setShortcuts( QKeySequence::Undo );
	undoAct->setStatusTip( "Undo" );
	connect( undoAct, &QAction::triggered, this, &MainWindow::undo );

	redoAct = new QAction( "&Redo", this );
	redoAct->setShortcuts( QKeySequence::Redo );
	redoAct->setStatusTip( "Redo" );
	connect( redoAct, &QAction::triggered, this, &MainWindow::redo );

	sampleFolderSelectAct = new QAction( "&Samples", this );
	//sampleFolderSelectAct->setShortcuts( QKeySequence::Open );
	sampleFolderSelectAct->setStatusTip( "Select the samples source folder" );
	connect( sampleFolderSelectAct, &QAction::triggered,
			this, &MainWindow::sampleFolderSelect );

	projectFolderSelectAct = new QAction( "&Projects", this );
	//projectFolderSelectAct->setShortcuts( QKeySequence::Open );
	projectFolderSelectAct->setStatusTip( "Select the projects folder" );
	connect( projectFolderSelectAct, &QAction::triggered,
			this, &MainWindow::projectFolderSelect );


	// Node creation shortcuts
	nodeMixAct = new QAction( "&Mix", this );
	nodeMixAct->setShortcuts( { Qt::CTRL + Qt::SHIFT + Qt::Key_M } );
	nodeMixAct->setStatusTip( "Create mix node" );
	connect( nodeMixAct, &QAction::triggered, this, &MainWindow::createMixNode );

	nodeJoinAct = new QAction( "&Join", this );
	nodeJoinAct->setShortcuts( { Qt::CTRL + Qt::SHIFT + Qt::Key_J } );
	nodeJoinAct->setStatusTip( "Create join node" );
	connect( nodeJoinAct, &QAction::triggered, this, &MainWindow::createJoinNode );

	nodeCutAct = new QAction( "&Cut", this );
	nodeCutAct->setShortcuts( { Qt::CTRL + Qt::SHIFT + Qt::Key_C } );
	nodeCutAct->setStatusTip( "Create cut node" );
	connect( nodeCutAct, &QAction::triggered, this, &MainWindow::createCutNode );

	nodeSetVolumeAct = new QAction( "&Set Volume", this );
	nodeSetVolumeAct->setShortcuts( { Qt::CTRL + Qt::SHIFT + Qt::Key_S } );
	nodeSetVolumeAct->setStatusTip( "Create set volume node" );
	connect( nodeSetVolumeAct, &QAction::triggered, this, &MainWindow::createSetVolumeNode );

	nodePanAct = new QAction( "&Pan", this );
	nodePanAct->setShortcuts( { Qt::CTRL + Qt::SHIFT + Qt::Key_P } );
	nodePanAct->setStatusTip( "Create pan node" );
	connect( nodePanAct, &QAction::triggered, this, &MainWindow::createPanNode );
	}


void MainWindow::createMenus()
	{
	QMenu * fileMenu = menuBar()->addMenu( "&File" );
    fileMenu->addAction( openAct );
	fileMenu->addAction( newAct );
    fileMenu->addAction( saveAct );
	fileMenu->addAction( saveAsAct );
	fileMenu->addSeparator();
    fileMenu->addAction( settingsAct );
	fileMenu->addAction( sampleFolderSelectAct );
	fileMenu->addAction( projectFolderSelectAct );


	QMenu * editMenu = menuBar()->addMenu( "&Edit");
	editMenu->addAction( undoAct );
	editMenu->addAction( redoAct );
	editMenu->addAction( duplicateAct );
	editMenu->addAction( resetZoomAct );
	editMenu->addAction( findNodeAct );
//    editMenu->addSeparator();

	QMenu * nodeMenu = menuBar()->addMenu( "&Nodes" );
	nodeMenu->addAction( nodeMixAct );
	nodeMenu->addAction( nodeJoinAct );
	nodeMenu->addAction( nodeCutAct );
	nodeMenu->addAction( nodeSetVolumeAct );
	nodeMenu->addAction( nodePanAct );
    }

#include <nodes/NodeStyle>
#include <nodes/FlowViewStyle>
#include <nodes/ConnectionStyle>
#include <nodes/FlowView>
#include <LotonFlowScene.hpp>
void MainWindow::paintAs( const QPalette & palette )
    {
	//Handle all the nodes custom styling
	auto & flowViewStyle = QtNodes::FlowViewStyle::style();
	flowViewStyle.CoarseGridColor = palette.color( QPalette::Highlight );
	flowViewStyle.FineGridColor = palette.color( QPalette::Mid );
	//flowViewStyle.BackgroundColor = QColor::fromHsl( 128, 128, 200 );
	centralWidgetManager->centralWidget->flowView->resetCachedContent();

	auto & nodeStyle = QtNodes::NodeStyle::style();
	nodeStyle.SelectedBoundaryColor = palette.color( QPalette::Highlight );

	for( auto & i : centralWidgetManager->centralWidget->flowView->items() )
		i->update();


	//Handle qt styling
	QString style;

	style += QString("QWidget:item:hover, QWidget:item:selected, QMenuBar::item:pressed, QSplitter::handle:hover,"
					 "QScrollBar::handle:horizontal, QScrollBar::handle:vertical"
					 "{ background-color: #%1; }")
		.arg( palette.color( QPalette::Highlight ).rgba(), 0, 16 );

	style += QString("QMenuBar, QToolTip, QWidget, QSplitter::handle,"
					 "QMainWindow::separator, QToolBar"
					 "{ background-color: #%1; }")
		.arg( palette.color( QPalette::Mid ).rgba(), 0, 16 );

	style += QString("QTreeView, QListView, QtNodes--FlowView, QScrollArea"
					 "{ background-color: #%1; }")
		.arg( palette.color( QPalette::Dark ).rgba(), 0, 16 );

	style += QString("QtNodes--FlowView"
					 "{ background-color: #%1; }")
		.arg( palette.color( QPalette::Shadow ).rgba(), 0, 16 );


	setStyleSheet( style );
	repaint();
    }

// Helper Objects ===================================================================================

bool ReleaseNotifyColorDialogEventFilter::eventFilter( QObject * obj, QEvent * event )
		{
		if( event->type() == QEvent::MouseButtonRelease )
			emit mouseReleased();
		return QObject::eventFilter(obj, event);
		}

ReleaseNotifyColorDialog::ReleaseNotifyColorDialog( QColor c ) : QColorDialog( c )
	{
	auto eventFilter = new ReleaseNotifyColorDialogEventFilter;
	for( auto i : children() )
		i->installEventFilter( eventFilter );

	// Both connections are needed because mouseReleased and currentColorChange can be
	//		emitted in either order depending on where in the dialog you click
	QObject::connect( eventFilter, &ReleaseNotifyColorDialogEventFilter::mouseReleased, this, [this]()
		{
		emit repaintUI();
		});
	QObject::connect( this, &QColorDialog::currentColorChanged, this, [this](const QColor &)
		{
		if( ! QApplication::mouseButtons() & Qt::LeftButton )
			emit repaintUI();
		});
	}
