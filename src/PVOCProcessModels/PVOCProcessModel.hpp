#pragma once

#include "FlanProcessModel.hpp"

class QVBoxLayout;
class PVOCPlayerModel;

/*
 * Base class for all nodes processing flan data
 */

class PVOCProcessModel : public FlanProcessModel
{ Q_OBJECT
public:
	PVOCProcessModel();
	virtual ~PVOCProcessModel() override;

	QWidget * makeHeaderWidget() override;

	QVBoxLayout * flanLayout;

private:
	std::unique_ptr<PVOCPlayerModel> playerModel;
};

