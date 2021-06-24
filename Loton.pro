TEMPLATE = app
TARGET = Loton

QT = core gui multimedia concurrent datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

HEADERS += \
	AudioFeedbackProcessModel.hpp \
	AudioProcessModels/AudioConvertToFunctionModel.hpp \
	AudioProcessModels/AudioConvertToLeftRightModel.hpp \
	AudioProcessModels/AudioConvertToMidSideModel.hpp \
	AudioProcessModels/AudioConvertToMonoModel.hpp \
	AudioProcessModels/AudioConvertToPVOCModel.hpp \
	AudioProcessModels/AudioConvertToStereoModel.hpp \
	AudioProcessModels/AudioConvolveModel.hpp \
	AudioProcessModels/AudioCutModel.hpp \
	AudioProcessModels/AudioDelayModel.hpp \
	AudioProcessModels/AudioEnergyModel.hpp \
	AudioProcessModels/AudioFadesModel.hpp \
	AudioProcessModels/AudioFeedbackProcessModel.hpp \
	AudioProcessModels/AudioGrainSelectModel.hpp \
	AudioProcessModels/AudioInfoProcessModel.hpp \
	AudioProcessModels/AudioInvertPhaseModel.hpp \
	AudioProcessModels/AudioIterateModel.hpp \
	AudioProcessModels/AudioJoinModel.hpp \
	AudioProcessModels/AudioLengthModel.hpp \
	AudioProcessModels/AudioMixModel.hpp \
	AudioProcessModels/AudioModifyVolumeModel.hpp \
	AudioProcessModels/AudioPanModel.hpp \
	AudioProcessModels/AudioPrismModel.hpp \
	AudioProcessModels/AudioRearrangeModel.hpp \
	AudioProcessModels/AudioSelectModel.hpp \
	AudioProcessModels/AudioSetVolumeModel.hpp \
	AudioProcessModels/AudioSimpleTransformModel.hpp \
	AudioProcessModels/AudioSingleSliderProcessModel.hpp \
	AudioProcessModels/AudioTextureModel.hpp \
	AudioProcessModels/AudioTextureSimpleModel.hpp \
	AudioProcessModels/AudioWaveshapeModel.hpp \
	AudioProcessModels/AudioWidenModel.hpp \
	Converters/FunctionConverters.hpp \
	Converters/SoundConverters.hpp \
	FeedbackNodeHandlers/AudioFeedbackWorker.hpp \
	FlanProcessModel.hpp \
	FunctionModels/DistributionExponentialModel.hpp \
	FunctionModels/DistributionModel.hpp \
	FunctionModels/DistributionNormalModel.hpp \
	FunctionModels/DistributionUniformIntModel.hpp \
	FunctionModels/DistributionUniformRealModel.hpp \
	FunctionModels/Func1x1ExponentialModel.hpp \
	FunctionModels/Func1x1PointInterpolationModel.hpp \
	FunctionModels/Func1x1PolynomialModel.hpp \
	FunctionModels/Func1x1SineModel.hpp \
	FunctionModels/Func2x1SurfaceGraphModel.hpp \
	FunctionModels/FunctionArithmeticNodeModel.hpp \
	FunctionModels/FunctionGeneratorNodeModel.hpp \
	FunctionModels/FunctionNodeModel.hpp \
	FunctionModels/LuaFunc1x1Model.hpp \
	FunctionModels/LuaFunc2x1Model.hpp \
	FunctionModels/LuaFunc2x2Model.hpp \
	FunctionModels/LuaHandlers/LuaFunction.hpp \
	FunctionModels/LuaHandlers/LuaPrismHandler.hpp \
	FunctionModels/LuaHandlers/LuaUtil.hpp \
	FunctionModels/NumberModel.hpp \
	FunctionModels/PointsInPlaneTestModel.hpp \
	LotonController.hpp \
	LotonFlowScene.hpp \
	LotonNodeModel.hpp \
	MainWindow.hpp \
	NodeControlPanel.hpp \
	NodeDataTypes/FunctionData.hpp \
	NodeDataTypes/InterpolatorData.hpp \
	NodeDataTypes/LotonNodeData.hpp \
	NodeDataTypes/NumberData.hpp \
	NodeDataTypes/WipeNodeData.hpp \
	PVOCProcessModels/PVOCAddOctavesModel.hpp \
	PVOCProcessModels/PVOCConvertToAudioModel.hpp \
	PVOCProcessModels/PVOCDesampleModel.hpp \
	PVOCProcessModels/PVOCGenerateModel.hpp \
	PVOCProcessModels/PVOCGetFrameModel.hpp \
	PVOCProcessModels/PVOCModifyFrequencyModel.hpp \
	PVOCProcessModels/PVOCModifyModel.hpp \
	PVOCProcessModels/PVOCPrismModel.hpp \
	PVOCProcessModels/PVOCRemoveNLoudestPartialsModel.hpp \
	PVOCProcessModels/PVOCRepitchModel.hpp \
	PVOCProcessModels/PVOCReplaceAmplitudesModel.hpp \
	PVOCProcessModels/PVOCResonateModel.hpp \
	PVOCProcessModels/PVOCRetainNLoudestPartialsModel.hpp \
	PVOCProcessModels/PVOCSelectModel.hpp \
	PVOCProcessModels/PVOCSourceDataModel.hpp \
	PVOCProcessModels/PVOCStretchModel.hpp \
	PVOCProcessModels/PVOCSubtractAmplitudesModel.hpp \
	Settings.hpp \
	Widgets/AudioPlayer.hpp \
	Widgets/ComboBox.hpp \
	Widgets/ControllableLogPlot.hpp \
	Widgets/ControllablePlot.hpp \
	Widgets/FunctionGeneratorPlot.hpp \
	Widgets/FunctionGeneratorPlot2x1.hpp \
	Widgets/FunctionGeneratorPlot2x2.hpp \
	Widgets/Interpolator.hpp \
	Widgets/LED.hpp \
	NodeDataTypes/AudioData.hpp \
	AudioProcessModels/AudioRepitchModel.hpp \
	AudioProcessModels/AudioReverseModel.hpp \
	AudioProcessModels/AudioProcessModel.hpp \
	AudioProcessModels/AudioSourceDataModel.hpp \
	CentralWidget.hpp \
    NodeDataTypes/AudioData.hpp \
    NodeDataTypes/PVOCData.hpp \
	NodeDataTypes/PVOCData.hpp \
	PVOCProcessModels/PVOCModifyTimeModel.hpp \
	PVOCProcessModels/PVOCProcessModel.hpp \
    Registration.hpp \
	Widgets/LuaSyntaxHighlighter.hpp \
	Widgets/OnOffButton.hpp \
	Widgets/PVOCPlayer.hpp \
	Widgets/PointsInPlane/Point2x1Graph.hpp \
	Widgets/PointsInPlane/PointInPlane.hpp \
	Widgets/PointsInPlane/PointInterpolator.hpp \
	Widgets/PointsInPlane/PointsInPlaneModel.hpp \
	Widgets/PointsInPlane/PointsInPlaneView.hpp \
	Widgets/ProcessExplorer.hpp \
	Widgets/RoundedDialog.hpp \
    TypeInfo.hpp \
	Widgets/SampleExplorer.hpp \
	Widgets/SettingsMenu.hpp \
	Widgets/SpectrographIcon.hpp \
	Widgets/StringDisplay.hpp \
	Widgets/SurfaceGraph.hpp \
	Widgets/TextEditor.hpp \
	Widgets/WaveformIcon.hpp \
	Widgets/NumberSlider.hpp \
	pvocProcessModel.hpp

SOURCES += \
	AudioProcessModels/AudioConvertToFunctionModel.cpp \
	AudioProcessModels/AudioConvertToLeftRightModel.cpp \
	AudioProcessModels/AudioConvertToMidSideModel.cpp \
	AudioProcessModels/AudioConvertToMonoModel.cpp \
	AudioProcessModels/AudioConvertToPVOCModel.cpp \
	AudioProcessModels/AudioConvertToStereoModel.cpp \
	AudioProcessModels/AudioConvolveModel.cpp \
	AudioProcessModels/AudioCutModel.cpp \
	AudioProcessModels/AudioDelayModel.cpp \
	AudioProcessModels/AudioEnergyModel.cpp \
	AudioProcessModels/AudioFadesModel.cpp \
	AudioProcessModels/AudioFeedbackProcessModel.cpp \
	AudioProcessModels/AudioGrainSelectModel.cpp \
	AudioProcessModels/AudioInfoProcessModel.cpp \
	AudioProcessModels/AudioInvertPhaseModel.cpp \
	AudioProcessModels/AudioIterateModel.cpp \
	AudioProcessModels/AudioJoinModel.cpp \
	AudioProcessModels/AudioLengthModel.cpp \
	AudioProcessModels/AudioMixModel.cpp \
	AudioProcessModels/AudioModifyVolumeModel.cpp \
	AudioProcessModels/AudioPanModel.cpp \
	AudioProcessModels/AudioRearrangeModel.cpp \
	AudioProcessModels/AudioRepitchModel.cpp \
	AudioProcessModels/AudioReverseModel.cpp \
	AudioProcessModels/AudioProcessModel.cpp \
	AudioProcessModels/AudioSelectModel.cpp \
	AudioProcessModels/AudioSetVolumeModel.cpp \
	AudioProcessModels/AudioSimpleTransformModel.cpp \
	AudioProcessModels/AudioSingleSliderProcessModel.cpp \
	AudioProcessModels/AudioSourceDataModel.cpp \
	AudioProcessModels/AudioTextureModel.cpp \
	AudioProcessModels/AudioTextureSimpleModel.cpp \
	AudioProcessModels/AudioWaveshapeModel.cpp \
	AudioProcessModels/AudioWidenModel.cpp \
	CentralWidget.cpp \
	Converters/FunctionConverters.cpp \
	Converters/SoundConverters.cpp \
	FeedbackNodeHandlers/AudioFeedbackWorker.cpp \
	FlanProcessModel.cpp \
	FunctionModels/DistributionExponentialModel.cpp \
	FunctionModels/DistributionModel.cpp \
	FunctionModels/DistributionNormalModel.cpp \
	FunctionModels/DistributionUniformIntModel.cpp \
	FunctionModels/DistributionUniformRealModel.cpp \
	FunctionModels/Func1x1ExponentialModel.cpp \
	FunctionModels/Func1x1PointInterpolationModel.cpp \
	FunctionModels/Func1x1PolynomialModel.cpp \
	FunctionModels/Func1x1SineModel.cpp \
	FunctionModels/Func2x1SurfaceGraphModel.cpp \
	FunctionModels/FunctionArithmeticNodeModel.cpp \
	FunctionModels/FunctionGeneratorNodeModel.cpp \
	FunctionModels/FunctionNodeModel.cpp \
	FunctionModels/LuaFunc1x1Model.cpp \
	FunctionModels/LuaFunc2x1Model.cpp \
	FunctionModels/LuaFunc2x2Model.cpp \
	FunctionModels/LuaHandlers/LuaFunction.cpp \
	FunctionModels/LuaHandlers/LuaPrismHandler.cpp \
	FunctionModels/LuaHandlers/LuaUtil.cpp \
	FunctionModels/NumberModel.cpp \
	FunctionModels/PointsInPlaneTestModel.cpp \
	LotonController.cpp \
	LotonFlowScene.cpp \
	LotonNodeModel.cpp \
	MainWindow.cpp \
	NodeControlPanel.cpp \
	NodeDataTypes/LotonNodeData.cpp \
	NodeDataTypes/PVOCData.cpp \
	PVOCProcessModels/PVOCAddOctavesModel.cpp \
	PVOCProcessModels/PVOCConvertToAudioModel.cpp \
	PVOCProcessModels/PVOCDesampleModel.cpp \
	PVOCProcessModels/PVOCGenerateModel.cpp \
	PVOCProcessModels/PVOCGetFrameModel.cpp \
	PVOCProcessModels/PVOCModifyFrequencyModel.cpp \
	PVOCProcessModels/PVOCModifyModel.cpp \
	PVOCProcessModels/PVOCModifyTimeModel.cpp \
	PVOCProcessModels/PVOCPrismModel.cpp \
	PVOCProcessModels/PVOCProcessModel.cpp \
	PVOCProcessModels/PVOCRemoveNLoudestPartialsModel.cpp \
	PVOCProcessModels/PVOCRepitchModel.cpp \
	PVOCProcessModels/PVOCReplaceAmplitudesModel.cpp \
	PVOCProcessModels/PVOCResonateModel.cpp \
	PVOCProcessModels/PVOCRetainNLoudestPartialsModel.cpp \
	PVOCProcessModels/PVOCSelectModel.cpp \
	PVOCProcessModels/PVOCSourceDataModel.cpp \
	PVOCProcessModels/PVOCStretchModel.cpp \
	PVOCProcessModels/PVOCSubtractAmplitudesModel.cpp \
    Registration.cpp \
	Settings.cpp \
	Widgets/AudioPlayer.cpp \
	Widgets/ComboBox.cpp \
	Widgets/ControllableLogPlot.cpp \
	Widgets/ControllablePlot.cpp \
	Widgets/FunctionGeneratorPlot.cpp \
	Widgets/FunctionGeneratorPlot2x1.cpp \
	Widgets/FunctionGeneratorPlot2x2.cpp \
	Widgets/Interpolator.cpp \
	Widgets/LuaSyntaxHighlighter.cpp \
	Widgets/OnOffButton.cpp \
	Widgets/PVOCPlayer.cpp \
	Widgets/PointsInPlane/Point2x1Graph.cpp \
	Widgets/PointsInPlane/PointInPlane.cpp \
	Widgets/PointsInPlane/PointInterpolator.cpp \
	Widgets/PointsInPlane/PointsInPlaneModel.cpp \
	Widgets/PointsInPlane/PointsInPlaneView.cpp \
	Widgets/ProcessExplorer.cpp \
	Widgets/RoundedDialog.cpp \
	Widgets/SampleExplorer.cpp \
	Widgets/SettingsMenu.cpp \
	Widgets/SpectrographIcon.cpp \
	Widgets/StringDisplay.cpp \
	Widgets/SurfaceGraph.cpp \
	Widgets/TextEditor.cpp \
	Widgets/WaveformIcon.cpp \
	Widgets/NumberSlider.cpp \
    main.cpp

RESOURCES += \
	Theme/breeze.qrc \
	Media/close-icon-w.png


#nodeeditor
DEFINES += NODE_EDITOR_STATIC
	 win32:CONFIG(release, debug|release): LIBS += -LC:/libs/build-nodeeditor-fork-Desktop_x64_windows_msvc2019_pe_64bit-Release/lib/ -lnodes
else:win32:CONFIG(debug,   debug|release): LIBS += -LC:/libs/build-nodeeditor-fork-Desktop_x64_windows_msvc2019_pe_64bit-Debug/lib/ -lnodes
INCLUDEPATH += C:/libs/nodeeditor-master/include
DEPENDPATH += C:/libs/nodeeditor-master/include


#libsndfile
win32: LIBS += -LC:/libs/libsndfile/lib/ -lsndfile
INCLUDEPATH += C:/libs/libsndfile/include
DEPENDPATH += C:/libs/libsndfile/include
win32:!win32-g++: PRE_TARGETDEPS += C:/libs/libsndfile/lib/sndfile.lib
else:win32-g++: PRE_TARGETDEPS += C:/libs/libsndfile/lib/libsndfile.a


#fftw
win32: LIBS += -LC:/libs/fftw/ -llibfftw3f-3
INCLUDEPATH += C:/libs/fftw
DEPENDPATH += C:/libs/fftw
win32:!win32-g++: PRE_TARGETDEPS += C:/libs/fftw/libfftw3f-3.lib
else:win32-g++: PRE_TARGETDEPS += C:/libs/fftw/liblibfftw3f-3.a


#OpenCL
win32: LIBS += -L'C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.2/lib/x64/' -lOpenCL
win32:!win32-g++: PRE_TARGETDEPS += 'C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.2/lib/x64/OpenCL.lib'
else:win32-g++: PRE_TARGETDEPS += 'C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.2/lib/x64/libOpenCL.a'
INCLUDEPATH += 'C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.2/include/CL'
DEPENDPATH += 'C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.2/include/CL'


#flan
Release:LIBS += -LC:/Users/Logan/source/repos/flan/out/build/x64-Release/libflan/ -llibflan
Release:win32:!win32-g++: PRE_TARGETDEPS += C:/Users/Logan/source/repos/flan/out/build/x64-Release/libflan/libflan.lib
else:Release:win32-g++: PRE_TARGETDEPS += C:/Users/Logan/source/repos/flan/out/build/x64-Release/libflan/liblibflan.a

Debug:LIBS += -LC:/Users/Logan/source/repos/flan/out/build/x64-Debug/libflan/ -llibflan
Debug:win32:!win32-g++: PRE_TARGETDEPS += C:/Users/Logan/source/repos/flan/out/build/x64-Debug/libflan/libflan.lib
else:Debug:win32-g++: PRE_TARGETDEPS += C:/Users/Logan/source/repos/flan/out/build/x64-Debug/libflan/liblibflan.a

INCLUDEPATH += C:/Users/Logan/source/repos/flan/libflan/include
DEPENDPATH += C:/Users/Logan/source/repos/flan/libflan/include


#qwt
DEFINES += QWT_DLL
	 win32:CONFIG(release, debug|release): LIBS += -LC:/libs/build-qwt-Desktop_Qt_5_15_0_MSVC2019_64bit-Release/lib/ -lqwt
else:win32:CONFIG(debug,   debug|release): LIBS += -LC:/libs/build-qwt-Desktop_Qt_5_15_0_MSVC2019_64bit-Debug/lib/ -lqwtd
INCLUDEPATH += C:/libs/qwt-6.1.4/src
DEPENDPATH += C:/libs/qwt-6.1.4/src


#lua
win32: LIBS += -LC:/Users/Logan/source/repos/luajit/src/ -llibluajit-5.1.dll
INCLUDEPATH += C:/Users/Logan/source/repos/luajit/src
DEPENDPATH += C:/Users/Logan/source/repos/luajit/src
