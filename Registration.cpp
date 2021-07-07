#include "Registration.hpp"

#include <memory>

#include <nodes/DataModelRegistry>

//Function Related Nodes
#include "FunctionModels/AudioConvertToFunctionModel.hpp"
#include "FunctionModels/DistributionExponentialModel.hpp"
#include "FunctionModels/DistributionNormalModel.hpp"
#include "FunctionModels/DistributionUniformRealModel.hpp"
#include "FunctionModels/DistributionUniformIntModel.hpp"
#include "FunctionModels/Func1x1ExponentialModel.hpp"
#include "FunctionModels/Func1x1PointInterpolationModel.hpp"
#include "FunctionModels/Func1x1PolynomialModel.hpp"
#include "FunctionModels/Func1x1SineModel.hpp"
#include "FunctionModels/Func2x1SurfaceGraphModel.hpp"
#include "FunctionModels/FunctionArithmeticNodeModel.hpp"
#include "FunctionModels/LuaFunc1x1Model.hpp"
#include "FunctionModels/LuaFunc2x1Model.hpp"
#include "FunctionModels/LuaFunc2x2Model.hpp"
#include "FunctionModels/NumberModel.hpp"

//Audio Transforms
#include "AudioProcessModels/AudioConvertToLeftRightModel.hpp"
#include "AudioProcessModels/AudioConvertToMidSideModel.hpp"
#include "AudioProcessModels/AudioConvertToMonoModel.hpp"
#include "AudioProcessModels/AudioConvertToPVOCModel.hpp"
#include "AudioProcessModels/AudioConvertToStereoModel.hpp"
#include "AudioProcessModels/AudioConvolveModel.hpp"
#include "AudioProcessModels/AudioCutModel.hpp"
#include "AudioProcessModels/AudioDelayModel.hpp"
#include "AudioProcessModels/AudioEnergyModel.hpp"
#include "AudioProcessModels/AudioFadesModel.hpp"
#include "AudioProcessModels/AudioGrainSelectModel.hpp"
#include "AudioProcessModels/AudioInvertPhaseModel.hpp"
#include "AudioProcessModels/AudioIterateModel.hpp"
#include "AudioProcessModels/AudioJoinModel.hpp"
#include "AudioProcessModels/AudioLengthModel.hpp"
#include "AudioProcessModels/AudioMaxModel.hpp"
#include "AudioProcessModels/AudioMixModel.hpp"
#include "AudioProcessModels/AudioModifyVolumeModel.hpp"
#include "AudioProcessModels/AudioPanModel.hpp"
#include "AudioProcessModels/AudioRearrangeModel.hpp"
#include "AudioProcessModels/AudioRepitchModel.hpp"
#include "AudioProcessModels/AudioReverseModel.hpp"
#include "AudioProcessModels/AudioSelectModel.hpp"
#include "AudioProcessModels/AudioSetVolumeModel.hpp"
#include "AudioProcessModels/AudioSourceDataModel.hpp"
#include "AudioProcessModels/AudioTextureModel.hpp"
#include "AudioProcessModels/AudioTextureSimpleModel.hpp"
#include "AudioProcessModels/AudioWaveshapeModel.hpp"
#include "AudioProcessModels/AudioWidenModel.hpp"

//PVOC Transforms
#include "PVOCProcessModels/PVOCAddOctavesModel.hpp"
#include "PVOCProcessModels/PVOCConvertToAudioModel.hpp"
#include "PVOCProcessModels/PVOCDesampleModel.hpp"
#include "PVOCProcessModels/PVOCGenerateModel.hpp"
#include "PVOCProcessModels/PVOCGetFrameModel.hpp"
#include "PVOCProcessModels/PVOCModifyFrequencyModel.hpp"
#include "PVOCProcessModels/PVOCModifyModel.hpp"
#include "PVOCProcessModels/PVOCModifyTimeModel.hpp"
#include "PVOCProcessModels/PVOCPrismModel.hpp"
#include "PVOCProcessModels/PVOCRepitchModel.hpp"
#include "PVOCProcessModels/PVOCReplaceAmplitudesModel.hpp"
#include "PVOCProcessModels/PVOCResonateModel.hpp"
#include "PVOCProcessModels/PVOCRemoveNLoudestPartialsModel.hpp"
#include "PVOCProcessModels/PVOCRetainNLoudestPartialsModel.hpp"
#include "PVOCProcessModels/PVOCSelectModel.hpp"
#include "PVOCProcessModels/PVOCSourceDataModel.hpp"
#include "PVOCProcessModels/PVOCStretchModel.hpp"
#include "PVOCProcessModels/PVOCSubtractAmplitudesModel.hpp"

//Converters
#include "Converters/SoundConverters.hpp"
#include "Converters/FunctionConverters.hpp"

//Data Types
#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

using namespace QtNodes;

std::shared_ptr<DataModelRegistry> registerDataModels()
	{
	auto ret = std::make_shared<DataModelRegistry>();

	QString arithmeticCategory = "Arithmetic";


	QString audioProcCategory = "Audio Transforms";
	ret->registerModel<AudioConvertToLeftRightModel>	( audioProcCategory );
	ret->registerModel<AudioConvertToMidSideModel>		( audioProcCategory );
	ret->registerModel<AudioConvertToMonoModel>			( audioProcCategory );
	ret->registerModel<AudioConvertToPVOCModel>			( audioProcCategory );
	ret->registerModel<AudioConvertToStereoModel>		( audioProcCategory );
	ret->registerModel<AudioConvolveModel>				( audioProcCategory );
	ret->registerModel<AudioCutModel>					( audioProcCategory );
	ret->registerModel<AudioDelayModel>					( audioProcCategory );
	ret->registerModel<AudioEnergyModel>				( audioProcCategory );
	ret->registerModel<AudioFadesModel>					( audioProcCategory );
	ret->registerModel<AudioGrainSelectModel>			( audioProcCategory );
	ret->registerModel<AudioInvertPhaseModel>			( audioProcCategory );
	ret->registerModel<AudioIterateModel>				( audioProcCategory );
	ret->registerModel<AudioJoinModel>					( audioProcCategory );
	ret->registerModel<AudioLengthModel>				( audioProcCategory );
	ret->registerModel<AudioMaxModel>					( audioProcCategory );
	ret->registerModel<AudioMixModel>					( audioProcCategory );
	ret->registerModel<AudioModifyVolumeModel>			( audioProcCategory );
	ret->registerModel<AudioPanModel>					( audioProcCategory );
	ret->registerModel<AudioRearrangeModel>				( audioProcCategory );
	ret->registerModel<AudioRepitchModel>				( audioProcCategory );
	ret->registerModel<AudioReverseModel>				( audioProcCategory );
	ret->registerModel<AudioStreamSelectModel>			( audioProcCategory );
	ret->registerModel<AudioSetVolumeModel>				( audioProcCategory );
	ret->registerModel<AudioSourceDataModel>			( audioProcCategory );
	ret->registerModel<AudioTextureModel>				( audioProcCategory );
	ret->registerModel<AudioTextureSimpleModel>			( audioProcCategory );
	ret->registerModel<AudioWaveshapeModel>				( audioProcCategory );
	ret->registerModel<AudioWidenModel>					( audioProcCategory );

	QString functionCategory = "Functional Nodes";
	ret->registerModel<AudioConvertToFunctionModel>		( functionCategory );
	ret->registerModel<Func1x1ExponentialModel>			( functionCategory );
	ret->registerModel<DistributionExponentialModel>	( functionCategory );
	ret->registerModel<DistributionNormalModel>			( functionCategory );
	ret->registerModel<DistributionUniformRealModel>	( functionCategory );
	ret->registerModel<DistributionUniformIntModel>		( functionCategory );
	ret->registerModel<Func1x1PointInterpolationModel>	( functionCategory );
	ret->registerModel<Func1x1PolynomialModel>			( functionCategory );
	ret->registerModel<Func1x1SineModel>				( functionCategory );
	ret->registerModel<Func2x1SurfaceGraphModel>		( functionCategory );
	ret->registerModel<LuaFunc1x1Model>					( functionCategory );
	ret->registerModel<LuaFunc2x1Model>					( functionCategory );
	ret->registerModel<LuaFunc2x2Model>					( functionCategory );
	ret->registerModel<NumberModel>						( functionCategory );

	ret->registerModel<FunctionAdditionNodeModel>		( functionCategory );
	ret->registerModel<FunctionSubtractionNodeModel>	( functionCategory );
	ret->registerModel<FunctionMultiplicationNodeModel>	( functionCategory );
	ret->registerModel<FunctionDivisionNodeModel>		( functionCategory );
	//ret->registerModel<PointsInPlaneTestModel>			( functionCategory );

	QString PVOCCategory = "PVOC Transforms";
	ret->registerModel<PVOCAddOctavesModel>				( PVOCCategory );
	ret->registerModel<PVOCConvertToAudioModel>			( PVOCCategory );
	ret->registerModel<PVOCDesampleModel>				( PVOCCategory );
	ret->registerModel<PVOCGenerateModel>				( PVOCCategory );
	ret->registerModel<PVOCGetFrameModel>				( PVOCCategory );
	ret->registerModel<PVOCModifyFrequencyModel>		( PVOCCategory );
	ret->registerModel<PVOCModifyModel>					( PVOCCategory );
	ret->registerModel<PVOCModifyTimeModel>				( PVOCCategory );
	ret->registerModel<PVOCPrismModel>					( PVOCCategory );
	ret->registerModel<PVOCRepitchModel>				( PVOCCategory );
	ret->registerModel<PVOCReplaceAmplitudesModel>		( PVOCCategory );
	ret->registerModel<PVOCResonateModel>				( PVOCCategory );
	ret->registerModel<PVOCRemoveNLoudestPartialsModel>	( PVOCCategory );
	ret->registerModel<PVOCRetainNLoudestPartialsModel>	( PVOCCategory );
	ret->registerModel<PVOCSelectModel>					( PVOCCategory );
	ret->registerModel<PVOCSourceDataModel>				( PVOCCategory );
	ret->registerModel<PVOCStretchModel>				( PVOCCategory );
	ret->registerModel<PVOCSubtractAmplitudesModel>		( PVOCCategory );

	//Converters
	#define Loton_REGISTER_CONVERTER_MACRO( T1, T2 ) \
	ret->registerTypeConverter( std::make_pair( T1 ## Data::Type(), T2 ## Data::Type() ), std::make_shared< T1 ## To ## T2 ## Converter >()	);

	Loton_REGISTER_CONVERTER_MACRO( Audio, PVOC )

	Loton_REGISTER_CONVERTER_MACRO( PVOC, Audio )

	Loton_REGISTER_CONVERTER_MACRO( Number, Func1x1 )
	Loton_REGISTER_CONVERTER_MACRO( Number, Func2x1 )
	Loton_REGISTER_CONVERTER_MACRO( Number, Func2x2 )

	Loton_REGISTER_CONVERTER_MACRO( Func1x1, Number )
	Loton_REGISTER_CONVERTER_MACRO( Func1x1, Func2x1 )
	Loton_REGISTER_CONVERTER_MACRO( Func1x1, Func2x2 )
	Loton_REGISTER_CONVERTER_MACRO( Func1x1, Interpolator )

	Loton_REGISTER_CONVERTER_MACRO( Func2x1, Number )
	Loton_REGISTER_CONVERTER_MACRO( Func2x1, Func1x1 )
	Loton_REGISTER_CONVERTER_MACRO( Func2x1, Func2x2 )

	return ret;
	}
