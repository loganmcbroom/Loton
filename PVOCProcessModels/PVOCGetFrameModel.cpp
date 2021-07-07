#include "PVOCGetFrameModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/NumberData.hpp"

#include "Widgets/NumberSlider.hpp"

using namespace flan;

PVOCGetFrameModel::PVOCGetFrameModel()
	: PVOCProcessModel()
	, frameTimeModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	{
	QObject::connect( frameTimeModel.get(), &NumberSliderModel::stateChanged,
					this, &PVOCGetFrameModel::updateData );

	auto sliderView = new NumberSliderView( frameTimeModel.get() );
	setToolTipToPort( sliderView, 1 );
	mainLayout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );
	}

PVOCGetFrameModel::	~PVOCGetFrameModel() = default;

QString PVOCGetFrameModel::caption() const { return "Get Frame"; }
QString PVOCGetFrameModel::name() const { return "PVOC Get Frame"; }

bool PVOCGetFrameModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;
	auto t = tryLockingInput<NumberData>( ins[1], frameTimeModel->getSliderPosition() );

	setFunctor( [in, t]()
		{
		return std::make_shared<PVOCData>( in.getFrame( t->f ) );
		} );

	return true;
	}

QString PVOCGetFrameModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "PVOC";
			case 1: return "Time";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}
unsigned int PVOCGetFrameModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 2;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCGetFrameModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return PVOCData::Type();
			case 1: return NumberData::Type();
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

ControllerPairs PVOCGetFrameModel::makeInputControllers()
	{
	auto slider = new NumberSliderView( frameTimeModel.get() );
	slider->setFixedHeight( 30 );
	return {{ portCaption( PortType::In, 1 ), slider }};
	}

QJsonObject PVOCGetFrameModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["slider"] = frameTimeModel->save();
	return modelJson;
	}

void PVOCGetFrameModel::restore( QJsonObject const & p )
	{
	frameTimeModel->restore( p["slider"].toObject() );
	}



