#pragma once

#include "FlanProcessModel.hpp"

class QVBoxLayout;
class StringDisplayModel;

/*
 * Base class for all nodes extracting numerical data from Audio
 */

class AudioInfoProcessModel : public FlanProcessModel
{
public:
	AudioInfoProcessModel();
	virtual ~AudioInfoProcessModel() override;

	QWidget * makeHeaderWidget() override;

	QVBoxLayout * infoLayout;

private:
	std::unique_ptr<StringDisplayModel> infoModel;
};
