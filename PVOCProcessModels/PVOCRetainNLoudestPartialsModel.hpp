#pragma once

#include "PVOCProcessModel.hpp"

class NumberSliderModel;

struct PVOCRetainNLoudestPartialsModel : public PVOCProcessModel
	{
	PVOCRetainNLoudestPartialsModel();

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(At any given time, numPartials should return a number of bins, N, to retain.
The N loudest bins are copied to the output.
All other output bins are 0 filled.

Num Partials - 1->1:
	The number of partials to retain as a function of time.
		)";
		}

	std::unique_ptr<NumberSliderModel> sliderModel;
	};
