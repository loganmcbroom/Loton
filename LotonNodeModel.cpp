#include "LotonNodeModel.hpp"

#include <QBoxLayout>

#include "MainWindow.hpp"
#include "CentralWidget.hpp"
#include "NodeControlPanel.hpp"

using namespace QtNodes;

LotonNodeModel::LotonNodeModel()
	: mainWidget( new QWidget )
	, _embeddedWidget( new QWidget )
	, modelValidationState( NodeValidationState::Valid )
	, modelValidationError( "" )
	//, controllers()
	{
	//Layout and styling
	_embeddedWidget->setStyleSheet(" QWidget { background-color: rgba(0,0,0,0); } ");
	auto embeddedLayout = new QHBoxLayout;
	_embeddedWidget->setLayout( embeddedLayout );
	embeddedLayout->setMargin( 0 );
	embeddedLayout->addWidget( mainWidget );
	}

LotonNodeModel::~LotonNodeModel()
	{
	if( ! MainWindow::getInstance() ) return;
	auto panel = MainWindow::getInstance()->centralWidgetManager->centralWidget->controlPanel;
	if( panel->activeNode() == this )
		panel->setActiveNode( nullptr );
	}

//QJsonObject LotonNodeModel::save() const
//	{
//	}

//void LotonNodeModel::restore( const QJsonObject & p )
//	{
//	}

//getPortIndex
//void LotonNodeModel::inputConnectionCreated ( const QtNodes::Connection & c ) {}
//void LotonNodeModel::inputConnectionDeleted ( const QtNodes::Connection & c ) {}
//void LotonNodeModel::outputConnectionCreated( const QtNodes::Connection & c ) {}
//void LotonNodeModel::outputConnectionDeleted( const QtNodes::Connection & c ) {}

void LotonNodeModel::clicked()
	{
	MainWindow::setActiveNode( this );
	}

void LotonNodeModel::setValidationState( QtNodes::NodeValidationState state, QString msg )
	{
	modelValidationState = state;
	modelValidationError = msg;
	}

//void LotonNodeModel::addController( LotonController * c )
//	{
//	controllers.emplace_back( c );
//	}

bool LotonNodeModel::captionVisible() const
	{
	return true;
	}

bool LotonNodeModel::portCaptionVisible( PortType type, PortIndex ) const
	{
	return type == PortType::Out;
	}


