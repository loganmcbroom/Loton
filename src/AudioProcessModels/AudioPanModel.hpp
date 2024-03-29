#pragma once

#include "AudioSingleSliderProcessModel.hpp"

class NumberSliderModel;

struct AudioPanModel : public AudioSingleSliderProcessModel
	{
	AudioPanModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio &, std::shared_ptr<Func1x1Data> ) override;

	QString caption() const override;
	QString name() const override;
	QString description() const override
		{
		return R"(This shifts the spatial position of the input.

Pan Amount - 1->1:
	Zero applies no pan, -1 and 1 pan hard left and right respectively.
		)";
		}
	};
