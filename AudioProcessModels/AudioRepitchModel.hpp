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

	ControllerPairs makeInputControllers() override;

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	std::unique_ptr<NumberSliderModel> factorModel;
	std::unique_ptr<NumberSliderModel> granularityModel;
	std::unique_ptr<LotonComboBoxModel> qualityModel;
	};
