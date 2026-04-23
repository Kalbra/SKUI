#ifndef CABLE_H
#define CABLE_H

#include <QCursor>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGuiApplication>
#include <QPainter>

#include "pad.h"

enum class DirectionalPlane { Horizontal, Vertical };
inline DirectionalPlane &operator!(DirectionalPlane &plane)
{
    return plane = plane == DirectionalPlane::Horizontal ? DirectionalPlane::Vertical
                                                         : DirectionalPlane::Horizontal;
}

struct CableSegment
{
    DirectionalPlane plane;
    QPoint start_point;
    QPoint end_point;
    // @TODO: Cable segments should also have a next and previous pointers and a mechansim for special
    // cases like connection split or the end of a cable. This would make it easier to manage the segments and their connections.
};

class Cable : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit Cable(QGraphicsItem *parent, const Pad *start_pad);

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;

    /** @brief Sets whether the Cable is editable.
     * 
     * This method sets wether the Cable is editable. 
     * If true, the Cable can be edited by the user, meaning that the user can move the corners and connect it to Pads. 
     * If false, the Cable is fixed and cannot be edited by the user.
     * 
     * The default value at construction is true.
     * 
     * Note: The bounding rect is the size of the scene in edit mode and will be shrink to the size of the Cable when not editable. 
     * @param editable Whether the Cable is editable.
     * 
     * @see Cable::isEditable()
     */
    void setEditable(bool editable);

    /** @brief Returns whether the Cable is editable.
     * 
     * This method returns whether the Cable is editable.
     * If true, the Cable can be edited by the user, meaning that the user can move the corners and connect it to Pads.
     * If false, the Cable is fixed and cannot be edited by the user.
     * 
     * @return Whether the Cable is editable.
     * 
     * @see Cable::setEditable(bool editable)
     */
    inline bool isEditable() const { return m_editable; }

protected:
    /** @brief Previews the current position of the cable.
     * 
     * This method renders a preview where the cable would be.
     * Due to the regulation that cables can only be horizontal or vertical,
     * the preview decides if it uses a horizontal or vertical based on
     * the angle to the latest corner. 
     * 
     * To enter a position, use corner(QPoint).
     * 
     * @param point The position to render the preview, normally the current mouse position.
     * 
     * @see Cable::corner(QPoint point)
     */
    void previewCable(QPoint point);

    /** @brief Locks a point as a new corner or moves the latest corner.
     * 
     * This method creates a new corner if the point is the oposite
     * direction. The point is at the position of the previewed cable.
     * 
     * @see Cable::previewCable(QPoint point)
     */
    void corner();

    /** @brief For the last corner before ending the Cable.
     * 
     * This method is for calling the last corner at the given point.
     * 
     * @param point The position where the cable ends.
     */
    void endCorner(QPoint point);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void updateStartCablePosition();
    void updateEndCablePosition();

private:
    void updateBoundingRect(QRectF new_rect);
    Pad *getPadAtScenePos(const QPointF &scene_pos) const;
    void initiateConnection(Pad *end_pad);

    QList<CableSegment> m_cable_segments;
    const Pad *m_start_pad = nullptr;
    Pad *m_end_pad = nullptr;
    Pad *m_hovered_pad = nullptr;
    bool m_editable = true;
    bool m_first_scene = true;
    // Do not access directly, use updateBoundingRect() to update repaint.
    QRectF m_bounding_rect;
};

#endif // CABLE_H