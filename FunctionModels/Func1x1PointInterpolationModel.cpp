#include "Func1x1PointInterpolationModel.hpp"

#include <QBoxLayout>
#include <QSizePolicy>

#include "Widgets/PointsInPlane/PointInterpolator.hpp"
#include "Widgets/Interpolator.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"

Func1x1PointInterpolationModel::Func1x1PointInterpolationModel()
	: Function1x1NodeModel()
	, piModel( new PointInterpolatorModel() )
	{
	auto mainLayout = new QHBoxLayout( mainWidget );
	mainWidget->setLayout( mainLayout );
	mainLayout->setContentsMargins( 0,7,0,2 );
	mainLayout->setSpacing( 0 );

	auto view = new PointInterpolatorView( piModel.get() );
	view->setFixedSize( 140, 110 );
	view->setStyleSheet( "PointsInPlaneView { border: 1px solid white; }" );
	view->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	mainLayout->addWidget( view );

	QObject::connect( piModel.get(), &PointInterpolatorModel::stateChanged, this, [this]()
		{
		updateOutput();
		emit computingFinished();
		emit dataUpdated( 0 );
		} );

	//updateOutput();
	//emit dataUpdated( 0 );
	}

Func1x1PointInterpolationModel::~Func1x1PointInterpolationModel() = default;

QString Func1x1PointInterpolationModel::caption() const
	{
	return "Point Interpolator";
	}

QString Func1x1PointInterpolationModel::name() const
	{
	return "Func1x1 Point Interpolator";
	}

std::shared_ptr<Func1x1Data> Func1x1PointInterpolationModel::function()
	{
	return piModel->function();
	}

QJsonObject Func1x1PointInterpolationModel::save() const
	{
	QJsonObject p = NodeDataModel::save();
	p["pi"] = piModel->save();
	return p;
	}

void Func1x1PointInterpolationModel::restore( const QJsonObject & p )
	{
	piModel->restore( p["pi"].toObject() );
	}

ControllerPairs Func1x1PointInterpolationModel::makeInputControllers()
	{
	return {
		{ "Interpolator", new LotonComboBoxView( piModel->interpModel.get() ) }
		};
	}

QWidget * Func1x1PointInterpolationModel::makeHeaderWidget()
	{
	auto h = new PointInterpolatorView( piModel.get() );
	h->setMinimumHeight( 256 );
	return h;
	}
