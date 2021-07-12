#pragma once

#include "PVOCProcessModel.hpp"

class InterpolatorModel;
class LotonComboBoxModel;

class PVOCModifyModel : public PVOCProcessModel
{
public:
	PVOCModifyModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	std::vector<PortIndex> portsRequired() const override { return { 0, 1 }; }
	QString description() const override
		{
		return R"(Each input point is mapped to an arbitrary output point by the Modifier.

Modifier - 2->2:
	Takes time/frequency points and returns the time/frequency those points should be moved to.
		)";
		}

	ControllerPairs makeInputControllers() override;

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	std::unique_ptr<InterpolatorModel> interpModel;
	std::unique_ptr<LotonComboBoxModel> deviceSelectorModel;
};
