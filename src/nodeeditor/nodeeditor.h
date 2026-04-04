#ifndef NodeEditor_H
#define NodeEditor_H

#include <QGraphicsView>
#include <QLoggingCategory>
#include <QMouseEvent>

#include "../node/node.h"
#include "graphicsitems/node_graphicsitem.h"
#include "nodeeditor_scene.h"

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

#endif // NodeEditor_H