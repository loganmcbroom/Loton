#pragma once

#include "../AudioProcessModels/AudioProcessModel.hpp"

struct PVOCToAudioConverter;
class StringDisplayModel;

struct PVOCConvertToAudioModel : public AudioProcessModel
	{
	PVOCConvertToAudioModel();
	~PVOCConvertToAudioModel() override;

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption( PortType, PortIndex ) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	void inputConnectionDeleted( PortIndex ) override;

	//std::unique_ptr<StringDisplayModel> windowSizeModel;
	//std::unique_ptr<StringDisplayModel> overlapsModel;
	std::unique_ptr<PVOCToAudioConverter> converter;
	};

