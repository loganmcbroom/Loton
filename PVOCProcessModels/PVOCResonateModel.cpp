#include "PVOCResonateModel.hpp"

#include <QBoxLayout>

#include "Widgets/NumberSlider.hpp"

#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"
#include "NodeDataTypes/NumberData.hpp"


PVOCResonateModel::PVOCResonateModel()
	: PVOCProcessModel()
	, resTimeModel( new NumberSliderModel( 0.5f, 0, NumberSlider::infinity ) )
	, lengthModel( new NumberSliderModel( 0.0, 0, NumberSlider::infinity ) )
	{
	auto resTimeView = new NumberSliderView( resTimeModel.get() );
	auto lengthView = new NumberSliderView( lengthModel.get() );
	resTimeView->setMinimumSize( 64, 20 );
	lengthView->setMinimumSize( 64, 20 );
	mainLayout->addWidget( resTimeView );
	mainLayout->addWidget( lengthView );

	QObject::connect( resTimeModel.get(), &NumberSliderModel::stateChanged,
					this, &PVOCResonateModel::updateData );
	QObject::connect( lengthModel.get(), &NumberSliderModel::stateChanged,
					this, &PVOCResonateModel::updateData );
	}

bool PVOCResonateModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;
	auto res = tryLockingInput<Func2x1Data>( ins[1], resTimeModel->getSliderPosition() );
	auto length = tryLockingInput<NumberData>( ins[2], in.getLength() + lengthModel->getSliderPosition() );

	setFunctor( [in, res, length, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new PVOCData( in.resonate( res->f, length->f, *c ) ) );
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
			case 1: return "Resonation";
			case 2: return "Extension";
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
			case 1: return Func2x1Data::Type();
			case 2: return NumberData::Type();
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
		{ portCaption( PortType::In, 1 ), new NumberSliderView( resTimeModel.get() ) },
		{ portCaption( PortType::In, 2 ), new NumberSliderView( lengthModel.get() ) }
		};
	}

QJsonObject PVOCResonateModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["resTime"] = resTimeModel->save();
	modelJson["length"] = lengthModel->save();
	return modelJson;
	}

void PVOCResonateModel::restore( QJsonObject const & p )
	{
	resTimeModel->restore( p["resTime"].toObject() );
	lengthModel->restore( p["length"].toObject() );
	}


