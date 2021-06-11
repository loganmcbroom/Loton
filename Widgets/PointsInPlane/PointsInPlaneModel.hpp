#pragma once

#include <QGraphicsScene>

#include "LotonController.hpp"

class PointInPlane;

class PointsInPlaneModel : public QGraphicsScene
{ Q_OBJECT
public:
	PointsInPlaneModel();
	~PointsInPlaneModel() override;

	//void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
	//void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event ) override;

	std::vector<PointInPlane *> points() const;
	std::vector<QPointF> pointPositions() const;

	void createPointAt( QPointF );
	void removePoint( PointInPlane * );
	//void removePointAt( QPointF );

	QUuid id() const;
	virtual QJsonObject save() const;
	virtual void restore( const QJsonObject & );

	static bool xThenYSortCompare( QPointF, QPointF );

	constexpr static float coarseGridSize = 1.0f;
	constexpr static float fineGridSize = 0.2f;

signals:
	void stateChanged();

private:
	friend class PointInPlane;

	struct ControllerProxy : public LotonController
		{
		ControllerProxy( PointsInPlaneModel * _owner )
			: LotonController( QVariant() ) //Null variant, we're not using controller undos
			, scene( _owner )
			{}
		QVariant state() const override;
		void setState( const QVariant & s ) override;
		PointsInPlaneModel * scene;
		} controller;

	struct PointAddCommand : public QUndoCommand
		{
		PointAddCommand( PointsInPlaneModel & model, QPointF p );
		void undo() override;
		void redo() override;
		const QPointF pos;
		const QUuid sceneId;
		const QUuid itemId;
		};

	struct PointRemoveCommand : public QUndoCommand
		{
		PointRemoveCommand( PointsInPlaneModel & scene, PointInPlane & point );
		void undo() override;
		void redo() override;
		const QPointF pos;
		const QUuid sceneId;
		const QUuid itemId;
		};
};


