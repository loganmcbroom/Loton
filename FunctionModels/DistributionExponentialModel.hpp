#pragma once

#include "DistributionModel.hpp"

class DistributionExponentialModel: public DistributionModel
{
public:
	DistributionExponentialModel();

	QString caption() const override { return "Exponential Distribution"; }
	QString name() const override { return "Distributions - Exponential"; }
	size_t numSliders() const override { return 2; }
	QString sliderCaption( size_t n ) const override { return std::vector({ "µ", "λ" })[n]; }
	float defaultSliderValue( size_t n ) const override { return std::vector({ 0, 1 })[n]; }
	int evalFrequency() const override { return 1; }

	QString graphTitle() const override { return "P = λe^(-λt) + µ"; }
	std::shared_ptr<Func1x1Data> sliderFunction( const std::vector<float> & sliderValues ) override;
	void generateGraphFunction( const std::vector<float> & vs ) override;
};

