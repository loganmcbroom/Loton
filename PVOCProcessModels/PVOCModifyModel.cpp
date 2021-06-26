#include "PVOCModifyModel.hpp"

#include "Widgets/Interpolator.hpp"

#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"
#include "Settings.hpp"

PVOCModifyModel::PVOCModifyModel()
	: PVOCProcessModel()
	, interpModel( new InterpolatorModel() )
	, deviceSelectorModel( new LotonComboBoxModel( 0, { "cpu", "gpu" } ) )
	{
	if( Settings::useOpenCL() )
		deviceSelectorModel->setSelection( 1 );

	QObject::connect( interpModel.get(), &InterpolatorModel::stateChanged, this, &PVOCModifyModel::updateData );
	QObject::connect( deviceSelectorModel.get(), &InterpolatorModel::stateChanged, this, &PVOCModifyModel::updateData );
	}

bool PVOCModifyModel::process()
	{
	if( ! ins[0] || ! ins[1] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;
	auto mod = std::dynamic_pointer_cast<Func2x2Data>( ins[1] );
	auto interp = tryLockingInput<InterpolatorData>( ins[2], interpModel->getInterpolator() );

	const bool useGPU = deviceSelectorModel->selection() == 1 && Settings::useOpenCL();

	setFunctor( [in, mod, interp, useGPU, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new PVOCData( useGPU?
			in.modify_cl( mod->f, interp->f, *c ) :
			in.modify( mod->f, interp->f, *c )
			) );
		});

	return true;
	}

QString PVOCModifyModel::caption() const { return "Modify"; }
QString PVOCModifyModel::name() const { return "PVOC Modify"; }
QString PVOCModifyModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "PVOC";
			case 1: return "Modifier";
			case 2: return "Interpolator";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}

unsigned int PVOCModifyModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 3;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCModifyModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return PVOCData::Type();
			case 1: return Func2x2Data::Type();
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

ControllerPairs PVOCModifyModel::makeInputControllers()
	{
	ControllerPairs ps;
	ps.push_back( { portCaption( PortType::In, 2 ), new LotonComboBoxView( interpModel.get() ) } );
	if( Settings::useOpenCL() )
		ps.push_back( { "Compute Device", new LotonComboBoxView( deviceSelectorModel.get() ) } );

	return ps;
	}

QJsonObject PVOCModifyModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["interpolator"] = interpModel->save();
	modelJson["device"] = deviceSelectorModel->save();
	return modelJson;
	}

void PVOCModifyModel::restore( QJsonObject const & p )
	{
	interpModel->restore( p["interpolator"].toObject() );
	deviceSelectorModel->restore( p["device"].toObject() );

	if( ! Settings::useOpenCL() )
		deviceSelectorModel->setSelection( 0 );
	}


