#pragma once

#include "AudioProcessModel.hpp"

class NumberSliderModel;

struct AudioRearrangeModel : public AudioProcessModel
	{
	AudioRearrangeModel();
	~AudioRearrangeModel() override;

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
		return R"(This performs chop, randomizes the order of the chopped pieces, and joins, crossfading.

Slice Length - Number:
	The length of each chopped piece.

Fade - Number:
	The crossfade time to apply when joining.
		)";
		}

	std::unique_ptr<NumberSliderModel> sliceLengthModel;
	std::unique_ptr<NumberSliderModel> fadeLengthModel;

	};
