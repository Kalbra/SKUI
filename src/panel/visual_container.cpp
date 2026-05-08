#include "visual_container.hpp"

VisualContainer::VisualContainer(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
}

void VisualContainer::childEvent(QChildEvent *event)
{
    if (event->added()) {
        QWidget *child_widget = qobject_cast<QWidget *>(event->child());
        if (child_widget) {
            layout()->addWidget(child_widget);
        }
    }
    QWidget::childEvent(event);
}

void VisualContainer::setNode(Node *node)
{
    Q_ASSERT(node);
    
    if (m_node == node) {
        return;
    }
    m_node = node;
    emit nodeChanged(node);
}