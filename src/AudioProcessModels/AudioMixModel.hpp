#pragma once

#include "AudioProcessModel.hpp"

class LotonComboBoxModel;
class NumberSliderModel;

struct AudioMixModel : public AudioProcessModel
	{
	AudioMixModel();
	~AudioMixModel() override;

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
		return R"(This mixes all the inputs.
		)";
		}

	virtual void inputConnectionCreated( PortIndex ) override;
	virtual void inputConnectionDeleted( PortIndex ) override;

	int numInPorts() const;
	void addBalance();

	std::unique_ptr<NumberSliderModel> interpModel;
	std::vector<std::unique_ptr<NumberSliderModel>> balanceModels;
	};
