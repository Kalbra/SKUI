#include "document.hpp"

Q_DECLARE_LOGGING_CATEGORY(document)
Q_LOGGING_CATEGORY(document, "DOCUMENT")

Document *Document::s_active_document = nullptr;

Document::Document(QObject *parent)
    : QObject{parent}
{
    m_panel = new Panel;
    m_nodeeditor = new NodeEditor;
    s_active_document = this;
}

Document::~Document()
{
    delete m_panel;
    delete m_nodeeditor;
}

QList<QString> Document::availableNodes() const
{
    return m_node_factories.keys();
}

void Document::createNode(const QString &name, const QPoint position_hint)
{
    if (m_node_factories.contains(name)) {
        Node *node = m_node_factories[name]();
        node->setPositionHint(position_hint);
        m_nodeeditor->addNode(node);
        if (node->isVisual()) {
            m_panel->addVisual(static_cast<Visual *>(node));
        }
    } else {
        qWarning() << "No Node factory for name" << name << "found. Could not create node!";
    }
}

void Document::deleteNode(Node *node)
{
    if (!node) {
        qWarning() << "Cannot delete node: Node pointer is null.";
        return;
    }

    qDebug() << "Deleting node" << node->objectName();
    // @TODO: Remove node from node editor and panel. This should be done in a better way, maybe by emitting a signal from the node and let the node editor and panel handle the deletion.
    // The problem is getting the NodeGraphicsItem of the node to remove it and the VisualContainer and the ResizeBoundingBox of the visual node.
    // Also might remove the visual container
    node->deleteLater();
    //m_nodeeditor->removeNode(node);
    //if (node->isVisual()) {
    //    m_panel->removeVisual(static_cast<Visual *>(node));
    //}
}