#include "AudioPanModel.hpp"

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

AudioPanModel::AudioPanModel()
	: AudioSingleSliderProcessModel( -1, 1, 0 )
	{
	}

std::function< std::shared_ptr<AudioData> () > AudioPanModel::getMethod( flan::Audio & in, std::shared_ptr<Func1x1Data> factor )
	{
	return [in, factor, c = canceller]()
		{
		return std::make_shared<AudioData>( in.pan( factor->f, *c ) );
		};
	}

QString AudioPanModel::caption() const { return "Pan"; }
QString AudioPanModel::name() const { return "Audio Pan"; }



