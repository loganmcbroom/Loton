#pragma once

#include "DistributionModel.hpp"

class DistributionNormalModel: public DistributionModel
{
public:
	DistributionNormalModel();

	QString caption() const override { return "Normal Distribution"; }
	QString name() const override { return "Distributions - Normal"; }
	size_t numSliders() const override { return 2; }
	QString sliderCaption( size_t n ) const override { return std::vector({ "µ", "σ" })[n]; }
	float defaultSliderValue( size_t n ) const override { return std::vector({ 0, 1 })[n]; }
	float sliderLowerBound( size_t n ) const override;

	QString graphTitle() const override { return "φ"; }
	std::shared_ptr<Func1x1Data> sliderFunction( const std::vector<float> & sliderValues ) override;
	void generateGraphFunction( const std::vector<float> & vs ) override;
};

