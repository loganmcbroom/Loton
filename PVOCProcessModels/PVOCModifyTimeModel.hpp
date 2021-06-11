#pragma once

#include "PVOCProcessModel.hpp"

class InterpolatorModel;
class LotonComboBoxModel;

class PVOCModifyTimeModel : public PVOCProcessModel
{
public:
    PVOCModifyTimeModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;

	ControllerPairs makeInputControllers() override;

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	std::unique_ptr<InterpolatorModel> interpModel;
	std::unique_ptr<LotonComboBoxModel> deviceSelectorModel;
};


