#pragma once

#include "FunctionNodeModel.hpp"

class PointInterpolatorModel;
class InterpolatorModel;

class Func1x1PointInterpolationModel : public Function1x1NodeModel
{ Q_OBJECT
public:
	Func1x1PointInterpolationModel();
	~Func1x1PointInterpolationModel() override;

	virtual QString caption() const override;
	virtual QString name() const override;
	virtual std::shared_ptr<Func1x1Data> function() override;

	virtual QJsonObject save() const override;
	virtual void restore( const QJsonObject & p ) override;
	virtual ControllerPairs makeInputControllers() override;
	virtual QWidget * makeHeaderWidget() override;

	std::unique_ptr<PointInterpolatorModel> piModel;
};

