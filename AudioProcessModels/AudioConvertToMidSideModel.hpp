#pragma once

#include "AudioSimpleTransformModel.hpp"

class NumberSliderModel;

struct AudioConvertToMidSideModel : public AudioSimpleTransformModel
	{
	AudioConvertToMidSideModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio ) const override;

	QString caption() const override;
	QString name() const override;
	QString description() const override
		{
		return R"(Audio normally stores "Left" and "Right" data in channel 0 and 1 respectively.
The result of this transform stores "Mid" and "Side" data in those channels instead.
This allows mid and side information to be processed independantly. See "Audio Convert To Left/Right" for the inverse.)";
		}
	};
