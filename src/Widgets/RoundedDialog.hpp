#pragma once

//Taken from somewhere on stackoverflow

#include <QtWidgets>

class RoundedDialog : public QDialog
{ Q_OBJECT
public:
    explicit RoundedDialog( QWidget *parent = nullptr )
        : QDialog(parent)
    {
        // The FramelessWindowHint flag and WA_TranslucentBackground attribute are vital.
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
        setAttribute(Qt::WA_TranslucentBackground);
    }

    qreal radius = 5.0;        // desired radius in absolute pixels
    qreal borderWidth = -1.0;  // -1 : use style hint frame width;  0 : no border;  > 0 : use this width.

protected:
    void paintEvent(QPaintEvent *) override
    {
        if (!(windowFlags() & Qt::FramelessWindowHint) && !testAttribute(Qt::WA_TranslucentBackground))
            return;  // nothing to do

        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // Have style sheet?
        if (testAttribute(Qt::WA_StyleSheetTarget)) {
            // Let QStylesheetStyle have its way with us.
            QStyleOption opt;
            opt.initFrom(this);
            style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
            p.end();
            return;
        }

        // Paint thyself.
        QRectF rect(QPointF(0, 0), size());
        // Check for a border size.
        qreal penWidth = borderWidth;
        if (penWidth < 0.0) {
            QStyleOption opt;
            opt.initFrom(this);
            penWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &opt, this);
        }
        // Got pen?
        if (penWidth > 0.0) {
            p.setPen(QPen(palette().brush(foregroundRole()), penWidth));
            // Ensure border fits inside the available space.
            const qreal dlta = penWidth * 0.5;
            rect.adjust(dlta, dlta, -dlta, -dlta);
        }
        else {
            // QPainter comes with a default 1px pen when initialized on a QWidget.
            p.setPen(Qt::NoPen);
        }
        // Set the brush from palette role.
        p.setBrush(palette().brush(backgroundRole()));
        // Got radius?  Otherwise draw a quicker rect.
        if (radius > 0.0)
            p.drawRoundedRect(rect, radius, radius, Qt::AbsoluteSize);
        else
            p.drawRect(rect);

        // C'est finí
        p.end();
    }
};

