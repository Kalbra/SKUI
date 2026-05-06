#pragma once

#include <QGraphicsView>
#include <QLoggingCategory>
#include <QMouseEvent>

#include "../node/node.hpp"
#include "graphicsitems/node_graphicsitem.hpp"
#include "nodeeditor_scene.hpp"

class NodeEditor : public QGraphicsView
{
    Q_OBJECT
public:
    explicit NodeEditor(QWidget *parent = nullptr);

    void addNode(Node *);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    NodeGraphicsItem *getTopLevelNode(QPoint pos);

    NodeEditorScene *m_scene;
    bool m_dragging = false;
    QPoint m_last_drag_pos;
};
