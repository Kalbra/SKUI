#include "nodeeditor_scene.hpp"
#include "graphicsitems/pad.hpp"

Q_DECLARE_LOGGING_CATEGORY(ne_scence)
Q_LOGGING_CATEGORY(ne_scence, "NE_SCENCE")

NodeEditorScene::NodeEditorScene(QObject *parent)
    : QGraphicsScene{parent}
{
    setSceneRect(-1000, -1000, 2000, 2000);
    setBackgroundBrush(Qt::white);
}

void NodeEditorScene::addNode(Node *node, QPoint relative_position_hint)
{
    Q_ASSERT(node);

    m_nodes.append(node);
    m_debug_x = m_debug_x + 200;
    NodeGraphicsItem *node_graphicitem = new NodeGraphicsItem(nullptr, node);
    node_graphicitem->setFlag(QGraphicsItem::ItemIsMovable, true);
    node_graphicitem->setPos(relative_position_hint);
    addItem(node_graphicitem);
}

Pad *NodeEditorScene::getPad(QPointF position)
{
    // We need to use the items method and cannot use itemAt() because
    // the top level item might be the active cable, because
    // Cabel have a maximum bounding box. This is because they can route "everywhere"
    for (QGraphicsItem *item : items(position.toPoint())) {
        if (Pad *pad = qgraphicsitem_cast<Pad *>(item)) {
            return pad;
        }
    }
    return nullptr;
}

void NodeEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    bool left_click = event->button() == Qt::LeftButton;
    bool last_active = false;

    if (m_last_cable) {
        last_active = m_last_cable->isEditable();
    }

    if (left_click && !last_active) {
        if (Pad *pad = getPad(event->scenePos())) {
            m_last_cable = new Cable(nullptr, pad);
            addItem(m_last_cable);
        }
    }

    QGraphicsScene::mousePressEvent(event);
}