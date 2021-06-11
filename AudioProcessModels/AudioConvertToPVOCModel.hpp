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

	std::unique_ptr<NumberSliderModel> windowSizeModel;
	std::unique_ptr<NumberSliderModel> hopSizeModel;
	std::unique_ptr<NumberSliderModel> fftSizeModel;
	std::unique_ptr<AudioToPVOCConverter> converter;
	};

