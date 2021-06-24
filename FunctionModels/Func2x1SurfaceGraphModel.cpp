#include "Func2x1SurfaceGraphModel.hpp"

#include <QBoxLayout>
#include <QLabel>

#include "NodeDataTypes/FunctionData.hpp"
#include "Widgets/SurfaceGraph.hpp"
#include "Widgets/StringDisplay.hpp"
#include "Widgets/NumberSlider.hpp"

Func2x1SurfaceGraphModel::Func2x1SurfaceGraphModel()
	: graphModel( new SurfaceGraphModel() )
	, timeMax( new NumberSliderModel( 5.0f, 0.0f, NumberSlider::infinity ) )
	, freqMax( new NumberSliderModel( 20000.0f, 0.0f, NumberSlider::infinity ) )
	{
	//Connect range slider models to graph model ranges
	QObject::connect( timeMax.get(), &NumberSliderModel::stateChanged, this, [this]()
		{
		graphModel->setMaxTime( timeMax->getSliderPosition() );
		} );
	QObject::connect( freqMax.get(), &NumberSliderModel::stateChanged, this, [this]()
		{
		graphModel->setMaxFreq( freqMax->getSliderPosition() );
		} );

	auto layout = new QVBoxLayout();
	mainWidget->setLayout( layout );
	layout->setContentsMargins( 0, 5, 0, 0 );

	auto label = new StringDisplay( "See details panel." );
	label->setFixedSize( 120, 32 );
	layout->addWidget( label );
	}

Func2x1SurfaceGraphModel::~Func2x1SurfaceGraphModel()
	{
	}

QString Func2x1SurfaceGraphModel::caption() const
	{
	return "Surface Graph";
	}

QString Func2x1SurfaceGraphModel::name() const
	{
	return "Surface Graph";
	}

QString Func2x1SurfaceGraphModel::portCaption( PortType, PortIndex ) const
	{
	return "Func2x1";
	}

void Func2x1SurfaceGraphModel::inputsUpdated( std::shared_ptr<NodeData> data, PortIndex )
	{
	out = ins[0];
	graphModel->setFunction( std::static_pointer_cast<Func2x1Data>( ins[0] ) );
	emit dataUpdated( 0 );
	}

void Func2x1SurfaceGraphModel::wipeOutputs( PortIndex )
	{
	out = makeWipe();
	emit dataUpdated( 0 );
	}

std::shared_ptr<NodeData> Func2x1SurfaceGraphModel::outData( PortIndex )
	{
	return out;
	}

NodeDataType Func2x1SurfaceGraphModel::dataType( PortType, PortIndex ) const
	{
	return Func2x1Data::Type();
	}

unsigned int Func2x1SurfaceGraphModel::nPorts( PortType ) const
	{
	return 1;
	}

QJsonObject Func2x1SurfaceGraphModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["timeMax"] = timeMax->save();
	modelJson["freqMax"] = freqMax->save();
	return modelJson;
	}

void Func2x1SurfaceGraphModel::restore( QJsonObject const & p )
	{
	timeMax->restore( p["timeMax"].toObject() );
	freqMax->restore( p["freqMax"].toObject() );
	}

void Func2x1SurfaceGraphModel::inputConnectionDeleted( PortIndex  )
	{
	graphModel->setFunction( nullptr );
	}

ControllerPairs Func2x1SurfaceGraphModel::makeInputControllers()
	{
	auto t = new NumberSliderView( timeMax.get() );
	auto f = new NumberSliderView( freqMax.get() );
	t->setFixedHeight( 30 );
	f->setFixedHeight( 30 );
	return
		{
		{ "Time Max", t },
		{ "Freq Max", f }
		};
	}

QWidget * Func2x1SurfaceGraphModel::makeHeaderWidget()
	{
	auto view = new SurfaceGraphView( graphModel.get() );
	view->setFixedHeight( 512 );
	return view;
	}

