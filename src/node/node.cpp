#include "node.hpp"

QMap<QString, int> Node::instance_counter;

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

void Node::config()
{
    QString class_name = metaObject()->className();
    // Needed because every dirived class should have their own counter.
    // Using only a static counter whould share between the different dirived classes.
    instance_counter[class_name]++;

    // Example: "Label 1"
    QString instance_name = class_name + " " + QString::number(instance_counter[class_name]);
    setObjectName(instance_name);

    qDebug() << "Created Viusal <" << instance_name << ">";
}

void Node::setPositionHint(QPoint position_hint)
{
    if (m_position_hint == position_hint) {
        return;
    }

    m_position_hint = position_hint;
    emit positionHintChanged(position_hint);
}