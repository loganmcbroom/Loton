#include "PVOCRetainNLoudestPartialsModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "Widgets/NumberSlider.hpp"

using namespace flan;

PVOCRetainNLoudestPartialsModel::PVOCRetainNLoudestPartialsModel()
	: PVOCProcessModel()
	, sliderModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	{
	QObject::connect( sliderModel.get(), &LotonController::stateChanged,
					this, &PVOCRetainNLoudestPartialsModel::updateData );

	auto sliderView = new NumberSliderView( sliderModel.get() );
	mainLayout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );
	}

QString PVOCRetainNLoudestPartialsModel::caption() const { return "Retain Loudest Partials"; }
QString PVOCRetainNLoudestPartialsModel::name() const { return "PVOC Retain Loudest Partials"; }

bool PVOCRetainNLoudestPartialsModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;
	auto partials = tryLockingInput<Func1x1Data>( ins[1], sliderModel->getSliderPosition() );

	setFunctor( [in, partials, c = canceller]()
		{
		return std::make_shared<PVOCData>( in.retainNLoudestPartials( partials->f, *c ) );
		} );

	return true;
	}

QString PVOCRetainNLoudestPartialsModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "PVOC";
			case 1: return "Partials";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}
unsigned int PVOCRetainNLoudestPartialsModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 2;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCRetainNLoudestPartialsModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return PVOCData::Type();
			case 1: return Func1x1Data::Type();
			default: return {"",""};
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0:  return PVOCData::Type();
			default: return {"",""};
			}
	else return {"",""};
	}

ControllerPairs PVOCRetainNLoudestPartialsModel::makeInputControllers()
	{
	auto slider = new NumberSliderView( sliderModel.get() );
	slider->setFixedHeight( 30 );
	return {{ portCaption( PortType::In, 1 ), slider }};
	}

QJsonObject PVOCRetainNLoudestPartialsModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["slider"] = sliderModel->save();
	return modelJson;
	}

void PVOCRetainNLoudestPartialsModel::restore( QJsonObject const & p )
	{
	sliderModel->restore( p["slider"].toObject() );
	}



