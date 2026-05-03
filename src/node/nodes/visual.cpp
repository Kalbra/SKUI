#include "visual.h"

Q_DECLARE_LOGGING_CATEGORY(visual)
Q_LOGGING_CATEGORY(visual, "VISUAL")

Visual::Visual(QObject *parent)
    : Node(parent)
{
    qDebug() << this->metaObject()->className();
}

void Visual::setWidget(QWidget *widget)
{
    if (m_widget == widget) {
        return;
    }

    m_widget = widget;
    emit widgetChanged(widget);
}