#include "nodeeditor_scene.h"
#include "graphicsitems/pad.h"

NodeEditorScene::NodeEditorScene(QObject *parent)
    : QGraphicsScene{parent}
{
    setBackgroundBrush(Qt::white);
}

void NodeEditorScene::addNode(Node *node)
{
    m_nodes.append(node);
    m_debug_x = m_debug_x + 200;
    NodeGraphicsItem *node_graphicitem = new NodeGraphicsItem(nullptr, node);
    node_graphicitem->setPos(m_debug_x, 100);
    addItem(node_graphicitem);
}

void NodeEditorScene::enableCableConnectionState(Pad *pad)
{
    m_initial_pad = pad;
    m_active_cable = new Cable(nullptr, pad->getSceneDockPoint());
    addItem(m_active_cable);
    QGuiApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
}

void NodeEditorScene::disableCableConnectionState()
{
    m_active_cable = nullptr;
    QGuiApplication::restoreOverrideCursor();
}

void NodeEditorScene::handlePadTrigger(Pad *pad)
{
    if (m_active_cable) {
        bool different_sides = m_initial_pad->getSide() != pad->getSide();
        if (different_sides) {
            m_active_cable->corner(pad->getSceneDockPoint());
            connectPads(m_initial_pad, pad);
            disableCableConnectionState();
        }
    } else {
        enableCableConnectionState(pad);
    }
}

void NodeEditorScene::connectPads(Pad *first, Pad *second)
{
    qDebug() << "Routing init" << first << second->getInterface();
    first->getInterface()->routeTo(second->getInterface());
}

void NodeEditorScene::handleCornerTrigger(QPoint corner_pos)
{
    if (m_active_cable) {
        m_active_cable->corner(corner_pos);
    }
}

void NodeEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QList<QGraphicsItem *> found_items = items(event->scenePos());
        for (QGraphicsItem *item : found_items) {
            if (Pad *pad = qgraphicsitem_cast<Pad *>(item)) {
                handlePadTrigger(pad);
                event->accept();
                return;
            }
        }
        handleCornerTrigger(event->scenePos().toPoint());
        event->accept();
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

void NodeEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF scene_pos = event->scenePos();
    QList<QGraphicsItem *> items_at_pos = items(scene_pos);

    for (QGraphicsItem *item : items_at_pos) {
        if (Pad *pad = qgraphicsitem_cast<Pad *>(item)) {
            pad->setHover(true);
            for (QGraphicsItem *other : items_at_pos) {
                if (Pad *other_pad = qgraphicsitem_cast<Pad *>(other)) {
                    if (other_pad != pad) {
                        other_pad->setHover(false);
                    }
                }
            }
            return;
        }
    }

    for (QGraphicsItem *item : items()) {
        if (Pad *pad = qgraphicsitem_cast<Pad *>(item)) {
            pad->setHover(false);
        }
    }

    if (isCableConnectionState()) {
        m_active_cable->previewCable(event->scenePos().toPoint());
    }
}