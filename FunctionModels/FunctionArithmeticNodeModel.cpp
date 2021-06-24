#include "FunctionArithmeticNodeModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include <nodes/Connection>

#include "NodeDataTypes/FunctionData.hpp"
#include "Widgets/StringDisplay.hpp"
#include "Widgets/NumberSlider.hpp"

FunctionArithmeticNodeModel::FunctionArithmeticNodeModel( const QString & _symbol )
	: LotonNodeModel()
	, symbol( _symbol )
	, stringModel( new StringDisplayModel( "" ) )
	, sliderModels(  )
	{
	ins.resize( 2 );

	sliderModels.emplace_back( new NumberSliderModel( 0 ) );
	sliderModels.emplace_back( new NumberSliderModel( 0 ) );

	auto layout = new QVBoxLayout;
	mainWidget->setLayout( layout );
	//auto label = new QLabel( symbol );
	//layout->addWidget( label );
	auto stringView = new StringDisplayView( stringModel.get() );
	stringView->setFixedWidth( 140 );
	stringView->setFixedHeight( 25 );
	layout->addWidget( stringView );

	QObject::connect( sliderModels[0].get(), &NumberSliderModel::stateChangedDynamic, this, &FunctionArithmeticNodeModel::updateDisplay );
	QObject::connect( sliderModels[1].get(), &NumberSliderModel::stateChangedDynamic, this, &FunctionArithmeticNodeModel::updateDisplay );
	QObject::connect( sliderModels[0].get(), &NumberSliderModel::stateChanged, this, &FunctionArithmeticNodeModel::updateOutput );
	QObject::connect( sliderModels[1].get(), &NumberSliderModel::stateChanged, this, &FunctionArithmeticNodeModel::updateOutput );
	QObject::connect( this, &LotonNodeModel::dataUpdated, this, [this]( int ){ updateDisplay(); } );

//	QFont font;
//	font.setPointSize( 18 );
	//label->setFont( font );

	updateDisplay();
	}

FunctionArithmeticNodeModel::~FunctionArithmeticNodeModel() = default;

void FunctionArithmeticNodeModel::inputsUpdated( std::shared_ptr<NodeData>, PortIndex  )
	{
	updateOutput();
	}

void FunctionArithmeticNodeModel::wipeOutputs( PortIndex )
	{
	out = makeWipe();
	emit dataUpdated( 0 );
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
		{ "Top port default", new NumberSliderView( sliderModels[0].get() ) },
		{ "Bottom port default", new NumberSliderView( sliderModels[1].get() ) }
		};

	controllers[0].second->setFixedHeight( 30 );
	controllers[1].second->setFixedHeight( 30 );

	return controllers;
	}

QJsonObject FunctionArithmeticNodeModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["x"] = sliderModels[0]->save();
	modelJson["y"] = sliderModels[1]->save();
	return modelJson;
	}

void FunctionArithmeticNodeModel::restore( QJsonObject const & p )
	{
	sliderModels[0]->restore( p["x"].toObject() );
	sliderModels[0]->restore( p["y"].toObject() );
	}

void FunctionArithmeticNodeModel::updateOutput()
	{
	out = operation();
	emit dataUpdated( 0 );
	}

void FunctionArithmeticNodeModel::updateDisplay()
	{
	auto truncateToString = []( float x )
		{
		return QString::number( std::round( x * 1000.0f ) / 1000.0f );
		};
	const float x = insCast( 0 )->f( 0, 0 );
	const float y = insCast( 1 )->f( 0, 0 );
	if( out ) stringModel->setString(
		  truncateToString( x ) + " " + symbol + " "
		+ truncateToString( y ) + " = "
		+ truncateToString( std::static_pointer_cast<Func2x1Data>( out )->f( 0, 0 ) ) );
	setValidationState( QtNodes::NodeValidationState::Valid, "" );
	}

std::shared_ptr<Func2x1Data> FunctionArithmeticNodeModel::insCast( int i ) const
	{
	if( ins[i] )
		return std::static_pointer_cast<Func2x1Data>( ins[i] );
	else
		return std::make_shared<Func2x1Data>( sliderModels[i]->getSliderPosition() );

	//return tryLockingInput<Func2x1Data>( ins[i], sliderModels[i]->getSliderPosition() );
	}

/*
 * Operations
 */

std::shared_ptr<NodeData>
FunctionAdditionNodeModel::operation()
	{
	return std::make_shared<Func2x1Data>( insCast(0)->f + insCast(1)->f );
	}
std::shared_ptr<NodeData>
FunctionSubtractionNodeModel::operation()
	{
	return std::make_shared<Func2x1Data>( insCast(0)->f - insCast(1)->f );
	}
std::shared_ptr<NodeData>
FunctionMultiplicationNodeModel::operation()
	{
	return std::make_shared<Func2x1Data>( insCast(0)->f * insCast(1)->f );
	}
std::shared_ptr<NodeData>
FunctionDivisionNodeModel::operation()
	{
	return std::make_shared<Func2x1Data>( insCast(0)->f / insCast(1)->f );
	}

