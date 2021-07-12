#pragma once

#include "AudioSimpleTransformModel.hpp"

class NumberSliderModel;

struct AudioConvertToStereoModel : public AudioSimpleTransformModel
	{
	AudioConvertToStereoModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio ) const override;

	QString caption() const override;
	QString name() const override;
	QString description() const override
		{
		return R"(This converts the input to a two channel Audio. The conversion is channel-count dependant. The currently accepted channel counts are: 1 and 2.
		)";
		}
	};
