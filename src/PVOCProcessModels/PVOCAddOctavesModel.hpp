#include "PVOCProcessModel.hpp"

class NumberSliderModel;
class InterpolatorModel;

class PVOCAddOctavesModel : public PVOCProcessModel
{
public:
	PVOCAddOctavesModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	ControllerPairs makeInputControllers() override;
	std::vector<PortIndex> portsRequired() const override { return { 0, 1 }; }
	QString description() const override
		{
		return R"(For each frequency, every octave above it is set to a copy of the base, scaled by Series Scale.

Series Scale - 2->1:
	The inputs to this are time and harmonic index starting at 0.
		)";
		}

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;
};
