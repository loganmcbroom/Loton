#include "FlanProcessModel.hpp"

#include <QBoxLayout>

#include <nodes/Connection>

#include "Widgets/LED.hpp"
#include "Widgets/NumberSlider.hpp"

#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/SurfaceData.hpp"
#include "Settings.hpp"

using namespace QtNodes;

struct FlanProcessModel::PainterDelegate : public NodePainterDelegate
{
	void paint( QPainter * p, const NodeGeometry &, const NodeDataModel * model ) override
		{
		p->drawPixmap( -3, -3, LED::size(), LED::size(),
			dynamic_cast<const FlanProcessModel *>( model )->indicator->pix );
		}
};

FlanProcessModel::FlanProcessModel()
	: LotonNodeModel()
	, out( nullptr )
	, outFuture( nullptr )
	, outWatcher( nullptr )
	, canceller()
	, mainLayout( nullptr )
	, indicator( new LED )
	{
	mainLayout = new QVBoxLayout;
	mainLayout->setMargin( 0 );
	mainWidget->setLayout( mainLayout );

	setValidationState( NodeValidationState::Warning, "Missing Inputs" );

	//LED setup
	QObject::connect( this, &LotonNodeModel::computingStarted, this, [this]()
		{
		indicator->setState( LED::State::thinking );
		emit graphicsUpdateRequested();
		} );
	QObject::connect( this, &LotonNodeModel::computingFinished, this, [this]()
		{
		if( validationState() == NodeValidationState::Valid )
			indicator->setState( LED::State::on );
		else
			indicator->setState( LED::State::off );
		emit graphicsUpdateRequested();
		} );
    }

FlanProcessModel::~FlanProcessModel()
	{
	if( canceller )
		*canceller = true;
	}

void FlanProcessModel::setFunctor( std::function< std::shared_ptr<NodeData> () > functor )
	{
	outWatcher.reset( new QFutureWatcher<std::shared_ptr<NodeData>>() );
	outFuture.reset( new QFuture<std::shared_ptr<NodeData>>( QtConcurrent::run( functor ) ) );

	QObject::connect( outWatcher.get(), &QFutureWatcher<std::shared_ptr<NodeData>>::finished, this, [this, c = canceller]()
		{
		out = outFuture->result();
		emit computingFinished();
		emit dataUpdated( 0 );
		if( c )
			*c = true;
		} );

	outWatcher->setFuture( *outFuture );
	}

void FlanProcessModel::updateData()
	{
	ins.resize( nPorts( PortType::In ) );

	if( canceller ) canceller->store( true );
	canceller.reset( new std::atomic<bool>( false ) );

	timeoutTimer.reset( new QTimer );
	timeoutTimer->setSingleShot( true );
	timeoutTimer->setInterval( Settings::processTimeout() );
	QObject::connect( timeoutTimer.get(), &QTimer::timeout, [this, c = canceller]()
		{
		if( !*c ) // If the process is still running, cancel it and set validation to error
			{
			setValidationState( QtNodes::NodeValidationState::Error, "Process timed out." );
			*c = true;
			}
		} );

	// Don't process wipe commands
	if( hasWipedInput() ) return;

	bool accepted = process();

	if( accepted )
		{
		setValidationState( QtNodes::NodeValidationState::Valid, "" );
		timeoutTimer->start();
		emit computingStarted();
		}
	else
		{
		setValidationState( QtNodes::NodeValidationState::Warning, "Missing inputs" );
		emit dataUpdated( 0 );
		}
	}

void FlanProcessModel::inputsUpdated( std::shared_ptr<NodeData> data, PortIndex index )
	{
	updateData();
	}

void FlanProcessModel::wipeOutputs( PortIndex )
	{
	out = makeWipe();
	emit dataUpdated( 0 );
	}

std::shared_ptr<NodeData> FlanProcessModel::outData( PortIndex )
    {
	return out;
    }

bool FlanProcessModel::portCaptionVisible( PortType, PortIndex ) const
	{
	return true;
	}

void FlanProcessModel::inputConnectionDeleted( PortIndex i )
	{
	ins[i].reset();
	}

QtNodes::NodePainterDelegate * FlanProcessModel::painterDelegate() const
	{
	static FlanProcessModel::PainterDelegate d;
	return &d;
	}

void FlanProcessModel::setValidationState( QtNodes::NodeValidationState state, QString msg )
	{
	modelValidationState = state;
	modelValidationError = msg;
	if( state != NodeValidationState::Valid )
		{
		indicator->setState( LED::State::off );
		emit graphicsUpdateRequested();
		}
	}
