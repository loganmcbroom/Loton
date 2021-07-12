#pragma once

#include "FunctionGeneratorNodeModel.hpp"

class Func1x1PolynomialModel : public FunctionGeneratorNodeModel
{ Q_OBJECT
public:
	Func1x1PolynomialModel();

	QString caption() const override { return "Polynomial"; }
	QString name() const override { return "1->1 Polynomial"; }
	size_t numSliders() const override { return 4; }
	QString sliderCaption( size_t n ) const override { return std::vector({ "a", "b", "c", "d" })[n]; }
	float defaultSliderValue( size_t n ) const override { return std::vector({ 0, 0, 0, 0 })[n]; }
	QString graphTitle() const override { return "a + bx + cx<sup>2</sup> + dx<sup>3</sup>"; }
	std::shared_ptr<Func1x1Data> sliderFunction( const std::vector<float> & sliderValues ) override;
	QString description() const override
		{
		return R"(This returns a + bx + cx<sup>2</sup> + dx<sup>3</sup>.

Input - Number:
	If provided, the node will output a constant function equal to itself evaluated at the Input.
		)";
		}
};

