#pragma once

#include <pybind11/embed.h>
#include <QApplication>
#include <QDebug>
#include <QEvent>

#include "python/python_service.hpp"

namespace py = pybind11;

class Skui : public QApplication
{
    Q_OBJECT

public:
    explicit Skui(int &, char **);
    ~Skui();

private:
    Q_DISABLE_COPY_MOVE(Skui)

    QThread *m_python_service_thread;
    std::unique_ptr<py::scoped_interpreter> m_python_interpreter;
    std::unique_ptr<py::gil_scoped_release> m_python_gil_release;
};
