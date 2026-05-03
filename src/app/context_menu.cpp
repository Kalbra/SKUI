#include "context_menu.h"
#include "document.h"
#include "rename_node_dialog.h"

ContextMenu::ContextMenu(QWidget *parent, Node *node, const QPoint spawn_position)
    : QMenu{parent}
    , m_node{node}
    , m_spawn_position{spawn_position}
{
    QAction *properties_action = new QAction("Properties", this);
    connect(properties_action, &QAction::triggered, this, &ContextMenu::onProperties);
    QAction *rename_action = new QAction("Rename...", this);
    connect(rename_action, &QAction::triggered, this, &ContextMenu::onRename);
    QAction *delete_action = new QAction("Delete", this);
    connect(delete_action, &QAction::triggered, this, &ContextMenu::onDelete);

    // These properties are only enabled if a node is right-clicked.
    if (!node) {
        properties_action->setEnabled(false);
        rename_action->setEnabled(false);
        delete_action->setEnabled(false);
    }

    addActions({properties_action, rename_action, delete_action});

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

void ContextMenu::onProperties()
{
    PropertyWindow *pw = new PropertyWindow(parentWidget(), m_node);
    pw->show();
}

void ContextMenu::onRename()
{
    if (!m_node) {
        return;
    }

    RenameNodeDialog rename_dialog(m_node->objectName(), parentWidget());
    if (rename_dialog.exec() != QDialog::Accepted) {
        return;
    }

    m_node->setObjectName(rename_dialog.nodeName());
    //m_node->setProperty("objectName", rename_dialog.nodeName());
}

void ContextMenu::onDelete()
{
    if (!m_node) {
        return;
    }

    Document::activeDocument()->deleteNode(m_node);
}