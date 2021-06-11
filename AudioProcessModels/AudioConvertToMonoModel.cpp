#include "AudioConvertToMonoModel.hpp"

#include "NodeDataTypes/AudioData.hpp"

using namespace flan;

AudioConvertToMonoModel::AudioConvertToMonoModel()
	: AudioSimpleTransformModel()
	{
	}

std::function< std::shared_ptr<AudioData> () > AudioConvertToMonoModel::getMethod( flan::Audio in ) const
	{
	return [in, cancel = canceller]()
		{
		return std::make_shared<AudioData>( in.convertToMono( *cancel ) );
		};
	}


QString AudioConvertToMonoModel::caption() const { return "Convert To Mono"; }
QString AudioConvertToMonoModel::name() const { return "Audio Convert To Mono"; }



