#pragma once

#include "AudioFeedbackProcessModel.hpp"

class NumberSliderModel;
class OnOffButtonModel;

struct AudioTextureModel : public AudioFeedbackProcessModel
	{
	AudioTextureModel();
	~AudioTextureModel() override;

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
		return R"(Texture is the main Audio timing function.
Events are generated following the provided Events Per Second and Scatter.
Each event places a copy of the input Audio at the event time.

Length - Number:
	Events will generate until this time.

Events Per Second - 1->1:
	The mean number of events per second.

Scatter 1->1:
	The standard deviation in events. Due to being in events, a higher
	Events Per Second will cause scatter to have less of an effect.
		)";
		}

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<NumberSliderModel> eventsPerSecondModel;
	std::unique_ptr<NumberSliderModel> scatterModel;
	std::unique_ptr<OnOffButtonModel> recursionModel;
	};
