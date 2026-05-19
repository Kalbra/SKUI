#pragma once

#include <QGuiApplication>
#include <QLoggingCategory>
#include <QMouseEvent>
#include <QRubberBand>
#include <QWidget>

#include "app/context_menu.hpp"
#include "node/nodes/visual.hpp"
#include "resizeboundingbox.hpp"
#include "visual_container.hpp"

enum class AlignDirection { Top, Right, Bottom, Left, Horizontal, Vertical };

enum class DisplayMode { Run, Edit };

static const int MAX_MOUSE_WIGGLE_TOLERANCE = 5; // px of size

using RbbWidgetPair = std::pair<ResizeBoundingBox *, QWidget *>;
class Panel : public QWidget
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = nullptr);

    /** @brief Adds a Visual to the Panel.
     * 
     * This method adds the given Visual element to the Panel.
     * By doing so, it also creates a VisualContainer for the Visual and sets the Visual's parent to the container.
     * 
     * @param visual The Visual element to be added.
     * @see Visual
     */
    void addVisual(Visual *visual);

public Q_SLOTS:
    /** @brief Sets the display mode of the panel.
     * 
     * This method changes the display mode of the panel to either Run or Edit.
     * In Edit mode, users can select and manipulate visual elements within the panel.
     * In Run mode, the panel behaves as a standard display without editing capabilities.
     * 
     * @param display_mode The desired display mode (Run or Edit).
    */
    void setMode(DisplayMode display_mode);

    /** @brief Align selected to direction.
     * 
     * This method aligns selected children and aligns them to a direction. 
     * 
     * @param direction Direction to align to. 
    */
    void triggeredAlign(AlignDirection direction);

    void changeGeometryForSelected(DragDirection, QPointF);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

    bool eventFilter(QObject *, QEvent *) override;
    void childEvent(QChildEvent *) override;

private:
    /**
     * @brief Makes the QWidget as selected.
     * 
     * This method adds the given QWidget to the list of selected visuals.
     * It also spawns the resize bounding box for the QWidget.
     * 
     * @param child The QWidget to be selected.
     */
    void select(QWidget *child);

    /**
     * @brief Deselects all selected QWidgets.
     * 
     * This method clears the list of selected visuals and deletes their
     * resize bounding boxes.
     */
    void deselectAll();

    /**
     * @brief Finds all QWidgets children within a given QRect.
     * 
     * This method returns a list of QWidgets that intersect with the specified QRect.
     * 
     * @param section The rectangle to check for intersecting QWidgets.
     * @return A list of QWidgets that intersect with the given QRect.
     * @see Panel::childAt(QPoint)
     */
    QList<QWidget *> childIn(QRect section);

    /**
     * @brief Finds a QWidget child at a given QPoint.
     * 
     * This method returns the QWidget located at the specified QPoint.
     * 
     * @param point The point to check for a QWidget.
     * @return The QWidget located at the given QPoint, or nullptr if none found.
     * @see Panel::childIn(QRect)
     */
    QWidget *childAt(QPoint point);

    /**
     * @brief Checks if mouse movement is within wiggle tolerance.
     * 
     * This method determines if the mouse movement from the origin point
     * is within the defined wiggle tolerance. 
     * This is useful for distinguishing between a click and a drag action.
     * 
     * @param size The size representing the mouse movement delta.
     * @return True if the movement is within wiggle tolerance, false otherwise.
     */
    bool inMouseWiggleTolerance(QSize size);

    /**
     * @brief Selects a single QWidget with Shift key check.
     * 
     * This method selects a single QWidget. If the Shift key is pressed,
     * it adds the QWidget to the current selection for multi-selection.
     * Else, it clears the current selection and selects only the given QWidget.
     * 
     * @param position The point where the selection is made.
     * @see Panel::select(QWidget*)
     */
    void selectSingle(QPoint position);

    /**
     * @brief Selects multiple QWidgets.
     * 
     * This method selects all QWidgets that intersect with the specified QRect.
     * It supports multi-selection if the Shift key is pressed.
     * 
     * @param found_visuals The list of visuals to select.
     * @see Panel::select(QWidget*)
     */
    void selectMultiple(const QList<QWidget *> &found_visuals);

    void alignChildToMinRect(AlignDirection direction, RbbWidgetPair rbbw_pair, QRect minimal_rect);
    QRect minimalSelectedRect();

    void installEventFilterRecursively(QObject *);
    QMouseEvent *mapMouseEventToPanel(QMouseEvent *, QWidget *);

    /**
     * @brief Calculates a new geometry.
     * 
     * This method calculates the new geometry based of the old one and the delta.
     * 
     * @param delta Change in geometry.
     * @param old_geometry Geomtry before appling the delta.
     * @param active_direction Direction to drag to.
     * @param min_w Minimum width that cannot be undercut by the new geometry.
     * @param min_h Minimum height that cannot be undercut by the new geometry.
     */
    QRect calculateNewGeometry(
        QPointF delta, QRect old_geometry, DragDirection active_direction, int min_w, int min_h);

    QPoint m_origin;
    QPointF m_last_global_position;
    bool m_mouse_pressed = false;
    QRubberBand *m_rubber_band;
    DisplayMode m_display_mode;

    QList<RbbWidgetPair> m_selection;
};
