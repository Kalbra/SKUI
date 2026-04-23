#ifndef RESIZEBOUNDINGBOX_H
#define RESIZEBOUNDINGBOX_H

#include <QCursor>
#include <QMargins>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

static const int OUTER_HITBOX_SIZE = 10;
static const QMargins WIDGET_MARGIN(20, 20, 20, 20);
static const QMargins CENTER_HITBOX_MARGIN(15, 15, 15, 15);

enum class DragDirection {
    NorthWest,
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    Center,
    None,
};

class ResizeBoundingBox : public QWidget
{
    Q_OBJECT
public:
    explicit ResizeBoundingBox(QWidget *parent = nullptr);

    /* @brief Sets the size of bounding box.

    This method sets the size of the visable bounding box.
    The actual Widget has an offset, so that the mouse direction ankers are drawn correctly.

    @param box_geometry Geometry of the bounding box.
    */
    void setBoxGeometry(QRect box_geometry);

    /* @brief Sets the minimum size of the bounding box.
    
    This method sets the minimum size of the visable bounding box.
    The actual Widget has an offset, so that the mouse direction ankers are drawn correctly.

    @param size Minimum size of the bounding box.
    */
    void setBoxMinimumSize(QSize size);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

    void paintEvent(QPaintEvent *) override;

signals:
    void changedDelta(DragDirection, QPointF);

private:
    QRect box_geometry;

    QRect mapRectFromParent(QRect);
    static QRect squareByCenter(QPoint, int);
    QMap<DragDirection, QPoint> outerHitboxPositions();
    QMap<DragDirection, QRect> hitboxes();
    DragDirection containesHitbox(QPoint);
    void setHoverCursor(DragDirection);

    DragDirection active_direction = DragDirection::None;
    bool mouse_pressed = false;

    QPointF last_global_position;
    QRect last_geometry;
};

#endif // RESIZEBOUNDINGBOX_H
