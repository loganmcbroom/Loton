#pragma once

#include "AudioSingleSliderProcessModel.hpp"

class NumberSliderModel;

struct AudioSetVolumeModel : public AudioSingleSliderProcessModel
	{
	AudioSetVolumeModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio &, std::shared_ptr<Func1x1Data> ) override;

	QString caption() const override;
	QString name() const override;
	};
