#pragma once

#include "PointsInPlaneModel.hpp"
#include "PointsInPlaneView.hpp"

class Func1x1Data;
class InterpolatorModel;

class PointInterpolatorModel : public PointsInPlaneModel
{
public:
	PointInterpolatorModel();
	~PointInterpolatorModel() override;

	std::shared_ptr<Func1x1Data> function();

	std::shared_ptr<InterpolatorModel> interpModel;

	QJsonObject save() const override;
	void restore( const QJsonObject & ) override;
};

class PointInterpolatorView : public PointsInPlaneView
{
public:
	PointInterpolatorView( PointInterpolatorModel * );

	virtual void drawForeground( QPainter * painter, const QRectF & rect ) override;
};

