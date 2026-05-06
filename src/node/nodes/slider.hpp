#pragma once

#include <QDebug>
#include <QSlider>

#include "visual.hpp"

class Slider : public Visual
{
    Q_OBJECT
public:
    explicit Slider(QObject *parent = nullptr);

private slots:
    void onSliderValueChanged(int value);

private:
    QWidget *paintWidget(VisualContainer *) override;
};
