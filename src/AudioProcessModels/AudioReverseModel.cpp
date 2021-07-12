#include "AudioReverseModel.hpp"

#include "NodeDataTypes/AudioData.hpp"

using namespace flan;

AudioReverseModel::AudioReverseModel()
	: AudioSimpleTransformModel()
	{}

std::function< std::shared_ptr<AudioData> () > AudioReverseModel::getMethod( flan::Audio in ) const
	{
	return [in, c = canceller]()
		{
		return std::make_shared<AudioData>( in.reverse( *c ) );
		};
	}

QString AudioReverseModel::caption() const { return "Reverse"; }
QString AudioReverseModel::name() const { return "Audio Reverse"; }



