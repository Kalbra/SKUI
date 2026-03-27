#include "pad.h"

Pad::Pad(QGraphicsItem *parent, Interface *interface, PadSide pad_side)
    : QGraphicsItem{parent}
    , m_represented_interface(interface)
    , m_pad_side(pad_side)
{
    setAcceptHoverEvents(true);

    // Enable mouse events for cable connections
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
}

void Pad::setHover(bool hover)
{
    m_hover = hover;
    update();
}

QPoint Pad::getSceneDockPoint()
{
    QPoint point;
    int used_pad_size = PAD_SIZE;
    if (m_hover) {
        used_pad_size = PAD_EXPANDED;
    }

    if (m_pad_side == PadSide::Right) {
        point = QPoint(LINE_LENGTH + used_pad_size / 2, 0);
    } else { // m_pad_side == PadSide::Left
        point = QPoint(used_pad_size / 2, 0);
    }

    return mapToScene(point).toPoint();
}

void Pad::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black, LINE_WIDTH));

    if (m_pad_side == PadSide::Right) {
        painter->drawLine(0, 0, LINE_LENGTH, 0);
    } else { // m_pad_side == PadSide::Left
        painter->drawLine(PAD_EXPANDED, 0, LINE_LENGTH + PAD_EXPANDED, 0);
    }

    // Default if not hovered
    QColor used_inner_fill = Qt::blue;
    int used_pad_size = PAD_SIZE;

    if (m_hover) {
        used_inner_fill = Qt::red;
        used_pad_size = PAD_EXPANDED;
    }

    painter->setBrush(used_inner_fill);

    if (m_pad_side == PadSide::Right) {
        painter->drawEllipse(LINE_LENGTH, -(used_pad_size / 2), used_pad_size, used_pad_size);
    } else { // m_pad_side == PadSide::Left
        painter->drawEllipse(0, -(used_pad_size / 2), used_pad_size, used_pad_size);
    }

    //Debug, draw the bouding Rect
    //painter->setPen(QPen(Qt::red, 1));
    //painter->setBrush(QBrush());
    //painter->drawRect(boundingRect());
}

QRectF Pad::boundingRect() const
{
    return QRectF(-LINE_WIDTH,
                  -(PAD_EXPANDED / 2 + LINE_WIDTH),
                  PAD_EXPANDED + LINE_LENGTH + LINE_WIDTH * 2,
                  PAD_EXPANDED + LINE_WIDTH * 2);
}