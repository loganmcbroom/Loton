#include "FunctionGeneratorNodeModel.hpp"

#include <QBoxLayout>
#include <QLabel>

#include "Widgets/NumberSlider.hpp"
#include "Widgets/FunctionGeneratorPlot.hpp"
#include "NodeDataTypes/FunctionData.hpp"

using namespace QtNodes;

FunctionGeneratorNodeModel::FunctionGeneratorNodeModel()
	: FunctionNodeModel()
	, sliders()
	, plotModel( nullptr )
	{
	//QObject::connect( this, &FunctionNodeModel::dataUpdatedDynamic, this, [this](){ generateGraphFunction( v ); } );
	}

FunctionGeneratorNodeModel::~FunctionGeneratorNodeModel() = default;

void FunctionGeneratorNodeModel::initialize()
	{
	auto mainLayout = new QHBoxLayout( mainWidget );
	mainWidget->setLayout( mainLayout );
	mainLayout->setContentsMargins( 0,3,0,0 );
	mainLayout->setSpacing( 0 );

	auto sliderLayout = new QVBoxLayout;
	mainLayout->addLayout( sliderLayout );
	sliderLayout->setSpacing( 0 );

	sliders.resize( numSliders() );
	for( int i = 0; i < numSliders(); ++i )
		{
		sliders[i] = new NumberSlider( defaultSliderValue( i ), sliderLowerBound( i ), sliderUpperBound( i ) );
		sliderLayout->addWidget( sliders[i] );
		sliders[i]->setFixedSize( 64, 20 );

		QObject::connect( sliders[i], &NumberSliderModel::stateChangedDynamic,
					this, &FunctionGeneratorNodeModel::updateOutput );

		QObject::connect( sliders[i], &NumberSliderModel::stateChanged,
					this, [this]()
					{
					emit computingFinished();
					emit dataUpdated( 0 );
					} );
		}

	plotModel = new FunctionGeneratorPlotModel( graphTitle(), this );

	auto plotView = new FunctionGeneratorPlotView( plotModel, evalFrequency() );
	mainLayout->addWidget( plotView );
	plotView->setFixedSize( 120, 120 );
	plotView->setStyleSheet( "QwtPlot { border: 1px solid white; }" );

	QObject::connect( this, &FunctionGeneratorNodeModel::dataUpdatedDynamic, plotModel, [this]()
		{
		if( out && !graphFunction )
			plotModel->setOut( uncomposedOut );
		else
			plotModel->setOut( graphFunction );
		} );

	updateOutput();
	emit dataUpdated( 0 );
	}

void FunctionGeneratorNodeModel::generateGraphFunction( const std::vector<float> & )
	{
	graphFunction.reset();
	}

float FunctionGeneratorNodeModel::sliderLowerBound( size_t ) const
	{
	return -NumberSlider::infinity;
	}

float FunctionGeneratorNodeModel::sliderUpperBound( size_t ) const
	{
	return NumberSlider::infinity;
	}

ControllerPairs FunctionGeneratorNodeModel::makeInputControllers()
	{
	ControllerPairs controllers( numSliders() );

	for( int i = 0; i < numSliders(); ++i )
		{
		NumberSliderView * slider = new NumberSliderView( sliders[i] );
		slider->setFixedHeight( 30 );
		controllers[i] = { sliderCaption(i), slider };
		}

	return controllers;
	}

QWidget * FunctionGeneratorNodeModel::makeHeaderWidget()
	{
	auto plotView = new FunctionGeneratorPlotViewAdv( plotModel, evalFrequency() );
	plotView->setFixedHeight( 300 );
	return plotView;
	}

int FunctionGeneratorNodeModel::evalFrequency() const
	{
	return 5;
	}

QJsonObject FunctionGeneratorNodeModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	for( int i = 0; i < sliders.size(); ++i )
		modelJson[ QString( "Slider" ) + QString::number( i ) ] = sliders[i]->save();
	return modelJson;
	}

void FunctionGeneratorNodeModel::restore( QJsonObject const & p )
	{
	for( int i = 0; i < sliders.size(); ++i )
		sliders[i]->restore( p[ QString( "Slider" ) + QString::number( i ) ].toObject() );
	}

std::shared_ptr<Func1x1Data> FunctionGeneratorNodeModel::function()
	{
	std::vector<float> v( sliders.size() );
	for( int i = 0; i < v.size(); ++i )
		v[i] = sliders[i]->getSliderPosition();

	generateGraphFunction( v );

	return sliderFunction( v );
	}
