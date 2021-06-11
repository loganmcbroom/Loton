#pragma once

#include "LotonNodeModel.hpp"

class NumberSliderModel;

class NumberModel : public LotonNodeModel
{
public:
	NumberModel();
	~NumberModel() override;

	QString caption() const override;
	QString name() const override;
	bool captionVisible() const override;
	QString portCaption( PortType, PortIndex ) const override;
	bool portCaptionVisible( PortType, PortIndex ) const override;
	void setInData( std::shared_ptr<NodeData>, PortIndex  ) override;
	std::shared_ptr<NodeData> outData( QtNodes::PortIndex ) override;
	NodeDataType dataType( PortType, PortIndex ) const override;
	unsigned int nPorts( PortType ) const override;
	ControllerPairs makeInputControllers() override;

	std::unique_ptr<NumberSliderModel> numberModel;
};

