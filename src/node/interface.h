#pragma once

#include <QDebug>
#include <QVariant>

enum class InterfaceDirection { Output, Input };

class Interface : public QVariant
{
public:
    Interface(QVariant &&, InterfaceDirection, QString);
    ~Interface();

    QString &getIdentifier() { return m_identifier; };
    const InterfaceDirection &getDirection() const { return m_interface_direction; };

    void update();
    void setCallback(std::function<void()> callback);

    bool routeTo(Interface *);
    void unrouteFrom(Interface *);

private:
    void updateRoutedInterfaces();

    QString m_identifier;
    const InterfaceDirection m_interface_direction;
    QList<Interface *> m_routed_interfaces;
    std::function<void()> m_callback;
};
