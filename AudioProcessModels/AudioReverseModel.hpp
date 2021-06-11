#pragma once

#include "AudioSimpleTransformModel.hpp"

class NumberSliderModel;

struct AudioReverseModel : public AudioSimpleTransformModel
	{
	AudioReverseModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio ) const override;

	QString caption() const override;
	QString name() const override;
	};
