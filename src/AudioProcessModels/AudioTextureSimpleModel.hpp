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
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(This is a utility for calling Audio Texture with the most common mod functions.

Length - Number:
	Events will generate until this time.

Events Per Second - 1->1:
	The mean number of events per second.

Scatter 1->1:
	The standard deviation in events. Due to being in events, a higher
	Events Per Second will cause scatter to have less of an effect.

Repitch - 1->1:
	The pitch scaling factor.

Gain - 1->1:
	The volume scaling factor.

Event Length - 1->1:
	The event length.

Pan - 1->1:
	Pan amount. See Audio Pan.
		)";
		}

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<NumberSliderModel> eventsPerSecondModel;
	std::unique_ptr<NumberSliderModel> scatterModel;
	std::unique_ptr<NumberSliderModel> repitchModel;
	std::unique_ptr<NumberSliderModel> gainModel;
	std::unique_ptr<NumberSliderModel> eventLengthModel;
	std::unique_ptr<NumberSliderModel> panModel;
	};
