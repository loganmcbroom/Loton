#pragma once

#include "AudioProcessModel.hpp"

class NumberSliderModel;

struct AudioTextureSimpleModel : public AudioProcessModel
	{
	AudioTextureSimpleModel();
	~AudioTextureSimpleModel() override;

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<NumberSliderModel> eventsPerSecondModel;
	std::unique_ptr<NumberSliderModel> scatterModel;
	std::unique_ptr<NumberSliderModel> repitchModel;
	std::unique_ptr<NumberSliderModel> gainModel;
	std::unique_ptr<NumberSliderModel> eventLengthModel;
	std::unique_ptr<NumberSliderModel> panModel;
	};
