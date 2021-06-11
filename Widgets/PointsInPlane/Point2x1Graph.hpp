#pragma once

#include "PointsInPlaneModel.hpp"
#include "PointsInPlaneView.hpp"

class Func2x1Data;

class Point2x1GraphModel : public PointsInPlaneModel
{
public:
	Point2x1GraphModel();
	~Point2x1GraphModel() override;

	QJsonObject save() const override;
	void restore( const QJsonObject & ) override;

	std::shared_ptr<Func2x1Data> getOut();
	void setOut( std::shared_ptr<Func2x1Data> );

private:
	std::shared_ptr<Func2x1Data> out;
};

class Point2x1GraphView : public PointsInPlaneView
{
public:
	Point2x1GraphView( Point2x1GraphModel * );

	virtual void drawForeground( QPainter * painter, const QRectF & rect ) override;
};

