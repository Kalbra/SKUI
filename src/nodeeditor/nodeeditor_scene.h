#ifndef NODEEDITOR_SCENE_H
#define NODEEDITOR_SCENE_H

#include <QGraphicsScene>
#include <QGuiApplication>
#include <QLoggingCategory>

#include "./graphicsitems/cable.h"
#include "./graphicsitems/node_graphicsitem.h"

class NodeEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit NodeEditorScene(QObject *parent = nullptr);

    void addNode(Node *node, QPoint relative_position_hint);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;

private:
    Pad *getPad(QPointF position); // Can be nullptr
    void finishCableConnection(Pad *pad);

    int m_debug_x = 0;
    QList<Node *> m_nodes;
    QCursor m_cursor_before_pad;
    Cable *m_last_cable = nullptr;
};

#endif // NODEEDITOR_SCENE_H