#ifndef NODEEDITOR_SCENE_H
#define NODEEDITOR_SCENE_H

#include <QGraphicsScene>
#include <QGuiApplication>

#include "./graphicsitems/cable.h"
#include "./graphicsitems/node_graphicsitem.h"

class NodeEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit NodeEditorScene(QObject *parent = nullptr);

    void addNode(Node *);

    /** @brief Checks if currently a cable gets connected
     * 
     * This method returns weather a cable gets connected at the moment
     * or if the default behaviour takes place.
     * The connection state allows the user to route cables.
     * The best indicatior for this state by a user perspective is,
     * that the cursor will be overritten to a cross.
     */
    inline bool isCableConnectionState() { return m_active_cable; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;

private:
    void handlePadTrigger(Pad *);
    void handlePadHover(Pad *);
    void connectPads(Pad *first, Pad *second);
    void handleCornerTrigger(QPoint corner_pos);
    void disableCableConnectionState();
    void enableCableConnectionState(Pad *);
    void removePadHoverStates();

    int m_debug_x = 0;
    QList<Node *> m_nodes;
    QCursor m_cursor_before_pad;

    // nullptr if there if on initital pad yet
    Pad *m_initial_pad = nullptr;
    Cable *m_active_cable = nullptr;
};

#endif // NODEEDITOR_SCENE_H