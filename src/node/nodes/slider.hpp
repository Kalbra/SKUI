#pragma once

#include <QDebug>
#include <QSlider>

#include "visual.hpp"

class Slider : public Visual
{
    Q_OBJECT
public:
    explicit Slider(QObject *parent = nullptr);

private Q_SLOTS:
    void onSliderValueChanged(int value);

private:
    QWidget *paintWidget(VisualContainer *) override;
};
