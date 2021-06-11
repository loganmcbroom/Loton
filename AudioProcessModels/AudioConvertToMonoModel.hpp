#pragma once

#include "AudioSimpleTransformModel.hpp"

class NumberSliderModel;

struct AudioConvertToMonoModel : public AudioSimpleTransformModel
	{
	AudioConvertToMonoModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio ) const override;

	QString caption() const override;
	QString name() const override;
	};
