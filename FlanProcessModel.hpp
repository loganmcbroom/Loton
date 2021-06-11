#pragma once

#include <array>

#include "LotonNodeModel.hpp"

#include <QtConcurrent>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;

class QVBoxLayout;
class NumberSlider;
class QLabel;

/*
 * Base class for all node models describing flan transformations
 *
 */

class FlanProcessModel : public LotonNodeModel
{ Q_OBJECT
public:
	struct PainterDelegate;

	FlanProcessModel();
	virtual ~FlanProcessModel() override;

	// Interface
	virtual bool process() = 0;
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0; //unique ID
	virtual QString portCaption( PortType, PortIndex ) const override = 0;
	virtual unsigned int nPorts( PortType type ) const override = 0;
	virtual NodeDataType dataType( PortType type, PortIndex index ) const override = 0;
	virtual ControllerPairs makeInputControllers() override { return {}; }
	virtual QWidget * makeHeaderWidget() override { return nullptr; }

	void setFunctor( std::function< std::shared_ptr<NodeData> () > functor );

public slots:
	void updateData();

protected:

	std::shared_ptr<NodeData> outData( PortIndex = 0 ) override; // Returns nullptr if there is no data
	QtNodes::NodePainterDelegate * painterDelegate() const override;
	virtual void inputConnectionDeleted( PortIndex ) override; //Clears input pointers on disconnect
	void setValidationState( QtNodes::NodeValidationState state, QString msg ) override;
	virtual void setInData( std::shared_ptr<NodeData> data, PortIndex ) override;
	//bool isProcessing() const;

	// Inputs
	std::vector<std::shared_ptr<NodeData>> ins;

	// Outputs (assuming single output for now)
	std::shared_ptr<NodeData> out;
	std::unique_ptr<QFuture<std::shared_ptr<NodeData>>> outFuture;
	std::unique_ptr<QFutureWatcher<std::shared_ptr<NodeData>>> outWatcher;

	std::shared_ptr<std::atomic<bool>> canceller;
	std::shared_ptr<QTimer> timeoutTimer;

	QVBoxLayout * mainLayout;

	std::unique_ptr<LED> indicator;

	//Utility used in derived classes
	template<typename T, typename S>
	static std::shared_ptr<T> tryLockingInput( std::shared_ptr<NodeData> input, S other )
		{
		return input?
			std::dynamic_pointer_cast<T>( input ) :
			std::make_shared<T>( other );
		}

private:
	bool portCaptionVisible( PortType, PortIndex ) const override;
};

