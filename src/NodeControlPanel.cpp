#include "NodeControlPanel.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QResizeEvent>

#include "LotonNodeModel.hpp"

#include "Settings.hpp"

NodeControlPanel::NodeControlPanel( QWidget * parent )
	: QScrollArea( parent )
	, _activeNode( nullptr )
	{
	setWidgetResizable( true );

	Settings::setDynamicColor( this, "QScrollArea { border: 3px solid #%1; }", QPalette::Dark );

	setActiveNode( nullptr );
	}

void NodeControlPanel::setActiveNode( LotonNodeModel * newNode )
	{
	if( newNode == _activeNode ) return;

	_activeNode = newNode;

	//Create new main widget
	auto newScrollAreaWidget = new QWidget(); // Memory managed by setWidget
	newScrollAreaWidget->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
	Settings::setDynamicColor( newScrollAreaWidget, "QWidget { background-color: #%1; }", QPalette::Dark );

	// Set up main widget layout
	auto mainLayout = new QVBoxLayout();
	newScrollAreaWidget->setLayout( mainLayout );
	mainLayout->setMargin( 0 );
	mainLayout->setSpacing( 3 );

	// "Note that you must add the layout of widget before you call this function"
	setWidget( newScrollAreaWidget );

	if( newNode == nullptr ) return;

	//Header Setup
	auto header = new QWidget;
	mainLayout->addWidget( header );
	Settings::setDynamicColor( header, "QWidget {background-color: #%1; }", QPalette::Mid );

	auto headerLayout = new QVBoxLayout;
	header->setLayout( headerLayout );

	auto title = new QLabel( newNode->caption(), header );
	headerLayout->addWidget( title );
	title->setAlignment( Qt::AlignCenter );

	if( auto customNodeHeaderWidget = newNode->makeHeaderWidget() )
		{
		headerLayout->addWidget( customNodeHeaderWidget );
		//customNodeHeaderWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
		}

	//Remaining Controllers Setup
	auto controllerPairs = newNode->makeInputControllers();

	for( auto & controller : controllerPairs )
		{
		auto controlWidget = new QWidget;
		mainLayout->addWidget( controlWidget );
		Settings::setDynamicColor( controlWidget, "QWidget { background-color: #%1; }", QPalette::Mid );

		auto controlLayout = new QHBoxLayout;
		controlWidget->setLayout( controlLayout );
		controlWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );

		if( ! controller.first.isNull() )
			{
			auto sliderLabel = new QLabel( controller.first );
			controlLayout->addWidget( sliderLabel );
			sliderLabel->setAlignment( Qt::AlignCenter );
			sliderLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
			}

		controller.second->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
		controlLayout->addWidget( controller.second );
		}
	}

LotonNodeModel * NodeControlPanel::activeNode()
	{
	return _activeNode;
	}

void NodeControlPanel::resizeEvent( QResizeEvent * e )
	{
	QScrollArea::resizeEvent( e );
	if( widget() )
		{
		widget()->setFixedWidth( e->size().width() );
		widget()->setContentsMargins( 0, 0, verticalScrollBar()->isVisible()? 3 : 0, 0 );
		}
	}



//#include "NodeControlPanel.hpp"

//#include <QHBoxLayout>
//#include <QLabel>

//#include "LotonNodeModel.hpp"

//#include "Settings.hpp"

//NodeControlPanel::NodeControlPanel( QWidget * parent )
//	: QListWidget( parent )
//	, activeNode( nullptr )
//	{
//	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
//	setSpacing( 3 );

//	//setContentsMargins( 0, 0, 0, 0 );

//	setStyleSheet( "QListWidget { margin: 0px; border-width: 0px; }"
//				   "QListView::item { padding: 0px; }" );
//	}

//void NodeControlPanel::setActivePanel( LotonNodeModel * newNode )
//	{
//	if( newNode == activeNode ) return;

//	activeNode = newNode;

//	//Reset scroll area
//	//setWidget( new QWidget );

//	if( newNode == nullptr ) return;

//	//Header Setup
//	auto header = new QWidget;

//	auto headerLayout = new QVBoxLayout;
//	header->setLayout( headerLayout );

//	auto title = new QLabel( newNode->caption(), header );
//	headerLayout->addWidget( title );
//	title->setAlignment( Qt::AlignCenter );

//	if( auto customNodeHeaderWidget = newNode->makeHeaderWidget() )
//		{
//		headerLayout->addWidget( customNodeHeaderWidget );
//		customNodeHeaderWidget->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed );
//		}

//	QListWidgetItem * headerItem = new QListWidgetItem( this );
//	addItem( headerItem );
//	headerItem->setSizeHint( header->sizeHint() );
//	setItemWidget( headerItem, header );

//	//Remaining Controllers Setup
//	auto controllerPairs = newNode->makeInputControllers();

//	for( auto & controller : controllerPairs )
//		{
//		auto controlWidget = new QWidget;
//		auto controlLayout = new QHBoxLayout;
//		controlWidget->setLayout( controlLayout );
//		controlWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );

//		auto sliderLabel = new QLabel( controller.first );
//		controlLayout->addWidget( sliderLabel );
//		//sliderLabel->setMargin( 5 );
//		sliderLabel->setAlignment( Qt::AlignCenter );
//		sliderLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );

//		controller.second->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
//		controlLayout->addWidget( controller.second );

//		QListWidgetItem * controllerItem = new QListWidgetItem( this );
//		addItem( controllerItem );
//		controllerItem->setSizeHint( controlWidget->sizeHint() );
//		setItemWidget( controllerItem, controlWidget );
//		}
//	}

//const LotonNodeModel * NodeControlPanel::getActiveNode() const
//	{
//	return activeNode;
//	}

//void NodeControlPanel::resizeEvent( QResizeEvent * )
//	{
//	for( int i = 0; i < count(); ++i )
//		{
//		QListWidgetItem * currentItem = item( i );
//		QWidget * w = itemWidget( currentItem );
//		if( w )
//			currentItem->setSizeHint( w->sizeHint() );
//		}
//	}

