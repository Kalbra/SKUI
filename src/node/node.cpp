#include "node.h"

Node::Node(QObject *parent)
    : QObject{parent}
{}

Interface *Node::getInterface(const QString &identifier)
{
    for (Interface &interface : m_interfaces) {
        if (interface.getIdentifier() == identifier) {
            return &interface;
        }
    }

    return nullptr; // No interface found with the given identifier
}