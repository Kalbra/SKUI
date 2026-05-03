#include "node_graphicsitem.h"
#include <QGraphicsScene>

NodeGraphicsItem::NodeGraphicsItem(QGraphicsItem *parent, Node *node)
    : QGraphicsObject{parent}
    , m_node(node)
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    QFont font = QFont();
    font.setBold(true);
    font.setPointSize(FONT_SIZE);
    m_font = font;

    m_object_name = m_node->objectName();
    m_node_name_geometry = QFontMetrics(m_font).boundingRect(m_object_name);
    int rect_width = m_node_name_geometry.width() + BOX_WIDTH * 2 + BOX_MARGIN.right() * 2;
    m_frame_geometry = QRect(PAD_SPACING, 0, rect_width, 200);

    m_node_connections.append(connect(m_node, &QObject::destroyed, this, [this]() {
        scene()->removeItem(this);
        delete this;
    }));

    m_node_connections.append(
        connect(m_node, &QObject::objectNameChanged, this, [this](const QString &object_name) {
            onNodeObjectNameChanged(object_name);
        }));

    createPads(m_frame_geometry);
}

void NodeGraphicsItem::onNodeObjectNameChanged(const QString &object_name)
{
    if (m_object_name == object_name) {
        return;
    }

    prepareGeometryChange();

    m_object_name = object_name;
    m_node_name_geometry = QFontMetrics(m_font).boundingRect(m_object_name);
    int rect_width = m_node_name_geometry.width() + BOX_WIDTH * 2 + BOX_MARGIN.right() * 2;
    m_frame_geometry.setWidth(rect_width);

    const int right_pad_x_pos = m_frame_geometry.right() + BOX_WIDTH * 2;
    for (QGraphicsItem *item : childItems()) {
        if (Pad *pad = qgraphicsitem_cast<Pad *>(item)) {
            if (pad->getSide() == PadSide::Right) {
                pad->setPos(right_pad_x_pos, pad->pos().y());
            }
        }
    }

    update();
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
    QColor used_box_color;
    if (isSelected()) {
        used_box_color = BOX_COLOR_AT_SELECTION;
    } else { // Normal case, no selection
        used_box_color = BOX_COLOR;
    }

    painter->setFont(m_font);
    QPen pen(used_box_color, BOX_WIDTH);
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

QRectF NodeGraphicsItem::boundingRect() const
{
    return m_frame_geometry;
}