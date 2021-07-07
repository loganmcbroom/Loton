#pragma once

#include "PVOCProcessModel.hpp"

class InterpolatorModel;
class LotonComboBoxModel;

class PVOCModifyFrequencyModel : public PVOCProcessModel
{
public:
	PVOCModifyFrequencyModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	std::vector<PortIndex> portsRequired() const override { return { 0, 1 }; }
	QString description() const override
		{
		return R"(Each input point is mapped to an arbitrary output frequency by the Modifier.

Modifier - 2->1:
	Takes time/frequency points and returns the frequency those points should be moved to at the given time.
		)";
		}

	ControllerPairs makeInputControllers() override;

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	std::unique_ptr<InterpolatorModel> interpModel;
	std::unique_ptr<LotonComboBoxModel> deviceSelectorModel;
};


