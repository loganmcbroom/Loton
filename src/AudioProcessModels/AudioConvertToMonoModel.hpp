#pragma once

#include "AudioSimpleTransformModel.hpp"

class NumberSliderModel;

struct AudioConvertToMonoModel : public AudioSimpleTransformModel
	{
	AudioConvertToMonoModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio ) const override;

	QString caption() const override;
	QString name() const override;
	QString description() const override
		{
		return R"(This converts an Audio with any number of channels to a single channel Audio.
Each output sample is the sum of the channels at that time over the square root of the number of channels.)";
		}
	};
