#include "visual.h"

Q_DECLARE_LOGGING_CATEGORY(visual)
Q_LOGGING_CATEGORY(visual, "VISUAL")


Visual::Visual(QObject *parent)
    : Node(parent)
{
    qDebug() << this->metaObject()->className();
}

void Visual::setPanel(QWidget *panel_widget, QPoint position)
{
    if (panel_widget) {
        m_visual_container = new VisualContainer(panel_widget);
        m_visual_container->move(position);
        m_visual_container->show();
        m_visual_widget = paintWidget(m_visual_container);
        m_visual_widget->show();
    }
}

QWidget *Visual::paintWidget(VisualContainer *visual_container)
{
    QWidget *widget = new QWidget(visual_container);
    widget->setGeometry(50, 50, 150, 100);
    widget->setAttribute(Qt::WA_StyledBackground, true);
    widget->setStyleSheet("background-color: orange; border: 1px solid gray;");
    return widget;
}

