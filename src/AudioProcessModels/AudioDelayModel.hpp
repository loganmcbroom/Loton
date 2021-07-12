#pragma once

#include "AudioFeedbackProcessModel.hpp"

class NumberSliderModel;
class OnOffButtonModel;

struct AudioDelayModel : public AudioFeedbackProcessModel
	{
	AudioDelayModel();
	~AudioDelayModel() override;

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
		return R"(Generates a volume-decaying iteration of the input. If a mod is supplied, it will be fed the previous iteration's ouput, rather than the original Audio.

Length - Number:
	The output length.

Delay Time - 1->1:
	 The amount of time between delays.

Decay - 1->1:
	Each delayed copy of the input will have its gain scaled by the decay at the event time. This is applied recursively, so a constant decay will fade out over time.
		)";
		}

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<NumberSliderModel> delayModel;
	std::unique_ptr<NumberSliderModel> decayAmtModel;
	std::unique_ptr<OnOffButtonModel> recursionModel;
	};

