#include "panel.hpp"

Q_DECLARE_LOGGING_CATEGORY(panel)
Q_LOGGING_CATEGORY(panel, "PANEL")

Panel::Panel(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);

    m_rubber_band = new QRubberBand(QRubberBand::Rectangle, this);
}

void Panel::addVisual(Visual *visual)
{
    Q_ASSERT(visual);
    
    QPoint mapped_position_hint = mapFromGlobal(visual->positionHint());
    VisualContainer *container = new VisualContainer(this);
    container->setGeometry(QRect(mapped_position_hint, QSize(100, 100)));
    container->setNode(visual);
    QWidget *visual_widget = visual->paintWidget(container);
    visual->setWidget(visual_widget);
    visual_widget->show();
    container->show();

    // @TODO: This is a quick and dirty solution to deselect the visual when it is deleted.
    // This should be done in a better way. Should be done at the rrbw refactor.
    connect(visual, &QObject::destroyed, this, [this]() { deselectAll(); });
    connect(visual, &QObject::destroyed, container, &QObject::deleteLater);
}

QList<QWidget *> Panel::childIn(QRect section)
{
    QList<QWidget *> contained_widgets;

    for (QWidget *child : findChildren<VisualContainer *>(Qt::FindDirectChildrenOnly)) {
        if (section.intersects(child->geometry()))
            contained_widgets.append(child);
    }

    return contained_widgets;
}

QWidget *Panel::childAt(QPoint point)
{
    QWidget *top_level_widget = nullptr;

    for (QWidget *child : findChildren<VisualContainer *>(Qt::FindDirectChildrenOnly)) {
        if (child->geometry().contains(point)) {
            top_level_widget = child;
        }
    }

    return top_level_widget;
}

void Panel::setMode(DisplayMode display_mode)
{
    m_display_mode = display_mode;
}

void Panel::triggeredAlign(AlignDirection direction)
{
    //@TODO Proper alignment of selected QWidgets
    QRect minimal_rect = minimalSelectedRect();

    for (const RbbWidgetPair pair : m_selection) {
        alignChildToMinRect(direction, pair, minimal_rect);
    }
}

QRect Panel::minimalSelectedRect()
{
    int top = INT_MAX;
    int left = INT_MAX;
    int bottom = INT_MIN;
    int right = INT_MIN;

    for (const RbbWidgetPair pair : m_selection) {
        QRect rect = pair.second->geometry();

        top = std::min(top, rect.top());
        left = std::min(left, rect.left());
        bottom = std::max(bottom, rect.bottom());
        right = std::max(right, rect.right());
    }

    return QRect(QPoint(left, top), QPoint(right, bottom));
}

void Panel::alignChildToMinRect(AlignDirection direction,
                                RbbWidgetPair rbbw_pair,
                                QRect minimal_rect)
{
    ResizeBoundingBox *rbb = rbbw_pair.first;
    QWidget *widget = rbbw_pair.second;
    QRect new_geometry = widget->geometry();

    switch (direction) {
    case AlignDirection::Top:
        new_geometry.moveTop(minimal_rect.top());
        break;
    case AlignDirection::Right:
        new_geometry.moveRight(minimal_rect.right());
        break;
    case AlignDirection::Bottom:
        new_geometry.moveBottom(minimal_rect.bottom());
        break;
    case AlignDirection::Left:
        new_geometry.moveLeft(minimal_rect.left());
        break;
    case AlignDirection::Horizontal:
        new_geometry.moveCenter(QPoint(minimal_rect.center().x(), new_geometry.center().y()));
        break;
    case AlignDirection::Vertical:
        new_geometry.moveCenter(QPoint(new_geometry.center().x(), minimal_rect.center().y()));
        break;
    default:
        return;
    }

    widget->setGeometry(new_geometry);
    rbb->setBoxGeometry(new_geometry);
}

void Panel::select(QWidget *child)
{
    ResizeBoundingBox *rbb = new ResizeBoundingBox(this);
    rbb->setBoxGeometry(child->geometry());
    rbb->show();
    connect(rbb, &ResizeBoundingBox::changedDelta, this, &Panel::changeGeometryForSelected);
    m_selection.append({rbb, child});
    //@TODO: Replace with actual name
    qInfo() << QString("Select: IMPLEMENT NAME");
}

void Panel::deselectAll()
{
    QMutableListIterator<RbbWidgetPair> i(m_selection);
    while (i.hasNext()) {
        ResizeBoundingBox *rbb = i.next().first;
        delete rbb;
        i.remove();
    }
    //@TODO: Replace with actual name
}

void Panel::selectSingle(QPoint position)
{
    QWidget *top_level_child = childAt(position);
    if (top_level_child) {
        bool shift_down = QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier);
        if (!shift_down) {
            deselectAll();
        }
        select(top_level_child);
    }
}

void Panel::selectMultiple(const QList<QWidget *> &found_widgets)
{
    for (QWidget *widget : found_widgets) {
        select(widget);
    }
}

void Panel::changeGeometryForSelected(DragDirection active_direction, QPointF delta)
{
    for (RbbWidgetPair select : m_selection) {
        ResizeBoundingBox *rbb = select.first;
        QWidget *widget = select.second;

        QRect new_geometry = calculateNewGeometry(delta,
                                                  widget->geometry(),
                                                  active_direction,
                                                  widget->minimumWidth(),
                                                  widget->minimumHeight());

        widget->setGeometry(new_geometry);
        rbb->setBoxGeometry(new_geometry);
    }
}

QRect Panel::calculateNewGeometry(
    QPointF delta, QRect old_geometry, DragDirection active_direction, int min_w, int min_h)
{
    int initial_x = old_geometry.x();
    int initial_y = old_geometry.y();
    int initial_w = old_geometry.width();
    int initial_h = old_geometry.height();

    int new_x = initial_x;
    int new_y = initial_y;
    int new_w = initial_w;
    int new_h = initial_h;

    switch (active_direction) {
    case DragDirection::North: // Fixed: Bottom
        new_y = initial_y + delta.y();
        new_h = initial_h - delta.y();
        break;
    case DragDirection::NorthEast: // Fixed: BottomLeft
        new_y = initial_y + delta.y();
        new_w = initial_w + delta.x();
        new_h = initial_h - delta.y();
        break;
    case DragDirection::East: // Fixed: Left
        new_w = initial_w + delta.x();
        break;
    case DragDirection::SouthEast: // Fixed: TopLeft
        new_w = initial_w + delta.x();
        new_h = initial_h + delta.y();
        break;
    case DragDirection::South: // Fixed: Top
        new_h = initial_h + delta.y();
        break;
    case DragDirection::SouthWest: // Fixed: TopRight
        new_x = initial_x + delta.x();
        new_w = initial_w - delta.x();
        new_h = initial_h + delta.y();
        break;
    case DragDirection::West: // Fixed: Right
        new_x = initial_x + delta.x();
        new_w = initial_w - delta.x();
        break;
    case DragDirection::NorthWest: // Fixed: BottomRight
        new_x = initial_x + delta.x();
        new_y = initial_y + delta.y();
        new_w = initial_w - delta.x();
        new_h = initial_h - delta.y();
        break;
    case DragDirection::Center:
        new_x = initial_x + delta.x();
        new_y = initial_y + delta.y();
        break;
    default: // DrageDirection::None
        break;
    }

    if (new_w < min_w) {
        new_w = initial_w;
        new_x = initial_x;
    }
    if (new_h < min_h) {
        new_h = initial_h;
        new_y = initial_y;
    }

    return QRect(new_x, new_y, new_w, new_h);
}

bool Panel::inMouseWiggleTolerance(QSize size)
{
    if (size.width() > MAX_MOUSE_WIGGLE_TOLERANCE || size.height() > MAX_MOUSE_WIGGLE_TOLERANCE) {
        return false;
    } else {
        return true;
    }
}

void Panel::mousePressEvent(QMouseEvent *event)
{
    bool edit_mode = m_display_mode == DisplayMode::Edit;
    bool left_button = event->button() == Qt::LeftButton;
    bool right_button = event->button() == Qt::RightButton;

    if (edit_mode && left_button) {
        m_origin = event->pos();
        m_rubber_band->setGeometry(QRect(m_origin, QSize(1, 1)));
        m_rubber_band->show();
        return;
    }

    if (edit_mode && right_button) {
        VisualContainer *container = qobject_cast<VisualContainer *>(childAt(event->pos()));
        Node *node = container ? container->node() : nullptr;
        ContextMenu context_menu(this, node, mapToGlobal(event->pos()));
        return;
    }

    QWidget::mousePressEvent(event);
}

void Panel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_display_mode == DisplayMode::Edit) {
        m_rubber_band->setGeometry(QRect(m_origin, event->pos()).normalized());
    }
}

void Panel::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_display_mode == DisplayMode::Edit) {
        m_rubber_band->hide();

        QList<QWidget *> found_visuals = childIn(m_rubber_band->geometry());
        if (found_visuals.isEmpty()) {
            deselectAll();
            return;
        }

        if (inMouseWiggleTolerance(m_rubber_band->size())) {
            selectSingle(event->pos());
        } else { // selction via rubber band
            selectMultiple(found_visuals);
        }
    }
}

void Panel::installEventFilterRecursively(QObject *obj)
{
    if (obj->isWidgetType()) {
        obj->installEventFilter(this);

        // Enable mouse tracking to receive mouse move events even when no button is pressed
        if (QWidget *widget = qobject_cast<QWidget *>(obj)) {
            widget->setMouseTracking(true);
        }
    }

    for (QObject *child : obj->children()) {
        installEventFilterRecursively(child);
    }
}

void Panel::childEvent(QChildEvent *event)
{
    if (event->type() == QEvent::ChildAdded) {
        installEventFilterRecursively(event->child());
    }
    QWidget::childEvent(event);
}

bool Panel::eventFilter(QObject *watched, QEvent *event)
{
    // Install event filter on any newly added QWidget child
    if (event->type() == QEvent::ChildAdded) {
        QChildEvent *child_event = static_cast<QChildEvent *>(event);
        if (child_event->child()->isWidgetType()) {
            installEventFilterRecursively(child_event->child());
        }
    }

    bool is_visual = qobject_cast<VisualContainer *>(watched);
    bool mouse_event = event->type() == QEvent::MouseButtonPress
                       || event->type() == QEvent::MouseMove
                       || event->type() == QEvent::MouseButtonRelease;
    bool forward_mouse_events = (m_display_mode == DisplayMode::Edit) && mouse_event && is_visual;
    if (forward_mouse_events) {
        //@TODO: Check coordinate system of event. It might be that the coordiante system
        //       of the event is relative to the child widget, not the panel.
        QMouseEvent *mapped_event = mapMouseEventToPanel(static_cast<QMouseEvent *>(event),
                                                         qobject_cast<QWidget *>(watched));

        switch (event->type()) {
        case QEvent::MouseButtonPress:
            mousePressEvent(mapped_event);
            delete mapped_event;
            return true;
        case QEvent::MouseMove:
            mouseMoveEvent(mapped_event);
            delete mapped_event;
            return true;
        case QEvent::MouseButtonRelease:
            mouseReleaseEvent(mapped_event);
            delete mapped_event;
            return true;
        default:
            delete mapped_event;
            break;
        }
        //@TODO: Crash on double click events
    }

    return QWidget::eventFilter(watched, event);
}

QMouseEvent *Panel::mapMouseEventToPanel(QMouseEvent *origin_event, QWidget *source_widget)
{
    QPoint new_pos = origin_event->pos();
    if (source_widget && source_widget != this) {
        new_pos = source_widget->mapTo(this, origin_event->pos());
    }

    QMouseEvent *mapped_event = new QMouseEvent(origin_event->type(),
                                                new_pos,
                                                origin_event->globalPosition(),
                                                origin_event->button(),
                                                origin_event->buttons(),
                                                origin_event->modifiers(),
                                                origin_event->pointingDevice());

    return mapped_event;
}