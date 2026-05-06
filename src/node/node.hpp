#pragma once

#include <QHash>
#include <QObject>
#include <QPoint>
#include <QVariant>

#include "interface.hpp"

class Node : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name)
    Q_PROPERTY(QPoint position_hint READ positionHint WRITE setPositionHint NOTIFY
                   positionHintChanged MEMBER m_position_hint)

public:
    explicit Node(QObject *parent);

    void setInterfaces(const QList<Interface> &interfaces) { m_interfaces = interfaces; }
    /** @brief Get the list of interfaces
     *
     * This method returns the reference to the interfaces of the node.
     *
     * @return A reference to the list of interfaces.
     * @see getInterface()
     * @see Interface
     */
    QList<Interface> &getInterfaces() { return m_interfaces; }

    /** @brief Get an interface by its identifier
     *
     * This method returns a pointer to the interface with the given identifier, or nullptr if no such interface exists.
     *
     * @param identifier The identifier of the interface to retrieve.
     * @return A pointer to the interface with the given identifier, or nullptr if no such interface exists.
     * @see setInterfaces()
     * @see Interface
     */
    Interface *getInterface(const QString &identifier);

    /** @brief Checks if the node is a visual node
     *
     * This method returns true if the node is a visual node, false otherwise.
     * Nodes are no visual nodes by default, so this method is overridden by the visual base class.
     *
     * @return True if the node is a visual node, false otherwise.
     * @see Visual
     */
    const virtual bool isVisual() const { return false; }

    /** @brief Get the position hint
     *
     * This property is used to store the position hint of the node. 
     * It is used to position the node in the node editor or panel.
     *
     * @return The position hint of the node.
     * @see setPositionHint()
     */
    QPoint positionHint() const { return m_position_hint; }

    /** @brief Set the position hint
     *
     * This property is used to store the position hint of the node.
     * It is used to position the node in the node editor or panel.
     *
     * @param position_hint The position hint of the node.
     * @see positionHint()
     */
    void setPositionHint(QPoint position_hint);

signals:
    /**
     * @see getInterface()
     * @see Interface
     */
    void changedInterfaceValue(const Interface &);

    /**
     * @see setPositionHint()
     * @see positionHint()
     */
    void positionHintChanged(const QPoint position_hint);

protected:
    /** @brief Set various configurations
     * 
     * This method sets the object name of instance.
     * It contains the class name and an instance counter.
     * The method also logs the creation of the object.
     */
    void config();

private:
    QWidget *m_nodeeditor = nullptr;

    // Properties
    QString m_name = "Undefined";
    QPoint m_position_hint = QPoint(0, 0);

    QList<Interface> m_interfaces;
    static QMap<QString, int> instance_counter;
};
