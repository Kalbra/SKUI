#include "cable.h"

Cable::Cable(QGraphicsItem *parent, QPoint initial_point)
    : QGraphicsItem{parent}
{
    m_current_preview_points = {initial_point, initial_point};
    m_corner.append(initial_point);
}

void Cable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::red);
    painter->drawRect(boundingRect());

    painter->setPen(QPen(Qt::green, 3));

    for (int i = 0; i < m_corner.length(); i++) {
        QPoint corner = m_corner[i];

        // Choose if the next item in the list is used or if its
        // at the end, use the preview.
        // Note: What we do here is dangerous. We might index out, if the condition is not correct
        int i_next = i + 1;
        if (i_next < m_corner.length()) { // Render corners
            painter->drawLine(corner, m_corner[i_next]);
        } else { // Render preview corners
            painter->drawLine(corner, m_current_preview_points.first);
            painter->drawLine(m_current_preview_points.first, m_current_preview_points.second);
        }
    }
}

QRectF Cable::boundingRect() const
{
    return QRectF(0, 0, 3000, 3000);
}

void Cable::previewCable(QPoint point)
{
    m_current_preview_points = getRestrictedCablePath(m_corner.last(), point);
    update();
}

void Cable::corner(QPoint point)
{
    std::pair<QPoint, QPoint> restriced_points = getRestrictedCablePath(m_corner.last(), point);
    m_corner.append(restriced_points.first);
    m_corner.append(restriced_points.second);
    switchPlane();
    update();
}

// Returns two preview points, one for x and one for y.
// We need this because lines must be vertical or horizontal.
// So to get to a target point we first have to go vertical and than horizontal
// or the other way around. m_current_plane referce to the last point
// or the point that is at the arguments given position.
// Which is normally the mouse position.
const std::pair<QPoint, QPoint> Cable::getRestrictedCablePath(const QPoint &start_point,
                                                              const QPoint &end_point)
{
    QPoint first_point;

    if (m_current_plane == DirectionalPlane::Vertical) {
        first_point.setX(end_point.x());
        //Lock y-axis for first point
        first_point.setY(start_point.y());
    } else { // m_current_plane == DirectionalPlane::Horizontal
        // Locking x-axis
        first_point.setX(start_point.x());
        first_point.setY(end_point.y());
    }
    return {first_point, end_point};
}

void Cable::switchPlane()
{
    if (m_current_plane == DirectionalPlane::Horizontal) {
        m_current_plane = DirectionalPlane::Vertical;
    } else { // m_current_plane == DirectionalPlane::Vertical
        m_current_plane = DirectionalPlane::Horizontal;
    }
}