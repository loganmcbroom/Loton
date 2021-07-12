#include "PVOCResonateModel.hpp"

#include <QBoxLayout>

#include "Widgets/NumberSlider.hpp"

#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"
#include "NodeDataTypes/NumberData.hpp"


PVOCResonateModel::PVOCResonateModel()
	: PVOCProcessModel()
	, lengthModel( new NumberSliderModel( 0.0, 0, NumberSlider::infinity ) )
	, resTimeModel( new NumberSliderModel( 0.5f, 0, NumberSlider::infinity ) )
	{
	auto lengthView = new NumberSliderView( lengthModel.get() );
	auto resTimeView = new NumberSliderView( resTimeModel.get() );
	lengthView->setMinimumSize( 64, 20 );
	resTimeView->setMinimumSize( 64, 20 );
	mainLayout->addWidget( lengthView );
	mainLayout->addWidget( resTimeView );
	setToolTipToPort( lengthView, 2 );
	setToolTipToPort( resTimeView, 1 );

	QObject::connect( lengthModel.get(), &NumberSliderModel::stateChanged,
					this, &PVOCResonateModel::updateData );
	QObject::connect( resTimeModel.get(), &NumberSliderModel::stateChanged,
					this, &PVOCResonateModel::updateData );

	}

bool PVOCResonateModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;
	auto length = tryLockingInput<NumberData>( ins[2], in.getLength() + lengthModel->getSliderPosition() );
	auto res = tryLockingInput<Func2x1Data>( ins[1], resTimeModel->getSliderPosition() );

	setFunctor( [in, length, res, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new PVOCData( in.resonate( length->f, res->f, *c ) ) );
		});

	return true;
	}

QString PVOCResonateModel::caption() const { return "Resonate"; }
QString PVOCResonateModel::name() const { return "PVOC Resonate"; }
QString PVOCResonateModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "PVOC";
			case 1: return "Length";
			case 2: return "Decay";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}

unsigned int PVOCResonateModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 3;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCResonateModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return PVOCData::Type();
			case 1: return NumberData::Type();
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

ControllerPairs PVOCResonateModel::makeInputControllers()
	{
	return {
		{ portCaption( PortType::In, 1 ), new NumberSliderView( lengthModel.get() ) },
		{ portCaption( PortType::In, 2 ), new NumberSliderView( resTimeModel.get() ) }
		};
	}

QJsonObject PVOCResonateModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["length"] = lengthModel->save();
	modelJson["resTime"] = resTimeModel->save();
	return modelJson;
	}

void PVOCResonateModel::restore( QJsonObject const & p )
	{
	lengthModel->restore( p["length"].toObject() );
	resTimeModel->restore( p["resTime"].toObject() );
	}


