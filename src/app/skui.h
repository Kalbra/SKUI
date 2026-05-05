#pragma once

#include <QApplication>
#include <QDebug>
#include <QEvent>

class Skui : public QApplication
{
public:
    explicit Skui(int &, char **);

    bool notify(QObject *, QEvent *) override;
};
