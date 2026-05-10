#include "lineedit.hpp"

LineEdit::LineEdit(QObject *parent)
    : Visual{parent}
{
    config();
    Interface text_interface = Interface(QVariant(""), InterfaceDirection::Output, "text");

    setInterfaces({text_interface});
}

QWidget *LineEdit::paintWidget(VisualContainer *visual_container)
{
    Q_ASSERT(visual_container);

    QLineEdit *line_edit = new QLineEdit(visual_container);
    line_edit->show();

    connect(line_edit, &QLineEdit::textChanged, this, &LineEdit::onLineEditValueChanged);

    return line_edit;
}

void LineEdit::onLineEditValueChanged(const QString &value)
{
    Interface *text_interface = getInterface("text");
    if (text_interface) {
        qDebug() << "LineEdit value changed to: " << value;
        text_interface->setValue(QVariant(value));

        text_interface->update();
    }
}