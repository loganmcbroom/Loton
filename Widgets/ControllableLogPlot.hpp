#pragma once

#include <QMouseEvent>

#include <qwt_plot.h>

class Func2x1Data;

struct sample
	{
	float x;
	float y;
	float z;
	};

struct sample2
	{
	float x;
	float y;
	float z;
	float w;
	};

float logBase( float arg, float base );
float logCeil( float x );

struct StepHelper
	{
	StepHelper( double evals, QwtInterval interval, bool _logMode )
		: logMode( _logMode )
		, jump()
		, start()
		, end()
		{
		if( logMode )
			{
			const float linearDist = std::log10( interval.maxValue() / interval.minValue() );
			jump = std::pow( 10.0, logCeil( linearDist ) / evals );
			start = std::pow( jump, std::ceil(  logBase( interval.minValue(), jump ) ) );
			end = std::pow( jump, std::floor( logBase( interval.maxValue(), jump ) ) );
			}
		else
			{
			jump = logCeil( interval.width() ) / evals;
			start =  std::ceil( interval.minValue() / jump ) * jump;
			end = std::floor( interval.maxValue() / jump ) * jump;
			}
		}

	float operator()( float a ) const
		{
		return logMode?
			a * jump:
			a + jump;
		}

	bool logMode;
	float jump, start, end;
	};

// 2D function graph
class ControllableLogPlot : public QwtPlot
{ Q_OBJECT
public:
	ControllableLogPlot( bool logMode );

	bool getLogMode() const;
	void setLogMode( bool );

	void mousePressEvent( QMouseEvent * me ) override;
	void mouseReleaseEvent( QMouseEvent * me ) override;
	void mouseMoveEvent( QMouseEvent * me ) override;
	void wheelEvent( QWheelEvent * me ) override;

	bool isClicked = false;
	QPoint position;

signals:
	void viewChanged();

private:
	bool logMode;
	void resizeEvent( QResizeEvent * ) override;
	QSize minimumSizeHint() const override;

	constexpr static double logLowerBound = 1.0;
	constexpr static double logUpperBound = 24000.0;
};

class ControllableLogPlot2x1 : public ControllableLogPlot
{
public:
	ControllableLogPlot2x1( bool logMode ) : ControllableLogPlot( logMode ) {}
	virtual void drawCanvas( QPainter * ) override;
	std::shared_ptr<std::vector<sample>> fs;
};

class ControllableLogPlot2x2 : public ControllableLogPlot
{
public:
	ControllableLogPlot2x2( bool logMode ) : ControllableLogPlot( logMode ) {}
	virtual void drawCanvas( QPainter * ) override;
	std::shared_ptr<std::vector<sample2>> fs;
};
