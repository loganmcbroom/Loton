#include "AudioMixModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include <nodes/Connection>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include "Widgets/Interpolator.hpp"
#include "Widgets/NumberSlider.hpp"

#include "MainWindow.hpp"

using namespace flan;

AudioMixModel::AudioMixModel()
	: AudioProcessModel()
	, interpModel( new NumberSliderModel( 0, -NumberSlider::infinity, 0 ) )
	, balanceModels()
	{
	addBalance();
	QObject::connect( interpModel.get(), &LotonComboBoxModel::stateChanged, this, &AudioMixModel::updateData );
	}

AudioMixModel::~AudioMixModel() = default;

bool AudioMixModel::process()
	{
	//Check for no connections
	if( ins.size() == 1 )
		return false;

	std::vector<flan::Audio> locks;
	for( int i = 0; i < ins.size() - 1; ++i )
		if( ins[i] )
			//Audio copy constructor doesn't perform deep copy, so this is fast
			locks.push_back( std::dynamic_pointer_cast<AudioData>( ins[i] )->audio );

	std::vector<flan::Func1x1> balances( balanceModels.size() );
	const float mixScale = std::pow( locks.size(), interpModel->getSliderPosition() );
	std::transform( balanceModels.begin(), balanceModels.end(), balances.begin(), [mixScale]( std::unique_ptr<NumberSliderModel> & i)
		{
		return i->getSliderPosition() * mixScale;
		});

	setFunctor( [locks, balances, c = canceller]()
		{
		return std::shared_ptr<NodeData>(
			new AudioData( Audio::mix( locks, balances, std::vector<float>(), *c ) ) );
		});

	return true;
	}

QString AudioMixModel::caption() const { return "Mix"; }
QString AudioMixModel::name() const { return "Audio Mix"; }
QString AudioMixModel::portCaption( PortType, PortIndex ) const
	{
	return "Audio";
	}
unsigned int AudioMixModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:
			return numInPorts();
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioMixModel::dataType( PortType, PortIndex ) const
	{
	return AudioData::Type();
	}

ControllerPairs AudioMixModel::makeInputControllers()
	{
	auto interpView = new NumberSliderView( interpModel.get() );
	interpView->setFixedHeight( 30 );
	ControllerPairs ps;
	ps.emplace_back( "Scaling Exponent", interpView );
	for( int i = 0; i < balanceModels.size() - 1; ++i )
		{
		auto v = new NumberSliderView( balanceModels[i].get() );
		ps.emplace_back( QString("Gain ").append( QString::number( i ) ), v );
		}
	return ps;
	}

QJsonObject AudioMixModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["interpScale"] = interpModel->save();
	modelJson["numPorts"] = numInPorts();
	return modelJson;
	}

void AudioMixModel::restore( QJsonObject const & p )
	{
	interpModel->restore( p["interpScale"].toObject() );
	const int numBalances = p["numPorts"].toInt();
	for( int i = 0; i < numBalances - 1; ++i )
		addBalance();
	}

//getPortIndex
void AudioMixModel::inputConnectionCreated ( PortIndex addedPortIndex )
	{
	if( addedPortIndex == balanceModels.size() - 1 )
		{
		addBalance();
		}

	if( MainWindow::activeNode() == this )
		MainWindow::setActiveNode( this );

	emit nPortsChanged();
	}

void AudioMixModel::inputConnectionDeleted( PortIndex removedPortIndex )
	{
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
		balanceModels.resize( lastConnectedPort + 2 );
		}

	//Update node panel
	if( MainWindow::activeNode() == this )
		MainWindow::setActiveNode( this );

	emit nPortsChanged();
	}

int AudioMixModel::numInPorts() const
	{
	return int( balanceModels.size() );
	}

void AudioMixModel::addBalance()
	{
	balanceModels.emplace_back( new NumberSliderModel( 1, 0.0, NumberSlider::infinity ) );
	QObject::connect( balanceModels.back().get(), &NumberSliderModel::stateChanged, this, &AudioMixModel::updateData );
	}


