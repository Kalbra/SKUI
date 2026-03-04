#include "node_graphicsitem.h"
#include <QGraphicsScene>

NodeGraphicsItem::NodeGraphicsItem(QGraphicsItem *parent, Node *node)
    : QGraphicsItem{parent}
    , m_node(node)
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    QFont font = QFont();
    font.setBold(true);
    font.setPointSize(FONT_SIZE);
    m_font = font;

    m_object_name = m_node->objectName();
    m_node_name_geometry = QFontMetrics(m_font).boundingRect(m_object_name);
    int rect_width = m_node_name_geometry.width() + BOX_WIDTH * 2 + BOX_MARGIN.right() * 2;
    m_frame_geometry = QRect(PAD_SPACING, 0, rect_width, 200);

    createPads(m_frame_geometry);
}

void NodeGraphicsItem::createPads(QRect frame_geometry)
{
    int left_y_offset = PAD_SPACING;
    int right_y_offset = PAD_SPACING;

    for (Interface &interface : m_node->getInterfaces()) {
        if (interface.getDirection() == InterfaceDirection::Input) {
            Pad *pad = new Pad(this, &interface, PadSide::Left);
            int x_pos = frame_geometry.left() - pad->boundingRect().width() + BOX_WIDTH;
            pad->setPos(x_pos, left_y_offset);
            left_y_offset += PAD_SPACING;
        } else if (interface.getDirection() == InterfaceDirection::Output) {
            Pad *pad = new Pad(this, &interface, PadSide::Right);
            int x_pos = frame_geometry.right() + BOX_WIDTH * 2;
            pad->setPos(x_pos, right_y_offset);
            right_y_offset += PAD_SPACING;
        }
    }

    m_frame_geometry.setHeight(std::max(left_y_offset, right_y_offset));
}

void NodeGraphicsItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget)
{
    painter->setFont(m_font);
    QPen pen(BOX_COLOR, BOX_WIDTH);
    painter->setPen(pen);

    QBrush brush(INNER_COLOR);
    painter->setBrush(brush);

    drawOuterFrame(painter);

    //Debug, draw name bounding
    //painter->setPen(Qt::red);
    //painter->setBrush(QBrush());
    //painter->drawRect(m_frame_geometry);
}

void NodeGraphicsItem::drawOuterFrame(QPainter *painter)
{
    painter->setFont(m_font);
    painter->drawRect(m_frame_geometry);
    painter->drawText(BOX_MARGIN.left() + BOX_WIDTH + PAD_SPACING,
                      -m_node_name_geometry.y(),
                      m_object_name);
}

void NodeGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_drag_start_pos = event->pos();
        m_is_dragging = true;
        event->accept();
    } else {
        QGraphicsItem::mousePressEvent(event);
    }
}

void NodeGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_is_dragging) {
        QGraphicsItem::mouseMoveEvent(event);
        return;
    }

    QPointF mouse_offset = event->pos() - m_drag_start_pos;
    QPointF new_pos = pos() + mouse_offset;

    setPos(new_pos);
    event->accept();
}

void NodeGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_is_dragging) {
        m_is_dragging = false;
        event->accept();
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void NodeGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "NodeGraphicsItem hover enter" << event->scenePos()
             << mapFromScene(event->scenePos());
    QPointF local_pos = mapFromScene(event->scenePos());

    for (QGraphicsItem *child : childItems()) {
        if (Pad *pad = qgraphicsitem_cast<Pad *>(child)) {
            QRectF pad_rect = pad->boundingRect();
            QPointF pad_pos = pad->pos();
            pad_rect.translate(pad_pos);
            qDebug() << "Pad rect:" << pad_rect << "contains:" << pad_rect.contains(local_pos);
            if (pad_rect.contains(local_pos)) {
                pad->setHover(true);
                return;
            }
        }
    }
    QGraphicsItem::hoverEnterEvent(event);
}

void NodeGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    for (QGraphicsItem *child : childItems()) {
        if (Pad *pad = qgraphicsitem_cast<Pad *>(child)) {
            pad->setHover(false);
        }
    }
    QGraphicsItem::hoverLeaveEvent(event);
}

QRectF NodeGraphicsItem::boundingRect() const
{
    // Calculate actual bounds based on frame geometry and pad areas
    qreal left = -PAD_SPACING; // Account for left pads
    qreal top = 0;
    qreal width = m_frame_geometry.width() + PAD_SPACING * 2; // Account for both pad areas
    qreal height = m_frame_geometry.height();

    return QRectF(left, top, width, height);
}