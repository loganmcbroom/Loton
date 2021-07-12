#include "SurfaceGraph.hpp"

#include <QBoxLayout>
#include <QtDataVisualization>
#include <Q3DSurface>

#include "Settings.hpp"
#include "NodeDataTypes/FunctionData.hpp"

using namespace QtDataVisualization;

// Model =====================================================+-

SurfaceGraphModel::SurfaceGraphModel()
	: QObject()
	, function( nullptr )
	, maxTime( 5.0f )
	, maxFreq( 20000.0f )
	{
	}

void SurfaceGraphModel::setFunction( std::shared_ptr<Func2x1Data> newFunction )
	{
	function = newFunction;
	emit dataUpdated();
	}

const std::shared_ptr<Func2x1Data> SurfaceGraphModel::getFunction() const
	{
	return function;
	}

void SurfaceGraphModel::setMaxTime( float t )
	{
	maxTime = t;
	emit maxTimeUpdated();
	}

void SurfaceGraphModel::setMaxFreq( float f )
	{
	maxFreq = f;
	emit maxFreqUpdated();
	}

// View ======================================================

SurfaceGraphView::SurfaceGraphView( SurfaceGraphModel * _model )
	: QWidget()
	, model( _model )
	, surface( new Q3DSurface )
	{
	QObject::connect( model, &SurfaceGraphModel::destroyed, this, [this]()
		{
		if( ! surface->seriesList().isEmpty() )
			surface->removeSeries( surface->seriesList()[0] );
		model = nullptr;
		} );

	QObject::connect( model, &SurfaceGraphModel::dataUpdated, this, [this]()
		{
		sampleFunction();
		} );
	sampleFunction();

	QObject::connect( model, &SurfaceGraphModel::maxTimeUpdated, this, [this]()
		{
		sampleFunction();
		} );
	QObject::connect( model, &SurfaceGraphModel::maxFreqUpdated, this, [this]()
		{
		sampleFunction();
		} );

	//Set Ranges
	surface->axisX()->setAutoAdjustRange( true );
	surface->axisY()->setAutoAdjustRange( true );
	surface->axisZ()->setAutoAdjustRange( true );

	//Theme setup
	auto theme = new Q3DTheme( Q3DTheme::ThemeUserDefined );

	auto & pal = Settings::palette();
	theme->setWindowColor( pal.color( QPalette::Dark ) );
	theme->setBackgroundEnabled( true );
	theme->setGridEnabled( true );
	theme->setBackgroundColor( pal.color( QPalette::Shadow ) );
	theme->setGridLineColor( pal.color( QPalette::Highlight ) );
	theme->setLabelBackgroundColor( Qt::transparent );
	theme->setLabelTextColor( Qt::transparent );

	surface->setActiveTheme( theme );

	setLayout( new QHBoxLayout );
	layout()->setMargin( 0 );
	layout()->addWidget( QWidget::createWindowContainer( surface ) );
	}

void SurfaceGraphView::sampleFunction()
	{
	if( !model ) return;
	if( !model->getFunction() ) return;

	//Sample f
	auto f = model->getFunction();

	QSurfaceDataArray * data = new QSurfaceDataArray;

	const float numSamples = 30.0f;

	for( float z = 0; z < numSamples; ++z ) // frequency
		{
		QSurfaceDataRow * row = new QSurfaceDataRow;
		for( float x = 0; x < numSamples; ++x ) // time
			{
			const float time = x / numSamples * model->getMaxTime();
			const float freq = z / numSamples * model->getMaxFreq();
			const float y = f->f( time, freq );
			*row << QVector3D( x, y, z );
			}
		*data << row;
		}

	auto series = new QSurface3DSeries;
	series->dataProxy()->resetArray( data );
	series->setDrawMode( QSurface3DSeries::DrawWireframe );

	if( ! surface->seriesList().isEmpty() )
		surface->removeSeries( surface->seriesList()[0] );
	surface->addSeries( series );
	}
