#include "propertywindow.hpp"

PropertyWindow::PropertyWindow(QWidget* parent, const QObject* object)
    : QWidget{parent, Qt::Window}
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    QScrollArea* scroll_area = new QScrollArea(this);
    scroll_area->setWidgetResizable(true);
    scroll_area->setFrameShape(QFrame::NoFrame);

    QWidget* content_widget = new QWidget();
    QFormLayout* form_layout = new QFormLayout(content_widget);

    if (object) {
        const QMetaObject* meta = object->metaObject();
        for (int i = 0; i < meta->propertyCount(); i++) {
            QMetaProperty prop = meta->property(i);

            if (!prop.isReadable())
                continue;

            QString name = prop.name();
            QVariant value = object->property(name.toLatin1());
            QString string_value = value.toString();

            if (string_value.isEmpty() && !value.isValid()) {
                string_value = "<Invalid>";
            } else if (string_value.isEmpty()) {
                string_value = QString("<%1>").arg(value.typeName());
            }

            form_layout->addRow(new QLabel(name), new QLabel(string_value));
        }

        setWindowTitle("Property Window [" + object->objectName() + "]");
    }
    scroll_area->setWidget(content_widget);
    main_layout->addWidget(scroll_area);
}
