#pragma once

#include "AudioSimpleTransformModel.hpp"

class NumberSliderModel;

struct AudioInvertPhaseModel : public AudioSimpleTransformModel
	{
	AudioInvertPhaseModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio ) const override;

	QString caption() const override;
	QString name() const override;
	QString description() const override
		{
		return R"(Standard phase inversion. Every sample s is assigned -s.
		)";
		}
	};

