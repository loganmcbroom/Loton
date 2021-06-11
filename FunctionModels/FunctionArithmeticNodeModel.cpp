#include "FunctionArithmeticNodeModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include <nodes/Connection>

#include "NodeDataTypes/FunctionData.hpp"
#include "Widgets/StringDisplay.hpp"
#include "Widgets/NumberSlider.hpp"

FunctionArithmeticNodeModel::FunctionArithmeticNodeModel( const QString & _symbol )
	: LotonNodeModel()
	, ins( 2 )
	, symbol( _symbol )
	, stringModel( new StringDisplayModel( "Missing Inputs" ) )
	, xSliderModel( new NumberSliderModel( 0 ) )
	, ySliderModel( new NumberSliderModel( 0 ) )
	{
	auto layout = new QVBoxLayout;
	mainWidget->setLayout( layout );
	auto label = new QLabel( symbol );
	layout->addWidget( label );

	//updateDisplay();
	QObject::connect( xSliderModel.get(), &NumberSliderModel::stateChanged, this, &FunctionArithmeticNodeModel::updateDisplay );
	QObject::connect( ySliderModel.get(), &NumberSliderModel::stateChanged, this, &FunctionArithmeticNodeModel::updateDisplay );

	QFont font;
	font.setPointSize( 18 );
	label->setFont( font );
	}

FunctionArithmeticNodeModel::~FunctionArithmeticNodeModel()
	{
	}

void FunctionArithmeticNodeModel::setInData( std::shared_ptr<NodeData> data, PortIndex index  )
	{
	if( !data ) ins[index].reset();
	else ins[index] = std::dynamic_pointer_cast<Func2x1Data>( data );

	if( ins[0] && ins[1] ) out = operation( ins );

	updateDisplay();
	}

std::shared_ptr<NodeData> FunctionArithmeticNodeModel::outData( QtNodes::PortIndex )
	{
	return out;
	}

NodeDataType FunctionArithmeticNodeModel::dataType( PortType, PortIndex ) const
	{
	return Func2x1Data::Type();
	}

unsigned int FunctionArithmeticNodeModel::nPorts( PortType type ) const
	{
	return type == PortType::In ? 2 : 1;
	}

void FunctionArithmeticNodeModel::inputConnectionDeleted( PortIndex i )
	{
	ins[i].reset();
	updateDisplay();
	}

QWidget * FunctionArithmeticNodeModel::makeHeaderWidget()
	{
	auto stringView = new StringDisplayView( stringModel.get() );
	stringView->setFixedHeight( 50 );
	return stringView;
	}

ControllerPairs FunctionArithmeticNodeModel::makeInputControllers()
	{
	ControllerPairs controllers
		{
		{ "Sample x", new NumberSliderView( xSliderModel.get() ) },
		{ "Sample y", new NumberSliderView( ySliderModel.get() ) }
		};

	controllers[0].second->setFixedHeight( 30 );
	controllers[1].second->setFixedHeight( 30 );

	return controllers;
	}

void FunctionArithmeticNodeModel::updateDisplay()
	{
	if( ins[0] && ins[1] )
		{
		auto truncateToString = []( float x )
			{
			return QString::number( std::round( x * 1000.0f ) / 1000.0f );
			};
		float x = (float) xSliderModel->getSliderPosition();
		float y = (float) ySliderModel->getSliderPosition();
		stringModel->setString(
			  truncateToString( ins[0]->f( x, y ) ) + " " + symbol + " "
			+ truncateToString( ins[1]->f( x, y ) ) + " = "
			+ truncateToString( out->f( x, y ) ) );
		setValidationState( QtNodes::NodeValidationState::Valid, "" );
		}
	else
		{
		stringModel->setString( "Missing Inputs" );
		setValidationState( QtNodes::NodeValidationState::Warning, "Missing inputs" );
		}
	}

/*
 * Operations
 */

std::shared_ptr<Func2x1Data>
FunctionAdditionNodeModel::operation( const std::vector<std::shared_ptr<Func2x1Data>> & ins )
	{
	return std::make_shared<Func2x1Data>( ins[0]->f + ins[1]->f );
	}
std::shared_ptr<Func2x1Data>
FunctionSubtractionNodeModel::operation( const std::vector<std::shared_ptr<Func2x1Data>> & ins )
	{
	return std::make_shared<Func2x1Data>( ins[0]->f - ins[1]->f );
	}
std::shared_ptr<Func2x1Data>
FunctionMultiplicationNodeModel::operation( const std::vector<std::shared_ptr<Func2x1Data>> & ins )
	{
	return std::make_shared<Func2x1Data>( ins[0]->f * ins[1]->f );
	}
std::shared_ptr<Func2x1Data>
FunctionDivisionNodeModel::operation( const std::vector<std::shared_ptr<Func2x1Data>> & ins )
	{
	return std::make_shared<Func2x1Data>( ins[0]->f / ins[1]->f );
	}

