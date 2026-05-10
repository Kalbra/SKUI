#include "interface.hpp"

Interface::Interface(QVariant &&value, InterfaceDirection interface_direction, QString identifier)
    : QVariant(value)
    , m_interface_direction(interface_direction)
    , m_identifier(identifier)
{
    qDebug() << m_identifier;
}

Interface::~Interface()
{
    for (Interface *routed_interface : m_routed_interfaces) {
        // Remove this interface from the routed interfaces of the connected interfaces to avoid dangling pointers.
        routed_interface->m_routed_interfaces.removeAll(this);
    }
}

bool Interface::routeTo(Interface *target_interface)
{
    Q_ASSERT(target_interface);

    if (target_interface == this) {
        return false; // Failed: Cant route to itself
    }

    if (target_interface->m_interface_direction == m_interface_direction) {
        return false; // Failed: Cant route to same direction
    }

    if (m_interface_direction == InterfaceDirection::Input) {
        // Route the other way around, because the output interface should update the input interface and not the other way around.
        return target_interface->routeTo(this);
    }

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