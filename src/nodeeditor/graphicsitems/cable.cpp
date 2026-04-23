#include "cable.h"
#include "pad.h"

Cable::Cable(QGraphicsItem *parent, const Pad *start_pad)
    : QGraphicsObject{parent}
    , m_start_pad{start_pad}
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    // Delete the cable if the start pad is deleted.
    connect(m_start_pad, &QObject::destroyed, this, &Cable::deleteLater);

    connect(m_start_pad, &Pad::positionChanged, this, &Cable::updateStartCablePosition);

    // Init cable
    QPoint start_point = m_start_pad->getSceneDockPoint();
    m_cable_segments.append({DirectionalPlane::Vertical, start_point, start_point});
    m_cable_segments.append({DirectionalPlane::Horizontal, start_point, start_point});
}

void Cable::updateStartCablePosition()
{
    bool restore_enable = isEditable();
    setEditable(true);

    CableSegment &first_segment = m_cable_segments[0];
    CableSegment &second_segment = m_cable_segments[1];
    QPoint dock_point = m_start_pad->getSceneDockPoint();

    // Edit the first segment and the start of the second segment
    first_segment.start_point = dock_point;
    if (first_segment.plane == DirectionalPlane::Horizontal) {
        first_segment.end_point.setY(dock_point.y());
        second_segment.start_point = first_segment.end_point;
    } else { // first_segment.plane == DirectionalPlane::Vertical
        first_segment.end_point.setX(dock_point.x());
        second_segment.start_point = first_segment.end_point;
    }
    setEditable(restore_enable);

    update();
}

void Cable::updateEndCablePosition()
{
    if (!m_end_pad) {
        return;
    }

    bool restore_enable = isEditable();
    setEditable(true);

    CableSegment &last_segment = m_cable_segments.last();
    CableSegment &second_last_segment = m_cable_segments[m_cable_segments.size() - 2];
    QPoint dock_point = m_end_pad->getSceneDockPoint();

    // Edit the last segment and the end of the second last segment
    last_segment.end_point = dock_point;
    if (last_segment.plane == DirectionalPlane::Horizontal) {
        last_segment.start_point.setY(dock_point.y());
        second_last_segment.end_point = last_segment.start_point;
    } else { // last_segment.plane == DirectionalPlane::Vertical
        last_segment.start_point.setX(dock_point.x());
        second_last_segment.end_point = last_segment.start_point;
    }
    setEditable(restore_enable);

    update();
}

void Cable::setEditable(bool editable)
{
    m_editable = editable;
    if (m_editable) {
        if (scene()) {
            updateBoundingRect(scene()->sceneRect());
        }
        QGuiApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    } else {
        // Shrink the bounding rect to the size of the cable
        QRectF new_bounding_rect;
        for (const CableSegment &segment : m_cable_segments) {
            new_bounding_rect = new_bounding_rect.united(
                QRectF(segment.start_point, segment.end_point).normalized());
        }
        updateBoundingRect(new_bounding_rect);
        QGuiApplication::restoreOverrideCursor();
    }
}

void Cable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::red);
    painter->drawRect(boundingRect());

    painter->setPen(QPen(Qt::green, 3));

    for (const CableSegment &segment : m_cable_segments) {
        painter->drawLine(segment.start_point, segment.end_point);
    }
}

void Cable::updateBoundingRect(QRectF new_rect)
{
    m_bounding_rect = new_rect;
    prepareGeometryChange();
}

QRectF Cable::boundingRect() const
{
    return m_bounding_rect;
}

void Cable::previewCable(QPoint point)
{
    // Two segments are needed to render a cable. They are the preview cables
    if (m_cable_segments.size() < 2) {
        return;
    }

    // Preview Segements are part of the cable segments and counted backwards.
    CableSegment &first_preview_segment = m_cable_segments.last();
    CableSegment &second_preview_segment = m_cable_segments[m_cable_segments.size() - 2];

    if (first_preview_segment.plane == DirectionalPlane::Horizontal) {
        second_preview_segment.end_point.setY(point.y());
        first_preview_segment.start_point = second_preview_segment.end_point;
        first_preview_segment.end_point.setX(point.x());
        first_preview_segment.end_point.setY(point.y());
    } else { // first_preview_segment.plane == DirectionalPlane::Vertical
        second_preview_segment.end_point.setX(point.x());
        first_preview_segment.start_point = second_preview_segment.end_point;
        first_preview_segment.end_point.setX(point.x());
        first_preview_segment.end_point.setY(point.y());
    }

    update();
}

void Cable::corner()
{
    CableSegment &latest_segment = m_cable_segments.last();
    m_cable_segments.append(latest_segment);
    CableSegment &new_segment = m_cable_segments.last();
    new_segment.plane = !latest_segment.plane;
    update();
}

void Cable::endCorner(QPoint point)
{
    previewCable(point);
    update();
}

QVariant Cable::itemChange(GraphicsItemChange change, const QVariant &value)
{
    // The cable bouding rect cannot be calculated in the constructor, because the scene is not set yet.
    // Therefore we set the bounding rect via setEditable() when the scene is available.
    // The code is only run once, after a scene is set not when changing the scene.
    if (change == ItemSceneHasChanged && m_first_scene) {
        setEditable(true);
        m_first_scene = false;
    }

    return QGraphicsItem::itemChange(change, value);
}

Pad *Cable::getPadAtScenePos(const QPointF &scene_pos) const
{
    if (!scene()) {
        return nullptr;
    }

    for (QGraphicsItem *item : scene()->items(scene_pos)) {
        if (item == this) {
            continue;
        }
        if (Pad *pad = qgraphicsitem_cast<Pad *>(item)) {
            return pad;
        }
    }

    return nullptr;
}

void Cable::initiateConnection(Pad *current_pad)
{
    bool connected = m_start_pad->getInterface()->routeTo(current_pad->getInterface());
    if (connected) {
        m_end_pad = current_pad;
        connect(m_end_pad, &QObject::destroyed, this, &Cable::deleteLater);
        connect(m_end_pad, &Pad::positionChanged, this, &Cable::updateEndCablePosition);

        endCorner(m_end_pad->getSceneDockPoint());
        setEditable(false);
    }
}

void Cable::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_editable) {
        previewCable(event->pos().toPoint());
    }

    // Preview the hovered pads. This cant be handled by the Pad itself,
    // because the Pad has no information about wether the connection is valid.
    // Like if both are input pads and then do not preview the hover.
    Pad *pad_under_cursor = getPadAtScenePos(event->scenePos());
    if (m_hovered_pad != pad_under_cursor) {
        if (m_hovered_pad) {
            m_hovered_pad->setHover(false);
        }
        m_hovered_pad = pad_under_cursor;
        if (m_hovered_pad) {
            m_hovered_pad->setHover(true);
        }
    }

    QGraphicsItem::hoverMoveEvent(event);
}

void Cable::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_hovered_pad) {
        m_hovered_pad->setHover(false);
        m_hovered_pad = nullptr;
    }

    QGraphicsItem::hoverLeaveEvent(event);
}

void Cable::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        Pad *current_pad = getPadAtScenePos(event->scenePos());
        bool is_end_pad = current_pad && current_pad != m_start_pad;
        if (is_end_pad) {
            initiateConnection(current_pad);
        }
        // Only create a new corner if there is no pad.
        // For starting pad the corner is created at the constructor.
        if (!current_pad) {
            corner();
        }
    }

    QGraphicsItem::mousePressEvent(event);
}