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

	std::unique_ptr<NumberSliderModel> startLengthModel;
	std::unique_ptr<NumberSliderModel> endLengthModel;
	std::unique_ptr<InterpolatorModel> interpModel;
	};
