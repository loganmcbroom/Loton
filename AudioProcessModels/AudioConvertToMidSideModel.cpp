#include "AudioConvertToMidSideModel.hpp"

#include "NodeDataTypes/AudioData.hpp"

using namespace flan;

AudioConvertToMidSideModel::AudioConvertToMidSideModel()
	: AudioSimpleTransformModel()
	{
	}

std::function< std::shared_ptr<AudioData> () > AudioConvertToMidSideModel::getMethod( flan::Audio in ) const
	{
	return [in, cancel = canceller]()
		{
		return std::make_shared<AudioData>( in.convertToMidSide( *cancel ) );
		};
	}

QString AudioConvertToMidSideModel::caption() const { return "Convert To Mid/Side"; }
QString AudioConvertToMidSideModel::name() const { return "Audio Convert To Mid/Side"; }
