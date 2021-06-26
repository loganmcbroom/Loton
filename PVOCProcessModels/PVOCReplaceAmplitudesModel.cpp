#include "PVOCReplaceAmplitudesModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "Widgets/NumberSlider.hpp"

using namespace flan;

PVOCReplaceAmplitudesModel::PVOCReplaceAmplitudesModel()
	: PVOCProcessModel()
	, sliderModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	{
	QObject::connect( sliderModel.get(), &NumberSliderModel::stateChanged,
					this, &PVOCReplaceAmplitudesModel::updateData );

	auto sliderView = new NumberSliderView( sliderModel.get() );
	mainLayout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );
	}

QString PVOCReplaceAmplitudesModel::caption() const { return "Replace Amplitudes"; }
QString PVOCReplaceAmplitudesModel::name() const { return "PVOC Replace Amplitudes"; }

bool PVOCReplaceAmplitudesModel::process()
	{
	if( ! ins[0] || ! ins[1] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;
	auto other = std::dynamic_pointer_cast<PVOCData>( ins[1] );
	auto amount = tryLockingInput<Func2x1Data>( ins[2], sliderModel->getSliderPosition() );

	setFunctor( [in, other, amount, c = canceller]()
		{
		return std::make_shared<PVOCData>( in.replaceAmplitudes( other->pvoc, amount->f, *c ) );
		} );

	return true;
	}

QString PVOCReplaceAmplitudesModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "PVOC";
			case 1: return "Amp PVOC";
			case 2: return "Amount";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}
unsigned int PVOCReplaceAmplitudesModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 3;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCReplaceAmplitudesModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return PVOCData::Type();
			case 1: return PVOCData::Type();
			case 2: return Func2x1Data::Type();
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

ControllerPairs PVOCReplaceAmplitudesModel::makeInputControllers()
	{
	auto slider = new NumberSliderView( sliderModel.get() );
	slider->setFixedHeight( 30 );
	return {{ portCaption( PortType::In, 2 ), slider }};
	}

QJsonObject PVOCReplaceAmplitudesModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["slider"] = sliderModel->save();
	return modelJson;
	}

void PVOCReplaceAmplitudesModel::restore( QJsonObject const & p )
	{
	sliderModel->restore( p["slider"].toObject() );
	}



