#include "nodeeditor_scene.h"
#include "graphicsitems/pad.h"

Q_DECLARE_LOGGING_CATEGORY(ne_scence)
Q_LOGGING_CATEGORY(ne_scence, "NE_SCENCE")

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
    node_graphicitem->setFlag(QGraphicsItem::ItemIsMovable, true);
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

void NodeEditorScene::connectPads(Pad *first, Pad *second)
{
    qDebug(ne_scence) << "Routing init" << first << second->getInterface();
    first->getInterface()->routeTo(second->getInterface());
}

void NodeEditorScene::handleCornerTrigger(QPoint corner_pos)
{
    if (m_active_cable) {
        m_active_cable->corner(corner_pos);
    }
}

void NodeEditorScene::removePadHoverStates()
{
    for (QGraphicsItem *item : items()) {
        if (Pad *pad = qgraphicsitem_cast<Pad *>(item)) {
            pad->setHover(false);
        }
    }
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

void NodeEditorScene::finishCableConnection(Pad *pad)
{
    bool different_sides = m_initial_pad->getSide() != pad->getSide();
    if (different_sides) {
        m_active_cable->corner(pad->getSceneDockPoint());
        connectPads(m_initial_pad, pad);
        disableCableConnectionState();
    }
}

void NodeEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    bool left_click = event->button() == Qt::LeftButton;

    if (left_click && !m_active_cable) {
        if (Pad *pad = getPad(event->scenePos())) {
            enableCableConnectionState(pad);
        }
    }

    if (left_click && m_active_cable) {
        if (Pad *pad = getPad(event->scenePos())) {
            finishCableConnection(pad);
        } else {
            handleCornerTrigger(event->scenePos().toPoint());
        }
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

void NodeEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Hovering of Pads is handled by the scence and not the Pad itself.
    // We do that because we want to Pad NOT to hover if there is an Pad of the same type
    // (e.g. input <-> input) the Pad has no information about that.
    // Therefore its handled here.

    // Set all pads to no hover as it is the default value
    removePadHoverStates();

    if (isCableConnectionState()) {
        // previewCable might be overwritten later!
        m_active_cable->previewCable(event->scenePos().toPoint());
    }

    if (Pad *pad = getPad(event->scenePos())) {
        pad->setHover(true);
        if (isCableConnectionState()) {
            m_active_cable->previewCable(pad->getSceneDockPoint());
        }
    }

    QGraphicsScene::mouseMoveEvent(event);
}
