#include "label.h"

Label::Label(QObject *parent)
    : Visual{parent}
{
    config();
    Interface text_interface = Interface(QVariant(""), InterfaceDirection::Input, "text");

    setInterfaces({text_interface});
    getInterface("text")->setCallback([this]() { setText(); });
}

QWidget *Label::paintWidget(VisualContainer *visual_container)
{
    // Create a label in the panel for this node.
    m_label = new QLabel("<Label>", visual_container);
    m_label->setGeometry(10, 10, 180, 30);
    m_label->show();
    return m_label;
}

void Label::setText()
{
    Interface *text_interface = getInterface("text");
    if (text_interface && m_label) {
        m_label->clear();
        m_label->setText(text_interface->toString());
    }
}
