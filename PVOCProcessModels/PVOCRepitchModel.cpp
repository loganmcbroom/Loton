#include "PVOCRepitchModel.hpp"

#include <QBoxLayout>

#include "Widgets/Interpolator.hpp"
#include "Widgets/NumberSlider.hpp"
#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"


PVOCRepitchModel::PVOCRepitchModel()
	: PVOCProcessModel()
	, factorModel( new NumberSliderModel( 1, 0, NumberSlider::infinity ) )
	, interpModel( new InterpolatorModel() )
	{
	QObject::connect( factorModel.get(), &NumberSliderModel::stateChanged,
					this, &PVOCRepitchModel::updateData );

	QObject::connect( interpModel.get(), &InterpolatorModel::stateChanged,
					this, &PVOCRepitchModel::updateData );

	auto sliderView = new NumberSliderView( factorModel.get() );
	mainLayout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );
	}

bool PVOCRepitchModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->flan;
	auto factor = tryLockingInput<Func2x1Data>( ins[1], factorModel->getSliderPosition() );
	auto interp = tryLockingInput<InterpolatorData>( ins[2], interpModel->getInterpolator() );

	setFunctor( [in, factor, interp, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new PVOCData( in.repitch( factor->f, interp->f, *c ) ) );
		});

	return true;
	}

QString PVOCRepitchModel::caption() const { return "Repitch"; }
QString PVOCRepitchModel::name() const { return "PVOC Repitch"; }
QString PVOCRepitchModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "PVOC";
			case 1: return "Factor";
			case 2: return "Interpolator";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}

unsigned int PVOCRepitchModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 3;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCRepitchModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return PVOCData::Type();
			case 1: return Func2x1Data::Type();
			case 2: return InterpolatorData::Type();
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

ControllerPairs PVOCRepitchModel::makeInputControllers()
	{
	return {
		{ portCaption( PortType::In, 1 ), new NumberSliderView( factorModel.get() ) },
		{ portCaption( PortType::In, 2 ), new LotonComboBoxView( interpModel.get() ) }
		};
	}

QJsonObject PVOCRepitchModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["factor"] = factorModel->save();
	modelJson["interpolator"] = interpModel->save();
	return modelJson;
	}

void PVOCRepitchModel::restore( QJsonObject const & p )
	{
	factorModel->restore( p["factor"].toObject() );
	interpModel->restore( p["interpolator"].toObject() );
	}
