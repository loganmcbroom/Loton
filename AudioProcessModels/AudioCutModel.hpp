#pragma once

#include "AudioProcessModel.hpp"

class NumberSliderModel;

struct AudioCutModel : public AudioProcessModel
	{
	AudioCutModel();
	~AudioCutModel() override;

	bool process() override;

	QString caption() const override final;
	QString name() const override final;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(This returns a peice of the original Audio that lied between Start and End.

Start - Number:
	The start time.

End - Number:
	The end time.
		)";
		}

	std::unique_ptr<NumberSliderModel> leftSliderModel;
	std::unique_ptr<NumberSliderModel> rightSliderModel;
	};

