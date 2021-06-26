#include "PVOCRemoveNLoudestPartialsModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "Widgets/NumberSlider.hpp"

using namespace flan;

PVOCRemoveNLoudestPartialsModel::PVOCRemoveNLoudestPartialsModel()
	: PVOCProcessModel()
	, sliderModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	{
	QObject::connect( sliderModel.get(), &NumberSliderModel::stateChanged,
					this, &PVOCRemoveNLoudestPartialsModel::updateData );

	auto sliderView = new NumberSliderView( sliderModel.get() );
	mainLayout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );
	}

QString PVOCRemoveNLoudestPartialsModel::caption() const { return "Remove Loudest Partials"; }
QString PVOCRemoveNLoudestPartialsModel::name() const { return "PVOC Remove Loudest Partials"; }

bool PVOCRemoveNLoudestPartialsModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;
	auto partials = tryLockingInput<Func1x1Data>( ins[1], sliderModel->getSliderPosition() );

	setFunctor( [in, partials, c = canceller]()
		{
		return std::make_shared<PVOCData>( in.removeNLoudestPartials( partials->f, *c ) );
		} );

	return true;
	}

QString PVOCRemoveNLoudestPartialsModel::portCaption( PortType type, PortIndex index ) const
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
unsigned int PVOCRemoveNLoudestPartialsModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 2;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCRemoveNLoudestPartialsModel::dataType( PortType type, PortIndex index ) const
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

ControllerPairs PVOCRemoveNLoudestPartialsModel::makeInputControllers()
	{
	auto slider = new NumberSliderView( sliderModel.get() );
	slider->setFixedHeight( 30 );
	return {{ portCaption( PortType::In, 1 ), slider }};
	}

QJsonObject PVOCRemoveNLoudestPartialsModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["slider"] = sliderModel->save();
	return modelJson;
	}

void PVOCRemoveNLoudestPartialsModel::restore( QJsonObject const & p )
	{
	sliderModel->restore( p["slider"].toObject() );
	}



