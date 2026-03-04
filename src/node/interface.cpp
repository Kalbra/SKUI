#include "interface.h"

Interface::Interface(QVariant &&value, InterfaceDirection interface_direction, QString identifier)
    : QVariant(value)
    , m_interface_direction(interface_direction)
    , m_identifier(identifier)
{
    qDebug() << m_identifier;
}

bool Interface::routeTo(Interface *target_interface)
{
    if (m_interface_direction == InterfaceDirection::Input) {
        return false; // Failed: Routing is done by output interfaces
    }
    qDebug() << m_identifier;
    //if (this->typeId() != target_interface->typeId()) {
    //    return false; // Failed: Interfaces not same type
    //}

    qDebug() << "Routing from Interface:" << m_identifier
             << "to Interface:" << target_interface->m_identifier;

    m_routed_interfaces.append(target_interface);
    target_interface->m_routed_interfaces.append(this);

    return true; // Success
}

void Interface::update()
{
    // Output Interfaces recive their update by the node after the value in the variant has changed.
    // Input Interfaces do not see the node of the output interfaces.
    // Therefore the Output Interface sends an update function call to the routed / connected input node.
    if (m_interface_direction == InterfaceDirection::Output) {
        updateRoutedInterfaces();
    } else { // m_interface_direction == InterfaceDirection::Input
        if (m_callback)
            m_callback(); // Call to process the new value
    }
}

void Interface::updateRoutedInterfaces()
{
    for (Interface *routed_interface : m_routed_interfaces) {
        routed_interface->setValue(*static_cast<QVariant *>(this));
        routed_interface->update();
    }
}

void Interface::setCallback(std::function<void()> callback)
{
    m_callback = callback;
}