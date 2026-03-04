#ifndef NODE_GRAPHICSITEM_H
#define NODE_GRAPHICSITEM_H

#include <QFontMetrics>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMargins>
#include <QPainter>
#include <QPen>
#include <QPointF>

#include "../../node/node.h"
#include "pad.h"

static const QMargins BOX_MARGIN(7, 7, 7, 7);
static const QColor BOX_COLOR = QColorConstants::Svg::midnightblue;
static const QColor INNER_COLOR = QColorConstants::Svg::mistyrose;
static const int BOX_WIDTH = 2;
static const int FONT_SIZE = 15; // points
//static const int PAD_SIZE = 5;
static const int PAD_SPACING = 20;

class NodeGraphicsItem : public QGraphicsItem
{
public:
    static const int Type = QGraphicsItem::UserType + 2;

    explicit NodeGraphicsItem(QGraphicsItem *parent = nullptr, Node *node = nullptr);

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;
    int type() const override { return Type; }

    /** @brief Gets the node represented by the graphicsitem
     * 
     * This method gets the node that the NodeGrpahicsItem represents.
     * 
     * @return Pointer to the node
     * @see Node::Node
     */
    inline const Node *getNode() { return m_node; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void drawOuterFrame(QPainter *painter);
    void createPads(QRect frame_geometry);

    Node *m_node;
    QRect m_frame_geometry;
    QRect m_node_name_geometry;
    QFont m_font;
    QString m_object_name;
    QPointF m_drag_start_pos;
    bool m_is_dragging = false;
};

#endif // NODE_GRAPHICSITEM_H