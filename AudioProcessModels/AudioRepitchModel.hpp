#pragma once

#include "AudioProcessModel.hpp"

class NumberSliderModel;
class LotonComboBoxModel;

struct AudioRepitchModel : public AudioProcessModel
	{
	AudioRepitchModel();
	~AudioRepitchModel() override;

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	ControllerPairs makeInputControllers() override;
	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;
	QString description() const override
		{
		return R"(Resampling based repitch.

Factor - 1->1:
	The pitch multiplier. Bounded below at 1 / 1000.

Granularity - Number:
	How often the factor is sampled. Small values allow faster moving Factors.

Quality - Number:
	0 - Sinc resampling (best quality)
	1 - Lerp and filter
	2 - Dirt quality, if you like dirt
		)";
		}

	std::unique_ptr<NumberSliderModel> factorModel;
	std::unique_ptr<NumberSliderModel> granularityModel;
	std::unique_ptr<LotonComboBoxModel> qualityModel;
	};
