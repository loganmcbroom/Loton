#pragma once

#include "FlanProcessModel.hpp"

class QVBoxLayout;
class AudioPlayerModel;

/*
 * Base class for all nodes processing audio data
 */

class AudioProcessModel : public FlanProcessModel
{ Q_OBJECT
public:
	AudioProcessModel();
	virtual ~AudioProcessModel() override;

	QWidget * makeHeaderWidget() override;

	QVBoxLayout * audioLayout;

private:
	std::unique_ptr<AudioPlayerModel> playerModel;
};
