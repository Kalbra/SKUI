#include "nodeeditor.h"

Q_DECLARE_LOGGING_CATEGORY(ne_view)
Q_LOGGING_CATEGORY(ne_view, "NE_VIEW")

NodeEditor::NodeEditor(QWidget *parent)
    : QGraphicsView{parent}
{
    setMouseTracking(true);

    // @TODO: Might delete the variable and use scene() method instead
    // For this addNode must be moved from the Scene to here
    m_scene = new NodeEditorScene(parent);
    setScene(m_scene);
    setSceneRect(0, 0, 3000, 1000);
    centerOn(0, 0);

    // setDragMode(DragMode::ScrollHandDrag);
    setResizeAnchor(ViewportAnchor::AnchorUnderMouse);
}

void NodeEditor::addNode(Node *node)
{
    m_scene->addNode(node);
}

void NodeEditor::mousePressEvent(QMouseEvent *event)
{
    bool shift_down = QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier);
    bool some_already_selected = !scene()->selectedItems().isEmpty();
    bool deselect_others = some_already_selected && !shift_down;

    if (event->button() == Qt::LeftButton) {
        if (deselect_others) {
            scene()->clearSelection();
        }

        // We need to use the items method and cannot use itemAt() because
        // the top level item might be the active cable, because
        // Cabel have a maximum bounding box. This is because they can route "everywhere"
        for (QGraphicsItem *item : items(event->pos())) {
            if (NodeGraphicsItem *node = qgraphicsitem_cast<NodeGraphicsItem *>(item)) {
                m_dragging = true;
                m_last_drag_pos = event->pos();
                node->setSelected(true);
                return;
            }
        }
        scene()->clearSelection();
    }

    event->ignore();
    QGraphicsView::mousePressEvent(event);
}

void NodeEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_dragging) {
        return QGraphicsView::mouseMoveEvent(event);
    }

    QPoint delta = event->pos() - m_last_drag_pos;

    for (QGraphicsItem *item : items()) {
        if (item->isSelected()) {
            item->setPos(item->pos() + delta);
        }
    }

    m_last_drag_pos = event->pos();
}

void NodeEditor::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_dragging = false;
}