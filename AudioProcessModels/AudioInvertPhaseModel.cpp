#include "AudioInvertPhaseModel.hpp"

#include "NodeDataTypes/AudioData.hpp"

using namespace flan;

AudioInvertPhaseModel::AudioInvertPhaseModel()
	: AudioSimpleTransformModel()
	{
	}

std::function< std::shared_ptr<AudioData> () > AudioInvertPhaseModel::getMethod( flan::Audio in ) const
	{
	return [in, c = canceller]()
		{
		return std::make_shared<AudioData>( in.invertPhase( *c ) );
		};
	}

QString AudioInvertPhaseModel::caption() const { return "Invert Phase"; }
QString AudioInvertPhaseModel::name() const { return "Audio Invert Phase"; }
