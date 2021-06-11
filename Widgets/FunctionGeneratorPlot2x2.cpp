#include "FunctionGeneratorPlot2x2.hpp"

#include <QtConcurrent>

#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_scaleitem.h"
#include "qwt_scale_widget.h"
#include "qwt_scale_engine.h"

#include "Settings.hpp"
#include "Widgets/ControllableLogPlot.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include <QBoxLayout>
#include <QLabel>
#include <QPainterPath>

FunctionGeneratorPlot2x2Model::FunctionGeneratorPlot2x2Model( const QString & _title, QObject * parent )
	: QObject( parent )
	, out( nullptr )
	, title( _title )
	{
	}

FunctionGeneratorPlot2x2Model::~FunctionGeneratorPlot2x2Model() {}

bool FunctionGeneratorPlot2x2Model::getLogMode() const
	{
	return logMode;
	}

void FunctionGeneratorPlot2x2Model::setLogMode( bool newMode )
	{
	logMode = newMode;
	emit logModeUpdated( newMode );
	emit dataUpdated();
	}

void FunctionGeneratorPlot2x2Model::setOut( std::shared_ptr<Func2x2Data> newOut )
	{
	out = newOut;
	emit dataUpdated();
	}



// =====================================================================================================

FunctionGeneratorPlot2x2View::FunctionGeneratorPlot2x2View( FunctionGeneratorPlot2x2Model * _model, int evalFreq, QWidget * parent )
	: QWidget( parent )
	, model( _model )
	, plot( new ControllableLogPlot2x2( model->getLogMode() ) )
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

	QObject::connect( model, &FunctionGeneratorPlot2x2Model::logModeUpdated, plot, &ControllableLogPlot2x1::setLogMode );

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

	QObject::connect( plot, &ControllableLogPlot::viewChanged,
					this, &FunctionGeneratorPlot2x2View::regraph );

	QObject::connect( model, &FunctionGeneratorPlot2x2Model::dataUpdated,
					this, &FunctionGeneratorPlot2x2View::regraph );

	plot->enableAxis( QwtPlot::Axis::xBottom, false );
	plot->enableAxis( QwtPlot::Axis::yLeft, false );

	regraph();
	}

FunctionGeneratorPlot2x2View::~FunctionGeneratorPlot2x2View() {}

/*
 * Uses the contents of out to update and redraw the curve and graph
 */
void FunctionGeneratorPlot2x2View::regraph()
	{
	if( !model || !model->getOut() )
		{
		//draw blank here
		return;
		}

	fsWatcher = std::make_unique<QFutureWatcher<FutureType>>();

	QwtInterval xInterval = plot->axisInterval( QwtPlot::Axis::xBottom );
	QwtInterval yInterval = plot->axisInterval( QwtPlot::Axis::yLeft );
	fs = std::make_unique<QFuture<FutureType>>( QtConcurrent::run(
		[evals = std::pow( 2, evalFrequency ), xInterval, yInterval, fCopy = model->getOut()->f, logMode = model->getLogMode()]()
		{
		const StepHelper modXInterval( evals, xInterval, false );
		const StepHelper modYInterval( evals, yInterval, logMode );

		FutureType newPoints;
		for( float y = modYInterval.start; y <= modYInterval.end; y = modYInterval( y ) )
			for( float x = modXInterval.start; x <= modXInterval.end; x = modXInterval( x ) )
				{
				const auto v = fCopy( x, y );
				newPoints.push_back( { x, y, v.x(), v.y() } );
				}

		return newPoints;
		}));

	QObject::connect( fsWatcher.get(), &QFutureWatcher<FutureType>::finished, this, [this]()
		{
		plot->fs = std::make_shared<FutureType>( fs->result() );
		plot->replot();
		} );

	fsWatcher->setFuture( *fs );
	plot->replot();
	}

// =====================================================================================================



FunctionGeneratorPlot2x2ViewAdv::FunctionGeneratorPlot2x2ViewAdv( FunctionGeneratorPlot2x2Model * _model, int ef, QWidget * parent )
	: FunctionGeneratorPlot2x2View( _model, ef, parent )
	{
	plot->setTitle( model->getTitle() );

	plot->enableAxis( QwtPlot::Axis::xBottom, true );
	plot->enableAxis( QwtPlot::Axis::yLeft, true );
	}
