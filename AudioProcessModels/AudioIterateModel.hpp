#pragma once

#include "AudioFeedbackProcessModel.hpp"

class NumberSliderModel;
class OnOffButtonModel;
class NumberData;
class AudioData;
class AudioIterateThread;

class AudioIterateModel : public AudioFeedbackProcessModel
	{ Q_OBJECT
public:
	AudioIterateModel();
	virtual ~AudioIterateModel() override;

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(This repeats the input n times.

N - Number:
	The number of desired iterations
		)";
		}

	std::unique_ptr<NumberSliderModel> numIterationsModel;
	std::unique_ptr<OnOffButtonModel> recursionModel;
	};



