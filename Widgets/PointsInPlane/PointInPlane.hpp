#pragma once

#include <QGraphicsObject>

#include "LotonController.hpp"

class PointInPlane : public LotonController, public QGraphicsItem
{ Q_OBJECT
public:
	PointInPlane( QPointF initial, QUuid id = QUuid() );

	QRectF boundingRect() const override;
	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,
		QWidget * widget = nullptr ) override;
	void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
	void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;
	void hoverEnterEvent( QGraphicsSceneHoverEvent * event ) override;
	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event ) override;

	//void grabMouse();

	enum { Type = UserType + 1 };
	int type() const override { return Type; }

private:
	float size;

	QVariant state() const override;
	void setState( const QVariant & s ) override;

	static constexpr float defaultSize = 10; //in pixels

//	struct PointMoveCommand : public QUndoCommand
//		{
//		PointMoveCommand( PointInPlane & model );
//		void undo() override;
//		void redo() override;
//		QPointF pos;
//		QPointF oldPos;
//		QUuid sceneId;
//		};
};
