#pragma once

#include <QWidget>

// Bells and Whistles wrapper of ControllablePlot, used for 2D function graphs

class ControllableLogPlot2x1;
class QwtPlotCurve;
class QwtPlotGrid;
class QwtPlotScaleItem;
class QwtLogScaleEngine;
class Func2x1Data;
struct sample;
template<typename T> class QFuture;
template<typename T> class QFutureWatcher;

class FunctionGeneratorPlot2x1Model : public QObject
{ Q_OBJECT
public:
	FunctionGeneratorPlot2x1Model( const QString & _title, QObject * parent = nullptr );
	virtual ~FunctionGeneratorPlot2x1Model();

	void setLogMode( bool );
	bool getLogMode() const;

	void setOut( std::shared_ptr<Func2x1Data> );

	std::shared_ptr<Func2x1Data> getOut() { return out; }
	const std::shared_ptr<Func2x1Data> getOut() const { return out; }
	QString getTitle() const { return title; }

signals:
	void dataUpdated();
	void logModeUpdated( bool newMode );

private:
	bool logMode = true;
	std::shared_ptr<Func2x1Data> out;
	QString title;
};


class FunctionGeneratorPlot2x1View : public QWidget
{ Q_OBJECT
public:
	FunctionGeneratorPlot2x1View ( FunctionGeneratorPlot2x1Model * model, int evalsBase = 3, QWidget * parent = nullptr );
	virtual ~FunctionGeneratorPlot2x1View ();

protected:
	void regraph();

	FunctionGeneratorPlot2x1Model * model;

	ControllableLogPlot2x1 * plot;
	std::unique_ptr<QwtPlotCurve> curve;
	std::unique_ptr<QwtPlotGrid> grid;

	std::unique_ptr<QwtPlotScaleItem> xAxis, yAxis;
	const int evalFrequency;

	using FutureType = std::vector<sample>;
	std::unique_ptr<QFuture<FutureType>> fs;
	std::unique_ptr<QFutureWatcher<FutureType>> fsWatcher;
};

class FunctionGeneratorPlot2x1ViewAdv : public FunctionGeneratorPlot2x1View
{
public:
	FunctionGeneratorPlot2x1ViewAdv( FunctionGeneratorPlot2x1Model * model, int evalsBase = 4, QWidget * parent = nullptr );

};


