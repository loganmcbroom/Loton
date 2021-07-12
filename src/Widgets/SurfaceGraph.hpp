#pragma once

#include <QWidget>
#include <QUndoCommand>

class Func2x1Data;
namespace QtDataVisualization
	{
	class QSurface3DSeries;
	class Q3DSurface;
	}

class SurfaceGraphModel : public QObject
{ Q_OBJECT
public:
	SurfaceGraphModel();

	void setFunction( std::shared_ptr<Func2x1Data> );
	const std::shared_ptr<Func2x1Data> getFunction() const;

	float getMaxTime() const { return maxTime; }
	float getMaxFreq() const { return maxFreq; }
	void setMaxTime( float );
	void setMaxFreq( float );

signals:
	void dataUpdated();
	void maxTimeUpdated();
	void maxFreqUpdated();

private:
	std::shared_ptr<Func2x1Data> function;
	float maxTime;
	float maxFreq;
};

class SurfaceGraphView : public QWidget
{
public:
	SurfaceGraphView( SurfaceGraphModel * model);

private:
	void sampleFunction();

	SurfaceGraphModel * model;
	QtDataVisualization::Q3DSurface * surface;
};

