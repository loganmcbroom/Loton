#include "AudioConvertToStereoModel.hpp"

#include "NodeDataTypes/AudioData.hpp"

using namespace flan;

AudioConvertToStereoModel::AudioConvertToStereoModel()
	: AudioSimpleTransformModel()
	{
	}

std::function< std::shared_ptr<AudioData> () > AudioConvertToStereoModel::getMethod( flan::Audio in ) const
	{
	return [in, c = canceller]()
		{
		return std::make_shared<AudioData>( in.convertToStereo( *c ) );
		};
	}

QString AudioConvertToStereoModel::caption() const { return "Convert To Stereo"; }
QString AudioConvertToStereoModel::name() const { return "Audio Convert To Stereo"; }
