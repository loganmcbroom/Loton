#pragma once

#include "DistributionModel.hpp"

class DistributionUniformIntModel: public DistributionModel
{
public:
	DistributionUniformIntModel();

	QString caption() const override { return "Uniform Int Distribution"; }
	QString name() const override { return "Distributions - Uniform Int"; }
	size_t numSliders() const override { return 2; }
	QString sliderCaption( size_t n ) const override { return std::vector({ "a", "b" })[n]; }
	float defaultSliderValue( size_t n ) const override { return std::vector({ 0, 1 })[n]; }
	int evalFrequency() const override { return 1; }

	QString graphTitle() const override { return "P = 1/(b - a + 1)"; }
	std::shared_ptr<Func1x1Data> sliderFunction( const std::vector<float> & sliderValues ) override;
	void generateGraphFunction( const std::vector<float> & vs ) override;
};



