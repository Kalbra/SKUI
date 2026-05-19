#pragma once

#include <QLineEdit>

#include "visual.hpp"

class LineEdit : public Visual
{
    Q_OBJECT

public:
    explicit LineEdit(QObject *parent = nullptr);

private Q_SLOTS:
    void onLineEditValueChanged(const QString &value);

private:
    QWidget *paintWidget(VisualContainer *) override;
};
