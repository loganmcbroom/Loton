#pragma once

#include "../PVOCProcessModels/PVOCProcessModel.hpp"

class NumberSliderModel;
struct AudioToPVOCConverter;

struct AudioConvertToPVOCModel : public PVOCProcessModel
	{
	AudioConvertToPVOCModel();
	~AudioConvertToPVOCModel() override;

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(This is a phase vocoder, the main transform of Loton. It converts Audio data to PVOC data.
You can think of PVOC data as a 2D sound format, with time and frequency as the x and y axes.

Window Size - Number:
	This is the number of Audio Frames used for each FFT.

Hop - Number:
	This is the forward jump in the input Audio per PVOC frame.

FFT Size - Number:
	The size of the fft.
		)";
		}

	std::unique_ptr<NumberSliderModel> windowSizeModel;
	std::unique_ptr<NumberSliderModel> hopSizeModel;
	std::unique_ptr<NumberSliderModel> fftSizeModel;
	std::unique_ptr<AudioToPVOCConverter> converter;
	};

