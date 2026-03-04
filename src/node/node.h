#ifndef NODE_H
#define NODE_H

#include <QHash>
#include <QObject>
#include <QVariant>

#include "interface.h"

class Node : public QObject
{
    Q_OBJECT

public:
    explicit Node(QObject *parent);

    Q_PROPERTY(QString name MEMBER m_name)

    void setInterfaces(const QList<Interface> &interfaces) { m_interfaces = interfaces; }
    QList<Interface> &getInterfaces() { return m_interfaces; }
    Interface *getInterface(const QString &identifier);

signals:
    void changedInterfaceValue(const Interface &);

protected:
    /** @brief Set various configurations
     * 
     * This method sets the object name of instance.
     * It contains the class name and an instance counter.
     * The method also logs the creation of the object.
     */
    void config();

private:
    QString m_name;
    QWidget *m_nodeeditor = nullptr;

    QList<Interface> m_interfaces;
    static QMap<QString, int> instance_counter;
};

#endif // NODE_H