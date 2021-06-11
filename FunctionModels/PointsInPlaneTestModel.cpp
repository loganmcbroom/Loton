#include "PointsInPlaneTestModel.hpp"

#include "Widgets/PointsInPlane/PointsInPlaneModel.hpp"
#include "Widgets/PointsInPlane/PointsInPlaneView.hpp"

PointsInPlaneTestModel::PointsInPlaneTestModel()
	: pipModel( new PointsInPlaneModel() )
	{
	}

PointsInPlaneTestModel::~PointsInPlaneTestModel() = default;

QWidget * PointsInPlaneTestModel::makeHeaderWidget()
	{
	auto pip = new PointsInPlaneView( pipModel.get() );
	pip->setMinimumHeight( 256 );
	return pip;
	}
