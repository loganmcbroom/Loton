#pragma once

#include "AudioSingleSliderProcessModel.hpp"

class NumberSliderModel;

struct AudioWidenModel : public AudioSingleSliderProcessModel
	{
	AudioWidenModel();

	std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio &, std::shared_ptr<Func1x1Data> ) override;

	QString caption() const override;
	QString name() const override;
	QString description() const override
		{
		return R"(This redistributes energy between the mid and side signals.

Factor - 1->1:
	Values on ranging from -1 to 1 distribute energy towards the mid and side signals respectively.
		)";
		}
	};

