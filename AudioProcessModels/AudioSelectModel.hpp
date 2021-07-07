#pragma once

#include "AudioProcessModel.hpp"

class LotonComboBoxModel;
class NumberSliderModel;

struct AudioStreamSelectModel : public AudioProcessModel
	{
	AudioStreamSelectModel();
	~AudioStreamSelectModel() override;

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	ControllerPairs makeInputControllers() override;
	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;
	std::vector<PortIndex> portsRequired() const override { return {}; }
	QString description() const override
		{
		return R"(At each point in time, selection decides which of the input Audio streams is playing.
Non-integer selections will mix appropriately scaled copies of the surrounding integer inputs.

Selection - 1->1:
	The audio stream selection. Zero-indexed.
		)";
		}

	virtual void inputConnectionCreated( PortIndex ) override;
	virtual void inputConnectionDeleted( PortIndex ) override;

	int numInPorts() const;
	int numInputs;

	std::unique_ptr<NumberSliderModel> selectionModel;
	};
