#include "visual.hpp"

Q_DECLARE_LOGGING_CATEGORY(visual)
Q_LOGGING_CATEGORY(visual, "VISUAL")

Visual::Visual(QObject *parent)
    : Node(parent)
{
    qDebug() << this->metaObject()->className();
} 

void Visual::setWidget(QWidget *widget)
{
    Q_ASSERT(widget);

    if (m_widget == widget) {
        return;
    }

    m_widget = widget;
    emit widgetChanged(widget);
}

QWidget *Visual::paintWidget(VisualContainer *visual_container)
{
    Q_ASSERT(visual_container);

    QWidget *widget = new QWidget(visual_container);
    widget->setGeometry(50, 50, 150, 100);
    widget->setAttribute(Qt::WA_StyledBackground, true);
    widget->setStyleSheet("background-color: orange; border: 1px solid gray;");
    return widget;
}
