#include "resizeboundingbox.hpp"

ResizeBoundingBox::ResizeBoundingBox(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
}

void ResizeBoundingBox::setBoxGeometry(QRect box_geometry)
{
    this->box_geometry = box_geometry;
    setGeometry(box_geometry.marginsAdded(WIDGET_MARGIN));
}

void ResizeBoundingBox::setBoxMinimumSize(QSize size)
{
    QSize margin_offset(WIDGET_MARGIN.left() + WIDGET_MARGIN.right(),
                        WIDGET_MARGIN.top() + WIDGET_MARGIN.bottom());

    setMinimumSize(size + margin_offset);
}

void ResizeBoundingBox::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::blue);
    painter.drawRect(mapRectFromParent(box_geometry));

    for (const QPoint center_positon : outerHitboxPositions()) {
        painter.fillRect(mapRectFromParent(squareByCenter(center_positon, 6)),
                         QBrush(Qt::green, Qt::SolidPattern));
    }

    // Debug: Show hitboxes (yellow) and geometry (red) of widget
    /*
    for (const QRect hitbox : hitboxes()) {
        painter.setPen(Qt::yellow);
        painter.drawRect(mapRectFromParent(hitbox));
    }
    painter.setPen(Qt::red);
    painter.drawRect(
        QRect(mapFromParent(geometry().topLeft()),
              geometry().size()
                  + QSize(-1,
                          -1) // Shiriking size by 1, so that the the Rect will render inside the widget
              ));*/
}

QRect ResizeBoundingBox::mapRectFromParent(QRect geometry)
{
    return QRect(mapFromParent(geometry.topLeft()), geometry.size());
}

QRect ResizeBoundingBox::squareByCenter(QPoint position, int length)
{
    return QRect(position.x() - length / 2, position.y() - length / 2, length, length);
}

QMap<DragDirection, QPoint> ResizeBoundingBox::outerHitboxPositions()
{
    return QMap<DragDirection, QPoint>(
        {{DragDirection::NorthWest, QPoint(box_geometry.topLeft())},
         {DragDirection::North,
          QPoint(box_geometry.left() + box_geometry.width() / 2, box_geometry.top())},
         {DragDirection::NorthEast, QPoint(box_geometry.topRight())},
         {DragDirection::East,
          QPoint(box_geometry.right(), box_geometry.top() + box_geometry.height() / 2)},
         {DragDirection::SouthEast, QPoint(box_geometry.bottomRight())},
         {DragDirection::South,
          QPoint(box_geometry.left() + box_geometry.width() / 2, box_geometry.bottom())},
         {DragDirection::SouthWest, QPoint(box_geometry.bottomLeft())},
         {DragDirection::West,
          QPoint(box_geometry.left(), box_geometry.top() + box_geometry.height() / 2)}});
}

QMap<DragDirection, QRect> ResizeBoundingBox::hitboxes()
{
    QMap<DragDirection, QRect> hitboxes;
    for (auto [direction, center_position] : outerHitboxPositions().asKeyValueRange()) {
        hitboxes[direction] = squareByCenter(center_position, OUTER_HITBOX_SIZE);
    }

    QRect center_hitbox = box_geometry.marginsRemoved(CENTER_HITBOX_MARGIN);
    hitboxes[DragDirection::Center] = center_hitbox;

    return hitboxes;
}

DragDirection ResizeBoundingBox::containesHitbox(QPoint position)
{
    for (auto [direction, hitbox] : hitboxes().asKeyValueRange()) {
        if (mapRectFromParent(hitbox).contains(position)) {
            return direction;
        }
    }
    return DragDirection::None;
}

void ResizeBoundingBox::setHoverCursor(DragDirection direction)
{
    switch (direction) {
    case DragDirection::NorthWest:
    case DragDirection::SouthEast:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case DragDirection::North:
    case DragDirection::South:
        setCursor(Qt::SizeVerCursor);
        break;
    case DragDirection::NorthEast:
    case DragDirection::SouthWest:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case DragDirection::West:
    case DragDirection::East:
        setCursor(Qt::SizeHorCursor);
        break;
    case DragDirection::Center:
        setCursor(Qt::OpenHandCursor);
        break;
    default: // DragDirection::None
        setCursor(Qt::ArrowCursor);
        break;
    }
}

void ResizeBoundingBox::mouseMoveEvent(QMouseEvent *event)
{
    if (mouse_pressed) {
        QPointF delta = event->globalPosition() - last_global_position;
        last_global_position = event->globalPosition();
        Q_EMIT changedDelta(active_direction, delta);
    }

    else { // On hover
        active_direction = containesHitbox(event->pos());
        setHoverCursor(active_direction);
    }
}

void ResizeBoundingBox::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        mouse_pressed = true;
        last_global_position = event->globalPosition();
        last_geometry = geometry();

        if (active_direction == DragDirection::Center) {
            setCursor(Qt::ClosedHandCursor);
        }
    }
}

void ResizeBoundingBox::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_pressed = false;
    mouseMoveEvent(event);
}
