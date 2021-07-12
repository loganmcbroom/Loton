#include "FunctionGeneratorPlot.hpp"

#include <QtConcurrent>

#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_scaleitem.h"
#include "qwt_scale_widget.h"

#include "Settings.hpp"
#include "Widgets/ControllablePlot.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include <QBoxLayout>
#include <QLabel>

FunctionGeneratorPlotModel::FunctionGeneratorPlotModel( const QString & _title, QObject * parent )
	: QObject( parent )
	, out( nullptr )
	, title( _title )
	{
	}

FunctionGeneratorPlotModel::~FunctionGeneratorPlotModel()
	{
	}

void FunctionGeneratorPlotModel::setOut( std::shared_ptr<Func1x1Data> newOut )
	{
	out = newOut;
	emit dataUpdated();
	}



// =====================================================================================================



FunctionGeneratorPlotView::FunctionGeneratorPlotView( FunctionGeneratorPlotModel * _model, int evalFreq, QWidget * parent )
	: QWidget( parent )
	, model( _model )
	, plot( new ControllablePlot )
	, curve( new QwtPlotCurve )
	, grid( new QwtPlotGrid )
	, xAxis( new QwtPlotScaleItem( QwtScaleDraw::TopScale, 0.0 ) )
	, yAxis( new QwtPlotScaleItem( QwtScaleDraw::RightScale, 0.0 ) )
	, evalFrequency( evalFreq )
	{
	QObject::connect( model, &QObject::destroyed, this, [this]()
		{
		model = nullptr;
		});

	auto layout = new QHBoxLayout;
	setLayout( layout );

	layout->addWidget( plot );

	QPalette pal = Settings::palette();
	pal.setColor( QPalette::WindowText, Qt::white );
	pal.setColor( QPalette::Text, Qt::transparent );

	plot->replot();
	Settings::setDynamicColor( plot, "background-color: #%1;", QPalette::Dark );
	QwtText qwtTitle( model->getTitle() );
	QFont font;
	font.setStyleHint( QFont::Monospace );
	font.setPointSize( 12 );
	qwtTitle.setFont( font );
	plot->setTitle( "" );

	grid->attach( plot );
	grid->setPen( pal.color( QPalette::Mid ), 1.0 );

	QFont axisFont;
	axisFont.setStyleHint( QFont::Monospace );
	axisFont.setPointSize( 10 );

	xAxis->setFont( axisFont );
	xAxis->setPalette( pal );
	xAxis->attach( plot );

	yAxis->setFont( axisFont );
	yAxis->setPalette( pal );
	yAxis->attach( plot );

	curve->attach( plot );
	QObject::connect( Settings::get(), &Settings::paletteUpdated, this, [this]( QPalette::ColorRole role )
		{
		if( role == QPalette::Highlight )
			{
			auto pal = Settings::palette();
			curve->setPen( pal.color( QPalette::Highlight ), 2.0 );
			plot->replot();
			}
		} );
	curve->setPen( pal.color( QPalette::Highlight ), 2.0 );

	QObject::connect( plot, &ControllablePlot::viewChanged,
					this, &FunctionGeneratorPlotView::regraph );

	QObject::connect( model, &FunctionGeneratorPlotModel::dataUpdated,
					this, &FunctionGeneratorPlotView::regraph );

	plot->enableAxis( QwtPlot::Axis::xBottom, false );
	plot->enableAxis( QwtPlot::Axis::yLeft, false );

	regraph();
	}

FunctionGeneratorPlotView::~FunctionGeneratorPlotView() {}

/*
 * Uses the contents of out to update and redraw the curve and graph
 */
void FunctionGeneratorPlotView::regraph()
	{
	if( !model || !model->getOut() )
		{
		//draw blank here
		return;
		}

	pointsWatcher = std::make_unique<QFutureWatcher<QVector<QPointF>>>();

	const int numPoints = plot->width() / evalFrequency;
	const auto xScale = plot->axisInterval( QwtPlot::Axis::xBottom );
	std::shared_ptr<Func1x1Data> outPtr = model->getOut();
	points = std::make_unique<QFuture<QVector<QPointF>>>( QtConcurrent::run( [numPoints, xScale, outCopy = outPtr->f]()
		{
		QVector<QPointF> newPoints( numPoints );
		const double margin = xScale.width() / 20.0;
		for( double i = xScale.minValue() - margin, j = 0; j < newPoints.size(); i += (xScale.width() + 2.0 * margin) / double(newPoints.size()), ++j )
			newPoints[j] = { i, outCopy( i ) };
		return newPoints;
		}));

	QObject::connect( pointsWatcher.get(), &QFutureWatcher<std::vector<QPointF>>::finished, this, [this]()
		{
		curve->setSamples( points->result() );
		plot->replot();
		} );

	pointsWatcher->setFuture( *points );
	}



// =====================================================================================================



FunctionGeneratorPlotViewAdv::FunctionGeneratorPlotViewAdv( FunctionGeneratorPlotModel * _model, int ef, QWidget * parent )
	: FunctionGeneratorPlotView( _model, ef, parent )
	{
	plot->setTitle( model->getTitle() );

	plot->enableAxis( QwtPlot::Axis::xBottom, true );
	plot->enableAxis( QwtPlot::Axis::yLeft, true );
	}
