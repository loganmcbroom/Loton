#pragma once

#include "AudioProcessModel.hpp"
#include "Widgets/Interpolator.hpp"

class NumberSliderModel;

struct AudioFadesModel : public AudioProcessModel
	{
	AudioFadesModel();
	~AudioFadesModel() override;

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
		return R"(Adds a fade to the ends of the input Audio.

Start - Number:
	Length of the start fade.

End - Number:
	Length of the end fade.

Interpolator - 1->1:
	The fading curve.
		)";
		}

	std::unique_ptr<NumberSliderModel> startLengthModel;
	std::unique_ptr<NumberSliderModel> endLengthModel;
	std::unique_ptr<InterpolatorModel> interpModel;
	};
