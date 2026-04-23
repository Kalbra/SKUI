#ifndef CONTEXT_MENU_H
#define CONTEXT_MENU_H

#include <QMenu>

#include "../node/node.h"
#include "propertywindow.h"

class ContextMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ContextMenu(QWidget *parent = nullptr,
                         Node *node = nullptr,
                         const QPoint spawn_position = QPoint(0, 0));

private slots:
    void onProperties();
    void onRename();
    void onDelete();

private:
    void loadNodesToMenu(QMenu *menu);

    const QPoint m_spawn_position;
    Node *m_node = nullptr;
};

#endif // CONTEXT_MENU_H