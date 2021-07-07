#pragma once

#include "AudioFeedbackProcessModel.hpp"

class NumberSliderModel;

struct AudioGrainSelectModel : public AudioFeedbackProcessModel
	{
	AudioGrainSelectModel();
	~AudioGrainSelectModel() override;

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
		return R"(This is a traditional granular synthesis tool.

Length - Number:
	Grains will generate until this time.

Grains Per Second - 1->1:
	The mean number of grains per second.

Scatter - 1->1:
	The standard deviation in grains.
	Due to being in grains, a higher Grains Per Second will cause scatter to have less of an effect.

Selection - 1->1:
	The input time from which grains will be read.

Grain Length - 1->1:
	The length of grains.

Fade - 1->1:
	The start and end fade time for each grain. Fades uses a sqrt curve.
		)";
		}

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<NumberSliderModel> grainsPerSecondModel;
	std::unique_ptr<NumberSliderModel> scatterModel;
	std::unique_ptr<NumberSliderModel> selectionModel;
	std::unique_ptr<NumberSliderModel> grainLengthModel;
	std::unique_ptr<NumberSliderModel> fadeLengthModel;
	};
