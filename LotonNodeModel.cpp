#include "LotonNodeModel.hpp"

#include <QBoxLayout>

#include "MainWindow.hpp"
#include "CentralWidget.hpp"
#include "NodeControlPanel.hpp"
#include "NodeDataTypes/LotonNodeData.hpp"
#include "NodeDataTypes/WipeNodeData.hpp"

using namespace QtNodes;

LotonNodeModel::LotonNodeModel()
	: mainWidget( new QWidget )
	, _embeddedWidget( new QWidget )
	, modelValidationState( NodeValidationState::Valid )
	, modelValidationError( "" )
	, ins( 1 )
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

void LotonNodeModel::setInData( std::shared_ptr<NodeData> data, PortIndex i )
	{
	ins.resize( nPorts( PortType::In ) );
	ins[i] = std::static_pointer_cast<LotonNodeData>( data );
	if( ins[i] && ins[i]->wipe() )
		{
		// Wipe inputs?
//		for( auto & in : ins )
//			in = makeWipe();
		wipeOutputs( i );
		}
	// If any input is connected but wiped, don't process
	if( ! hasWipedInput() )
		inputsUpdated( data, i );
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

std::shared_ptr<LotonNodeData> LotonNodeModel::makeWipe() const
	{
	return std::make_shared<WipeNodeData>();
	}

bool LotonNodeModel::hasWipedInput() const
	{
	return std::any_of( ins.begin(), ins.end(), []( std::shared_ptr<LotonNodeData> a ){ return a && a->wipe(); } );
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


