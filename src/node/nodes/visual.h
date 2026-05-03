#ifndef VISUAL_H
#define VISUAL_H

#include <QLoggingCategory>

#include "../../panel/visual_container.h"
#include "../node.h"

class Visual : public Node
{
    Q_OBJECT
    Q_PROPERTY(QWidget *widget READ widget WRITE setWidget NOTIFY widgetChanged)

public:
    explicit Visual(QObject *parent = nullptr);
    virtual ~Visual() = default;

    /** @brief Gets the widget associated with this visual node.
     * 
     * This method returns the QWidget that is associated with this visual node.
     * The QWidget is a VisualContainer and is managed by the panel.
     * 
     * @return The QWidget associated with this visual node.
     * @see setWidget()
     * @see VisualContainer
     * @see Panel::addVisual()
     */
    QWidget *widget() const { return m_widget; }

    /** @brief Sets the widget associated with this visual node.
     * 
     * This method sets the QWidget that is associated with this visual node.
     * The QWidget is a VisualContainer and is managed by the panel.
     * 
     * @param widget The QWidget to associate with this visual node.
     * @see widget()
     * @see VisualContainer
     * @see Panel::addVisual()
     */
    void setWidget(QWidget *widget);

    /** @brief Paints the visual node onto a VisualContainer.
     * 
     * This pure virtual method creates and returns a QWidget that represents the visual node.
     * It is the entry point for rendering content to the panel.
     * The QWidget is a child of the given VisualContainer and is styled to visually represent the node.
     * 
     * @param parent The VisualContainer to which the visual node's widget will be added as a child.
     * @return A QWidget that visually represents the node, styled and ready to be displayed within the panel.
     * @see Panel
     * @see VisualContainer
     */
    virtual QWidget *paintWidget(VisualContainer *parent) = 0;

    /** @brief Checks if the node is a visual node
     * 
     * This method returns true.
     * 
     * @return True, since this is a visual node.
     * @see Node::isVisual()
     */
    bool isVisual() const override { return true; }

signals:
    /**
     * @see widget()
     * @see setWidget()
     */
    void widgetChanged(QWidget *widget);

private:
    QWidget *m_widget = nullptr;
};

#endif // VISUAL_H