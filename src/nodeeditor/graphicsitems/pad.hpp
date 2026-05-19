#pragma once

#include <QCursor>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "node/interface.hpp"

static const int LINE_WIDTH = 3;
static const int PAD_SIZE = 10;
static const int PAD_EXPANDED = 12;
static const int LINE_LENGTH = 10;

enum class PadSide { Left, Right };

class Pad : public QGraphicsObject
{
    Q_OBJECT

public:
    static const int Type = QGraphicsItem::UserType + 1;

    explicit Pad(QGraphicsItem *parent = nullptr,
                 Interface *interface = nullptr,
                 const PadSide pad_side = PadSide::Left);

    /** @brief Get position for the cable to dock to
     * 
     * This method the position, where the cable starts/ends.
     * Normally the midpoint of the pad.
     * 
     * @return Point in scence coordinates
     */
    QPoint getSceneDockPoint() const;

    /** @brief Sets the hover state
     * 
     * This method set the hover state.
     * If true, there are visual differences to indicate
     * that the Pad is currently on hover.
     * If false, the Pad has the default appearance.
     * Note: This method does not sets the opservation of a hover,
     * but the hover itself.
     * 
     * @param hover State of the hover
     */
    void setHover(bool hover);

    inline PadSide getSide() const { return m_pad_side; }
    inline Interface *getInterface() const { return m_represented_interface; }
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;
    int type() const override { return Type; }

Q_SIGNALS:
    void positionChanged();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    bool m_hover = false;
    const PadSide m_pad_side;
    Interface *m_represented_interface;
};
