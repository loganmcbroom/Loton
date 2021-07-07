#pragma once

#include "AudioSimpleTransformModel.hpp"

class NumberSliderModel;

struct AudioConvertToLeftRightModel : public AudioSimpleTransformModel
	{
	AudioConvertToLeftRightModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio ) const override;

	QString caption() const override;
	QString name() const override;
	QString description() const override
		{
		return R"(Converts back to Left/Right from Mid/Side. See "Audio Convert To Mid/Side".)";
		}
	};

