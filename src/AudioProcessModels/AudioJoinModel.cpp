#include "AudioJoinModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include <nodes/Connection>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include "Widgets/Interpolator.hpp"
#include "Widgets/NumberSlider.hpp"

#include "MainWindow.hpp"

using namespace flan;

AudioJoinModel::AudioJoinModel()
	: AudioProcessModel()
	, numInputs( 1 )
	{
	}

bool AudioJoinModel::process()
	{
	//Check for no connections
	if( ins.size() == 1 )
		return false;

	std::vector<flan::Audio> locks;
	for( int i = 0; i < ins.size() - 1; ++i )
		if( ins[i] )
			//Audio copy constructor doesn't perform deep copy, so this is fast
			locks.push_back( std::dynamic_pointer_cast<AudioData>( ins[i] )->audio );

	setFunctor( [locks, c = canceller]()
		{
		return std::shared_ptr<NodeData>(
			new AudioData( Audio::join( locks, *c ) ) );
		});

	return true;
	}

QString AudioJoinModel::caption() const { return "Join"; }

QString AudioJoinModel::name() const { return "Audio Join"; }

QString AudioJoinModel::portCaption( PortType, PortIndex ) const
	{
	return "Audio";
	}

unsigned int AudioJoinModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return numInPorts();
		case PortType::Out: return 1;
		default: return 0;
		}
	}

NodeDataType AudioJoinModel::dataType( PortType, PortIndex ) const
	{
	return AudioData::Type();
	}

QJsonObject AudioJoinModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["numPorts"] = numInputs;
	return modelJson;
	}

void AudioJoinModel::restore( QJsonObject const & p )
	{
	numInputs = p["numPorts"].toInt();
	}

//getPortIndex
void AudioJoinModel::inputConnectionCreated( PortIndex addedPortIndex )
	{	
	qDebug() << "inputConnectionCreated";

	if( addedPortIndex == numInputs - 1 )
		++numInputs;

	if( MainWindow::activeNode() == this )
		MainWindow::setActiveNode( this );

	emit nPortsChanged();
	}

void AudioJoinModel::inputConnectionDeleted( PortIndex removedPortIndex )
	{
	qDebug() << "inputConnectionDeleted";
	//Compress remaining ports
	ins[removedPortIndex] = nullptr;

	//If removing the second to last port, shrink time
	if( removedPortIndex == nPorts( PortType::In ) - 2 )
		{
		//Find last connected port, or fail to find
		int lastConnectedPort = -1;
		for( int i = int( ins.size() ) - 3; i >= 0; --i )
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

int AudioJoinModel::numInPorts() const
	{
	return numInputs;
	}

