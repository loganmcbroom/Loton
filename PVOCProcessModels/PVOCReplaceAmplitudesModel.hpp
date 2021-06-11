#pragma once

#include "PVOCProcessModel.hpp"

class NumberSliderModel;

// PVOC replaceAmplitudes( const PVOC & ampSource, Func2x1 amount = 1 ) const;

struct PVOCReplaceAmplitudesModel : public PVOCProcessModel
	{
	PVOCReplaceAmplitudesModel();

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;

	std::unique_ptr<NumberSliderModel> sliderModel;
	};
