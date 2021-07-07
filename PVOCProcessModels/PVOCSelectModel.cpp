#include "PVOCSelectModel.hpp"

#include <QBoxLayout>

#include "Widgets/NumberSlider.hpp"
#include "Widgets/Interpolator.hpp"

#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"


PVOCSelectModel::PVOCSelectModel()
	: PVOCProcessModel()
	, lengthModel( new NumberSliderModel( 0, 1, NumberSlider::infinity ) )
	, interpModel( new InterpolatorModel() )
	{
	auto lengthView = new NumberSliderView( lengthModel.get() );
	setToolTipToPort( lengthView, 1 );
	mainLayout->addWidget( lengthView );
	lengthView->setMinimumSize( 64, 20 );

	QObject::connect( lengthModel.get(), &NumberSliderModel::stateChanged,
					  this, &PVOCSelectModel::updateData );
	}

bool PVOCSelectModel::process()
	{
	if( ! ins[0] || ! ins[2] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;
	auto length = tryLockingInput<NumberData>( ins[1], lengthModel->getSliderPosition() );
	auto selector = std::dynamic_pointer_cast<Func2x2Data>( ins[2] );
	auto interp = tryLockingInput<InterpolatorData>( ins[3], interpModel->getInterpolator() );

	setFunctor( [in, length, selector, interp, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new PVOCData(
			in.select( length->f, selector->f, interp->f, *c ) ) );
		});

	return true;
	}

QString PVOCSelectModel::caption() const { return "Select"; }
QString PVOCSelectModel::name() const { return "PVOC Select"; }
QString PVOCSelectModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "PVOC";
			case 1: return "Length";
			case 2: return "Selector";
			case 3: return "Interpolator";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}

unsigned int PVOCSelectModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 4;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCSelectModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return PVOCData::Type();
			case 1: return NumberData::Type();
			case 2: return Func2x2Data::Type();
			case 3: return InterpolatorData::Type();
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

ControllerPairs PVOCSelectModel::makeInputControllers()
	{
	return {
		{ portCaption( PortType::In, 1 ), new NumberSliderView( lengthModel.get() ) },
		{ portCaption( PortType::In, 3 ), new LotonComboBoxView( interpModel.get() ) }
		};
	}

QJsonObject PVOCSelectModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["length"] = lengthModel->save();
	modelJson["interpolator"] = interpModel->save();
	return modelJson;
	}

void PVOCSelectModel::restore( QJsonObject const & p )
	{
	lengthModel->restore( p["length"].toObject() );
	interpModel->restore( p["interpolator"].toObject() );
	}


