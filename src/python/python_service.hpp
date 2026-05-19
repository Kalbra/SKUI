#pragma once

#include <pybind11/embed.h>
#include <QObject>
#include <QThread>

#include "stdout_proxy.hpp"

namespace py = pybind11;

class PythonService : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void startInterpreter();

Q_SIGNALS:
    void crashed(const QString &error_message);
};
