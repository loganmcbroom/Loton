#include "PVOCAddOctavesModel.hpp"

#include <QBoxLayout>

#include "Widgets/Interpolator.hpp"
#include "Widgets/NumberSlider.hpp"

#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"


PVOCAddOctavesModel::PVOCAddOctavesModel()
	: PVOCProcessModel()
	{
	}

bool PVOCAddOctavesModel::process()
	{
	if( ! ins[0] || !ins[1] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;
	auto series = std::dynamic_pointer_cast<Func2x1Data>( ins[1] );

	setFunctor( [in, series, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new PVOCData( in.addOctaves( series->f, *c ) ) );
		});

	return true;
	}

QString PVOCAddOctavesModel::caption() const { return "Add Octaves"; }
QString PVOCAddOctavesModel::name() const { return "PVOC Add Octaves"; }
QString PVOCAddOctavesModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "PVOC";
			case 1: return "Series";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}

unsigned int PVOCAddOctavesModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 2;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCAddOctavesModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return PVOCData::Type();
			case 1: return Func2x1Data::Type();
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

ControllerPairs PVOCAddOctavesModel::makeInputControllers()
	{
	return {};
	}

QJsonObject PVOCAddOctavesModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	return modelJson;
	}

void PVOCAddOctavesModel::restore( QJsonObject const & p )
	{
	}


