#pragma once

#include "AudioSingleSliderProcessModel.hpp"

class NumberSliderModel;

struct AudioWidenModel : public AudioSingleSliderProcessModel
	{
	AudioWidenModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio &, std::shared_ptr<Func1x1Data> ) override;

	QString caption() const override;
	QString name() const override;
	};

