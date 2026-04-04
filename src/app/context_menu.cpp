#include "context_menu.h"
#include "document.h"

ContextMenu::ContextMenu(QWidget *parent, const Node *node, const QPoint spawn_position)
    : QMenu{parent}
    , m_spawn_position{spawn_position}
{
    QAction *properties_action = new QAction("Properties", this);
    addAction(properties_action);

    QAction *rename_action = new QAction("Rename...", this);
    addAction(rename_action);

    QAction *delete_action = new QAction("Delete", this);
    addAction(delete_action);

    QMenu *add_node_menu = addMenu("Add Node");
    addMenu(add_node_menu);
    loadNodesToMenu(add_node_menu);

    exec(m_spawn_position);
}

void ContextMenu::loadNodesToMenu(QMenu *menu)
{
    for (const QString &node_name : Document::activeDocument()->availableNodes()) {
        QAction *node_action = new QAction(node_name, this);
        connect(node_action, &QAction::triggered, this, [this, node_name] {
            Document::activeDocument()->createNode(node_name, m_spawn_position);
        });
        menu->addAction(node_action);
    }
}
