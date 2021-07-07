#pragma once

#include "AudioSingleSliderProcessModel.hpp"

class NumberSliderModel;

struct AudioSetVolumeModel : public AudioSingleSliderProcessModel
	{
	AudioSetVolumeModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio &, std::shared_ptr<Func1x1Data> ) override;

	QString caption() const override;
	QString name() const override;
	QString description() const override
		{
		return R"(This normalizes and then scales each sample by the factor.

Level - 1->1:
	The gain scale.
		)";
		}
	};
