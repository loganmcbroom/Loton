#include "NumberModel.hpp"

#include <QBoxLayout>

#include "NodeDataTypes/NumberData.hpp"
#include "Widgets/NumberSlider.hpp"

NumberModel::NumberModel()
	: numberModel( new NumberSliderModel )
	{
	QObject::connect( numberModel.get(), &NumberSliderModel::stateChanged, this, [this]()
		{
		out = std::make_shared<NumberData>( numberModel->getSliderPosition() );
		emit dataUpdated( 0 );
		} );

	auto sliderView = new NumberSliderView( numberModel.get() );
	auto layout = new QVBoxLayout();
	mainWidget->setLayout( layout );
	layout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );

	// Set indicator to green
	emit computingFinished();
	}

NumberModel::~NumberModel() = default;

QString NumberModel::caption() const { return "Number"; }

QString NumberModel::name() const { return "Number"; }

bool NumberModel::captionVisible() const { return false; }

QString NumberModel::portCaption( PortType, PortIndex ) const { return ""; }

bool NumberModel::portCaptionVisible( PortType, PortIndex ) const { return true; }

std::shared_ptr<NodeData> NumberModel::outData( QtNodes::PortIndex )
	{
	return out;
	}

NodeDataType NumberModel::dataType( PortType, PortIndex ) const
	{
	return NumberData::Type();
	}

unsigned int NumberModel::nPorts( PortType t ) const
	{
	return t == PortType::In ? 0 : 1;
	}

ControllerPairs NumberModel::makeInputControllers()
	{
	auto slider = new NumberSliderView( numberModel.get() );
	slider->setFixedHeight( 30 );
	return { { "Number", slider }};
	}

void NumberModel::wipeOutputs( PortIndex )
	{
	out = makeWipe();
	emit dataUpdated( 0 );
	}

QJsonObject NumberModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["number"] = numberModel->save();
	return modelJson;
	}

void NumberModel::restore( const QJsonObject & p )
	{
	numberModel->restore( p["number"].toObject() );
	}
