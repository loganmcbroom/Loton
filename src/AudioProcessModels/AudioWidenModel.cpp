#include "AudioWidenModel.hpp"

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

AudioWidenModel::AudioWidenModel()
	: AudioSingleSliderProcessModel( -1, 1, 0 )
	{
	}

std::function< std::shared_ptr<AudioData> () > AudioWidenModel::getMethod( flan::Audio & in, std::shared_ptr<Func1x1Data> factor )
	{
	return [in, factor, c = canceller]()
		{
		return std::make_shared<AudioData>( in.widen( factor->f, *c ) );
		};
	}

QString AudioWidenModel::caption() const { return "Widen"; }
QString AudioWidenModel::name() const { return "Audio Widen"; }
