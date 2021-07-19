cmake_minimum_required( VERSION 3.15 )

project( Loton VERSION 1.7 LANGUAGES CXX )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

# Create execulable
add_executable( Loton
	src/main.cpp
    src/MainWindow.cpp
	src/FlanProcessModel.cpp 
	src/Registration.cpp 
	src/LotonController.cpp 
	src/LotonFlowScene.cpp 
	src/LotonNodeModel.cpp 
	src/NodeControlPanel.cpp 
	src/Settings.cpp 
	src/CentralWidget.cpp

	src/NodeDataTypes/LotonNodeData.cpp 
    
    src/AudioProcessModels/AudioFeedbackProcessModel.cpp
	src/AudioProcessModels/AudioConvertToLeftRightModel.cpp
	src/AudioProcessModels/AudioConvertToMidSideModel.cpp
	src/AudioProcessModels/AudioConvertToMonoModel.cpp 
	src/AudioProcessModels/AudioConvertToPVOCModel.cpp 
	src/AudioProcessModels/AudioConvertToStereoModel.cpp 
	src/AudioProcessModels/AudioConvolveModel.cpp 
	src/AudioProcessModels/AudioCutModel.cpp 
	src/AudioProcessModels/AudioDelayModel.cpp 
	src/AudioProcessModels/AudioEnergyModel.cpp 
	src/AudioProcessModels/AudioFadesModel.cpp 
	src/AudioProcessModels/AudioFeedbackProcessModel.cpp 
	src/AudioProcessModels/AudioGrainSelectModel.cpp 
	src/AudioProcessModels/AudioInfoProcessModel.cpp 
	src/AudioProcessModels/AudioInvertPhaseModel.cpp 
	src/AudioProcessModels/AudioIterateModel.cpp 
	src/AudioProcessModels/AudioJoinModel.cpp 
	src/AudioProcessModels/AudioLengthModel.cpp 
	src/AudioProcessModels/AudioMaxModel.cpp 
	src/AudioProcessModels/AudioMixModel.cpp 
	src/AudioProcessModels/AudioModifyVolumeModel.cpp 
	src/AudioProcessModels/AudioPanModel.cpp
	src/AudioProcessModels/AudioRearrangeModel.cpp 
	src/AudioProcessModels/AudioSelectModel.cpp 
	src/AudioProcessModels/AudioSetVolumeModel.cpp 
	src/AudioProcessModels/AudioSimpleTransformModel.cpp 
	src/AudioProcessModels/AudioSingleSliderProcessModel.cpp 
	src/AudioProcessModels/AudioTextureModel.cpp 
	src/AudioProcessModels/AudioTextureSimpleModel.cpp 
	src/AudioProcessModels/AudioWaveshapeModel.cpp 
	src/AudioProcessModels/AudioWidenModel.cpp 
	src/AudioProcessModels/AudioRepitchModel.cpp 
	src/AudioProcessModels/AudioReverseModel.cpp 
	src/AudioProcessModels/AudioProcessModel.cpp 
	src/AudioProcessModels/AudioSourceDataModel.cpp 

	src/PVOCProcessModels/PVOCAddOctavesModel.cpp 
	src/PVOCProcessModels/PVOCConvertToAudioModel.cpp 
	src/PVOCProcessModels/PVOCDesampleModel.cpp 
	src/PVOCProcessModels/PVOCGenerateModel.cpp 
	src/PVOCProcessModels/PVOCGetFrameModel.cpp 
	src/PVOCProcessModels/PVOCModifyFrequencyModel.cpp 
	src/PVOCProcessModels/PVOCModifyModel.cpp 
	src/PVOCProcessModels/PVOCPrismModel.cpp 
	src/PVOCProcessModels/PVOCRemoveNLoudestPartialsModel.cpp 
	src/PVOCProcessModels/PVOCRepitchModel.cpp 
	src/PVOCProcessModels/PVOCReplaceAmplitudesModel.cpp 
	src/PVOCProcessModels/PVOCResonateModel.cpp 
	src/PVOCProcessModels/PVOCRetainNLoudestPartialsModel.cpp 
	src/PVOCProcessModels/PVOCSelectModel.cpp 
	src/PVOCProcessModels/PVOCSourceDataModel.cpp 
	src/PVOCProcessModels/PVOCStretchModel.cpp 
	src/PVOCProcessModels/PVOCSubtractAmplitudesModel.cpp 
	src/PVOCProcessModels/PVOCModifyTimeModel.cpp 
	src/PVOCProcessModels/PVOCProcessModel.cpp 

	src/FunctionModels/AudioConvertToFunctionModel.cpp 
	src/FunctionModels/DistributionExponentialModel.cpp 
	src/FunctionModels/DistributionModel.cpp 
	src/FunctionModels/DistributionNormalModel.cpp 
	src/FunctionModels/DistributionUniformIntModel.cpp 
	src/FunctionModels/DistributionUniformRealModel.cpp 
	src/FunctionModels/Func1x1ExponentialModel.cpp 
	src/FunctionModels/Func1x1PointInterpolationModel.cpp 
	src/FunctionModels/Func1x1PolynomialModel.cpp 
	src/FunctionModels/Func1x1SineModel.cpp 
	src/FunctionModels/Func2x1SurfaceGraphModel.cpp 
	src/FunctionModels/FunctionArithmeticNodeModel.cpp 
	src/FunctionModels/FunctionGeneratorNodeModel.cpp 
	src/FunctionModels/FunctionNodeModel.cpp 
	src/FunctionModels/LuaFunc1x1Model.cpp 
	src/FunctionModels/LuaFunc2x1Model.cpp 
	src/FunctionModels/LuaFunc2x2Model.cpp 
	src/FunctionModels/LuaHandlers/LuaFunction.cpp 
	src/FunctionModels/LuaHandlers/LuaPrismHandler.cpp 
	src/FunctionModels/LuaHandlers/LuaUtil.cpp 
	src/FunctionModels/NumberModel.cpp 
	src/FunctionModels/PointsInPlaneTestModel.cpp 

	src/Converters/FunctionConverters.cpp 
	src/Converters/SoundConverters.cpp 

	src/FeedbackNodeHandlers/AudioFeedbackWorker.cpp 
	
	src/Widgets/AudioPlayer.cpp 
	src/Widgets/ComboBox.cpp 
	src/Widgets/ControllableLogPlot.cpp 
	src/Widgets/ControllablePlot.cpp 
	src/Widgets/FunctionGeneratorPlot.cpp 
	src/Widgets/FunctionGeneratorPlot2x1.cpp 
	src/Widgets/FunctionGeneratorPlot2x2.cpp 
	src/Widgets/Interpolator.cpp 
	src/Widgets/LED.cpp 
	src/Widgets/LotonHelpWidget.cpp 
	src/Widgets/LuaSyntaxHighlighter.cpp 
	src/Widgets/OnOffButton.cpp 
	src/Widgets/PVOCPlayer.cpp 
	src/Widgets/PointsInPlane/Point2x1Graph.cpp 
	src/Widgets/PointsInPlane/PointInPlane.cpp 
	src/Widgets/PointsInPlane/PointInterpolator.cpp 
	src/Widgets/PointsInPlane/PointsInPlaneModel.cpp 
	src/Widgets/PointsInPlane/PointsInPlaneView.cpp 
	src/Widgets/ProcessExplorer.cpp 
	src/Widgets/RoundedDialog.cpp 
	src/Widgets/SampleExplorer.cpp 
	src/Widgets/SettingsMenu.cpp 
	src/Widgets/SpectrographIcon.cpp 
	src/Widgets/StringDisplay.cpp 
	src/Widgets/SurfaceGraph.cpp 
	src/Widgets/TextEditor.cpp 
	src/Widgets/WaveformIcon.cpp 
	src/Widgets/NumberSlider.cpp 

	resources/Loton.qrc
    resources/Theme/breeze.qrc
    )

target_compile_features( Loton PRIVATE cxx_std_17 )
set_target_properties( Loton PROPERTIES CXX_EXTENSIONS OFF )
target_include_directories( Loton PRIVATE src )


#==================================================================================================
# Link Libraries
#==================================================================================================

# Qt components
find_package( Qt5 5.3 COMPONENTS
    Core
    Gui
    Multimedia
    Concurrent
    DataVisualization
    Widgets
    ) 

target_link_libraries( Loton PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Multimedia
    Qt5::Concurrent
    Qt5::DataVisualization
    Qt5::Widgets
    )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" )

# Flan
find_package( Flan REQUIRED ) # Flan::Flan
target_link_libraries( Loton PRIVATE Flan::Flan )

# NodeEditor
find_package( NodeEditor REQUIRED ) # NodeEditor::NodeEditor
target_link_libraries( Loton PRIVATE NodeEditor::NodeEditor )

# Qwt
add_compile_definitions( QWT_DLL )
find_package( Qwt REQUIRED ) # Qwt::Qwt
target_link_libraries( Loton PRIVATE Qwt::Qwt )

# LuaJit
find_package( LuaJit REQUIRED ) # LuaJit::LuaJit
target_link_libraries( Loton PRIVATE LuaJit::LuaJit )


#==================================================================================================
# Installation
#==================================================================================================

install( TARGETS Loton DESTINATION bin )