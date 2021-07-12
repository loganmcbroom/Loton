#pragma once

#include "AudioProcessModel.hpp"

class NumberSliderModel;

struct AudioModifyVolumeModel : public AudioProcessModel
	{
	AudioModifyVolumeModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(This multiplies each sample by the given factor at each point in time.

Gain - 1->1:
	The gain multiplier.
		)";
		}

	ControllerPairs makeInputControllers() override;

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	std::unique_ptr<NumberSliderModel> sliderModel;
	};
