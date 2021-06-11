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

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<NumberSliderModel> grainsPerSecondModel;
	std::unique_ptr<NumberSliderModel> scatterModel;
	std::unique_ptr<NumberSliderModel> selectionModel;
	std::unique_ptr<NumberSliderModel> grainLengthModel;
	std::unique_ptr<NumberSliderModel> fadeLengthModel;
	};
