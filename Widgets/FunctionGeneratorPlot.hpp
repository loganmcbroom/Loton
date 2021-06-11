#pragma once

#include <QWidget>

// Bells and Whistles wrapper of ControllablePlot, used for 2D function graphs

class ControllablePlot;
class QwtPlotCurve;
class QwtPlotGrid;
class QwtPlotScaleItem;
class Func1x1Data;
template<typename T> class QFuture;
template<typename T> class QFutureWatcher;

class FunctionGeneratorPlotModel : public QObject
{ Q_OBJECT
public:
	FunctionGeneratorPlotModel( const QString & _title, QObject * parent = nullptr );
	virtual ~FunctionGeneratorPlotModel();

	void setOut( std::shared_ptr<Func1x1Data> );

	std::shared_ptr<Func1x1Data> getOut() { return out; }
	const std::shared_ptr<Func1x1Data> getOut() const { return out; }
	QString getTitle() const { return title; }

signals:
	void dataUpdated();

private:
	std::shared_ptr<Func1x1Data> out;
	QString title;
};


class FunctionGeneratorPlotView : public QWidget
{ Q_OBJECT
public:
	FunctionGeneratorPlotView( FunctionGeneratorPlotModel * model, int evalFrequency = 5, QWidget * parent = nullptr );
	virtual ~FunctionGeneratorPlotView();

protected:
	void regraph();

	FunctionGeneratorPlotModel * model;

	ControllablePlot * plot;
	std::unique_ptr<QwtPlotCurve> curve;
	std::unique_ptr<QwtPlotGrid> grid;
	std::unique_ptr<QwtPlotScaleItem> xAxis, yAxis;
	const int evalFrequency;

	std::unique_ptr<QFuture<QVector<QPointF>>> points;
	std::unique_ptr<QFutureWatcher<QVector<QPointF>>> pointsWatcher;
};

class FunctionGeneratorPlotViewAdv : public FunctionGeneratorPlotView
{
public:
	FunctionGeneratorPlotViewAdv( FunctionGeneratorPlotModel * model, int evalFrequency = 5, QWidget * parent = nullptr );

};


//class FunctionGeneratorPlot : public FunctionGeneratorPlotModel, public FunctionGeneratorPlotView
//{
//public:
//	FunctionGeneratorPlot( const QString & title );
//};

