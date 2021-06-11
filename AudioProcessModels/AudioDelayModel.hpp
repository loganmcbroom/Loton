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

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<NumberSliderModel> delayModel;
	std::unique_ptr<NumberSliderModel> decayAmtModel;
	std::unique_ptr<OnOffButtonModel> recursionModel;
	};

