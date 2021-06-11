#include "AudioSelectModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include <nodes/Connection>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include "Widgets/NumberSlider.hpp"

#include "MainWindow.hpp"

using namespace flan;

AudioStreamSelectModel::AudioStreamSelectModel()
	: AudioProcessModel()
	, numInputs( 2 )
	, selectionModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	{
	auto v = new NumberSliderView( selectionModel.get() );
	mainLayout->addWidget( v );
	v->setMinimumSize( 64, 20 );
	QObject::connect( selectionModel.get(), &NumberSliderModel::stateChanged, this, &AudioStreamSelectModel::updateData );
	}

AudioStreamSelectModel::~AudioStreamSelectModel() = default;

bool AudioStreamSelectModel::process()
	{
	//Check for no connections
	if( ins.size() <= 2 )
		return false;

	auto selection = tryLockingInput<Func1x1Data>( ins[0], selectionModel->getSliderPosition() );

	// Lock variable number of inputs
	std::vector<flan::Audio> locks;
	for( int i = 1; i < ins.size() - 2; ++i )
		if( ins[i] )
			//Audio copy constructor doesn't perform deep copy, so this is fast
			locks.push_back( std::dynamic_pointer_cast<AudioData>( ins[i] )->audio );

	setFunctor( [locks, selection, c = canceller]()
		{
		return std::shared_ptr<NodeData>(
			new AudioData( Audio::select( locks, selection->f, std::vector<float>(), *c ) ) );
		});

	return true;
	}

QString AudioStreamSelectModel::caption() const { return "Select"; }
QString AudioStreamSelectModel::name() const { return "Audio Select"; }
QString AudioStreamSelectModel::portCaption( PortType t, PortIndex i ) const
	{
	if( t == PortType::In && i == 0 )
		return "Selection";
	return "Audio";
	}
unsigned int AudioStreamSelectModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:
			return numInPorts();
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioStreamSelectModel::dataType( PortType t, PortIndex i ) const
	{
	if( t == PortType::In && i == 0 )
		return Func1x1Data::Type();
	return AudioData::Type();
	}

ControllerPairs AudioStreamSelectModel::makeInputControllers()
	{
	auto v = new NumberSliderView( selectionModel.get() );
	v->setFixedHeight( 30 );
	return {{ portCaption( PortType::In, 0 ), v }};
	}

QJsonObject AudioStreamSelectModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson[portCaption( PortType::In, 0 )] = selectionModel->save();
	modelJson["numInputs"] = numInputs;
	return modelJson;
	}

void AudioStreamSelectModel::restore( QJsonObject const & p )
	{
	selectionModel->restore( p[portCaption( PortType::In, 0 )].toObject() );
	numInputs = p["numInputs"].toInt();
	}

//getPortIndex
void AudioStreamSelectModel::inputConnectionCreated( PortIndex addedPortIndex )
	{
	if( addedPortIndex == 0 ) return; // First port is selection

	if( addedPortIndex == numInputs - 1 )
		++numInputs;

	if( MainWindow::activeNode() == this )
		MainWindow::setActiveNode( this );

	emit nPortsChanged();
	}

void AudioStreamSelectModel::inputConnectionDeleted( PortIndex removedPortIndex )
	{
	if( removedPortIndex == 0 ) return; // First port is selection
	ins[removedPortIndex] = nullptr;

	//If removing the second to last port, shrink num ports
	if( removedPortIndex == nPorts( PortType::In ) - 2 )
		{
		//Find last connected port, or fail to find
		int lastConnectedPort = 0;
		for( int i = int( ins.size() ) - 3; i >= 1; --i )
			if( ins[i] )
				{
				lastConnectedPort = i;
				break;
				}

		//+1 for extra port, +1 for size being 1 above index
		ins.resize( lastConnectedPort + 2 );
		numInputs = lastConnectedPort + 2;
		}

	//Update node panel
	if( MainWindow::activeNode() == this )
		MainWindow::setActiveNode( this );

	emit nPortsChanged();
	}

int AudioStreamSelectModel::numInPorts() const
	{
	return numInputs;
	}


