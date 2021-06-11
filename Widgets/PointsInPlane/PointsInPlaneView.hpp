#pragma once

#include <QGraphicsView>

#include "LotonController.hpp"

class PointsInPlaneModel;

class PointsInPlaneView : public QGraphicsView
{ Q_OBJECT
public:
	PointsInPlaneView( PointsInPlaneModel * );

	PointsInPlaneModel * model;

public slots:
	void scaleUp();
	void scaleDown();

protected:
	void drawBackground( QPainter *, const QRectF & ) override;

	void wheelEvent( QWheelEvent * ) override;
	void mousePressEvent( QMouseEvent * event ) override;
	void mouseMoveEvent( QMouseEvent * event ) override;
	void mouseReleaseEvent( QMouseEvent * event ) override;
	void mouseDoubleClickEvent( QMouseEvent * event ) override;

	QPointF sceneClickPos;

private:
	void translate( QPointF ); // Intentional shadowing

	struct ControllerProxy : public LotonController
		{
		ControllerProxy( PointsInPlaneView * _owner );
		QVariant state() const override;
		void setState( const QVariant & s ) override;
		PointsInPlaneView * owner;
		} controller;

	struct ZoomCommand : public QUndoCommand
		{
		ZoomCommand( const PointsInPlaneView &, float newScale );
		void undo() override;
		void redo() override;
		int id() const override;
		bool mergeWith(const QUndoCommand *other) override;

		QUuid viewId;
		float oldScale;
		float newScale;
		};

};


