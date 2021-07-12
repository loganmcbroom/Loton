#pragma once

#include "AudioProcessModel.hpp"

class NumberSliderModel;
class AudioData;
class Func1x1Data;
namespace flan { class Audio; };

struct AudioSingleSliderProcessModel : public AudioProcessModel
	{
	AudioSingleSliderProcessModel( double sliderLow, double sliderHigh, double sliderCenter );
	virtual ~AudioSingleSliderProcessModel() override;

	virtual std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio &, std::shared_ptr<Func1x1Data> ) = 0;

	bool process() override final;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }

	std::unique_ptr<NumberSliderModel> sliderModel;
	};
