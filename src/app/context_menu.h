#ifndef CONTEXT_MENU_H
#define CONTEXT_MENU_H

#include <QMenu>

#include "../node/node.h"

class ContextMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ContextMenu(QWidget *parent = nullptr, const Node *node = nullptr, const QPoint spawn_position = QPoint(0, 0));

private:
    void loadNodesToMenu(QMenu *menu);

    const QPoint m_spawn_position;
};

#endif // CONTEXT_MENU_H