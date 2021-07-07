#pragma once

#include "FlanProcessModel.hpp"

class NumberSliderModel;
struct AudioToPVOCConverter;
class FunctionGeneratorPlotModel;

struct AudioConvertToFunctionModel : public FlanProcessModel
	{
	AudioConvertToFunctionModel();
	~AudioConvertToFunctionModel() override;

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	QWidget * makeHeaderWidget() override;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(Extracts a function approximating volume over time. The extracted function is generated by linear approximations. Only the first channel is processed.

Granularity - Number:
	This parameter decides how often (in ms) the current average gain is sampled.
	A granularity of 1 will return a function exactly equal to the input Audio.
	Smaller granularities will generate more costly functions.
		)";
		}

	std::unique_ptr<NumberSliderModel> granularityModel;
	std::unique_ptr<FunctionGeneratorPlotModel> plotModel;
	};

