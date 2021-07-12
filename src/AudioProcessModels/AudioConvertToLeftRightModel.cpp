#include "AudioConvertToLeftRightModel.hpp"

#include "NodeDataTypes/AudioData.hpp"

using namespace flan;

AudioConvertToLeftRightModel::AudioConvertToLeftRightModel()
	: AudioSimpleTransformModel()
	{
	}

std::function< std::shared_ptr<AudioData> () > AudioConvertToLeftRightModel::getMethod( flan::Audio in ) const
	{
	return [in, cancel = canceller]()
		{
		return std::make_shared<AudioData>( in.convertToLeftRight( *cancel ) );
		};
	}

QString AudioConvertToLeftRightModel::caption() const { return "Convert To Left/Right"; }
QString AudioConvertToLeftRightModel::name() const { return "Audio Convert To Left/Right"; }
