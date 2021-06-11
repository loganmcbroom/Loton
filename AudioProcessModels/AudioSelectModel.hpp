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

	virtual void inputConnectionCreated( PortIndex ) override;
	virtual void inputConnectionDeleted( PortIndex ) override;

	int numInPorts() const;
	int numInputs;

	std::unique_ptr<NumberSliderModel> selectionModel;
	};
