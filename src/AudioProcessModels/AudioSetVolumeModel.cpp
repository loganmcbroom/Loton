#include "AudioSetVolumeModel.hpp"

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "Widgets/NumberSlider.hpp"

AudioSetVolumeModel::AudioSetVolumeModel()
	: AudioSingleSliderProcessModel( 0, NumberSlider::infinity, 1 )
	{
	}

std::function< std::shared_ptr<AudioData> () > AudioSetVolumeModel::getMethod( flan::Audio & in, std::shared_ptr<Func1x1Data> factor )
	{
	return [in, factor, c = canceller]()
		{
		return std::make_shared<AudioData>( in.setVolume( factor->f, *c ) );
		};
	}

QString AudioSetVolumeModel::caption() const { return "Set Volume"; }
QString AudioSetVolumeModel::name() const { return "Audio Set Volume"; }
