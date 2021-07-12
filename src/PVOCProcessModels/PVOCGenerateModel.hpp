#include "PVOCProcessModel.hpp"

class NumberSliderModel;
class InterpolatorModel;

class PVOCGenerateModel : public PVOCProcessModel
{
public:
	PVOCGenerateModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	ControllerPairs makeInputControllers() override;
	std::vector<PortIndex> portsRequired() const override { return { 2 }; }
	QString description() const override
		{
		return R"(Generates a PVOC from a spectrum. This process has a known error and will sound slightly off.

Length - Number:
	The output length.

Frequency - 1->1:
	The output frequency.

Harmonic Weights - 2->1:
	This takes time and a harmonic index, and returns a harmonic magnitude.
		)";
		}

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<NumberSliderModel> freqModel;
};
