#include "PVOCGenerateModel.hpp"

#include <QBoxLayout>

#include "Widgets/Interpolator.hpp"
#include "Widgets/NumberSlider.hpp"

#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/NumberData.hpp"


PVOCGenerateModel::PVOCGenerateModel()
	: PVOCProcessModel()
	, lengthModel( new NumberSliderModel( 1, 0 ) )
	, freqModel( new NumberSliderModel( 1, 0 ) )
	{
	QObject::connect( lengthModel.get(), &InterpolatorModel::stateChanged, this, &PVOCGenerateModel::updateData );
	QObject::connect( freqModel.get(), &InterpolatorModel::stateChanged, this, &PVOCGenerateModel::updateData );
	auto lengthView = new NumberSliderView( lengthModel.get() );
	auto freqView = new NumberSliderView( freqModel.get() );
	mainLayout->addWidget( lengthView );
	mainLayout->addWidget( freqView );
	lengthView->setMinimumSize( 64, 20 );
	freqView->setMinimumSize( 64, 20 );
	setToolTipToPort( lengthView, 0 );
	setToolTipToPort( freqView, 1 );
	}

bool PVOCGenerateModel::process()
	{
	if( ! ins[2] ) return false;

	auto length = tryLockingInput<NumberData>( ins[0], lengthModel->getSliderPosition() );
	auto freq = tryLockingInput<Func1x1Data>( ins[1], freqModel->getSliderPosition() );
	auto series = std::dynamic_pointer_cast<Func2x1Data>( ins[2] );

	setFunctor( [length, freq, series, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new PVOCData( flan::PVOC::generate( length->f, freq->f, series->f, *c ) ) );
		});

	return true;
	}

QString PVOCGenerateModel::caption() const { return "Generate"; }
QString PVOCGenerateModel::name() const { return "PVOC Generate"; }
QString PVOCGenerateModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Length";
			case 1: return "Frequency";
			case 2: return "Series";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}

unsigned int PVOCGenerateModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 3;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCGenerateModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return NumberData::Type();
			case 1: return Func1x1Data::Type();
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

ControllerPairs PVOCGenerateModel::makeInputControllers()
	{
	return {
		{ "Length", new NumberSliderView( lengthModel.get() ) },
		{ "Frequency", new NumberSliderView( freqModel.get() ) }
		};
	}

QJsonObject PVOCGenerateModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["Length"] = lengthModel->save();
	modelJson["Frequency"] = freqModel->save();
	return modelJson;
	}

void PVOCGenerateModel::restore( QJsonObject const & p )
	{
	lengthModel->restore( p["Length"].toObject() );
	freqModel->restore( p["Frequency"].toObject() );
	}


