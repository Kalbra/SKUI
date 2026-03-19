#ifndef CABLE_H
#define CABLE_H

#include <QGraphicsItem>
#include <QPainter>

enum class DirectionalPlane { Horizontal, Vertical };

struct CableSegment {
    
};

class Cable : public QGraphicsItem
{
public:
    explicit Cable(QGraphicsItem *parent = nullptr, QPoint initial_point = QPoint());

    /** @brief Previews the current position of the cable.
     * 
     * This method renders a preview where the cable would be.
     * Due to the regulation that cables can only be horizontal or vertical,
     * the preview decides if it uses a horizontal or vertical based on
     * the angle to the latest corner. 
     * 
     * To enter a position, use setCorner(QPoint).
     * 
     * @param point The position to render the preview, normally the current mouse position.
     * 
     * @see Cable::corner(QPoint point)
     */
    void previewCable(QPoint point);

    /** @brief Appends a Cable to the given position.
     * 
     * This method appends the Cable from the last appended corner to the 
     * given position.
     * 
     * @param point 
     */
    void appendCable(QPoint point);

    /** @brief Locks a point as a new corner or moves the latest corner.
     * 
     * This method creates a new corner if the point is the oposite
     * direction. If not the latest corner will move to the 
     * specified point. See previewCable(QPoint) for more info
     * about vertical and horizontal cables.
     * 
     * @param point The position for the corner to be, normally on a mouse click position.
     * 
     * @see Cable::previewCable(QPoint point)
     */
    void corner(QPoint point);

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;

private:
    const std::pair<QPoint, QPoint> getRestrictedCablePath(const QPoint &start_point, const QPoint &end_point);
    void switchPlane();

    QList<QPoint> m_corner;
    std::pair<QPoint, QPoint> m_current_preview_points;
    DirectionalPlane m_current_plane = DirectionalPlane::Horizontal;
};

#endif // CABLE_H