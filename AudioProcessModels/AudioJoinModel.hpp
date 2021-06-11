#pragma once

#include "AudioProcessModel.hpp"

class LotonComboBoxModel;
class NumberSliderModel;

struct AudioJoinModel : public AudioProcessModel
	{
	AudioJoinModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;

	virtual void inputConnectionCreated( PortIndex ) override;
	virtual void inputConnectionDeleted( PortIndex ) override;

	int numInPorts() const;
	int numInputs;
	};
