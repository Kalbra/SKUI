#pragma once

#include <QFormLayout>
#include <QLabel>
#include <QMetaProperty>
#include <QScrollArea>
#include <QWidget>

class PropertyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyWindow(QWidget *parent, const QObject *object);
};
