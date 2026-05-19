#include "stdout_proxy.hpp"

Q_DECLARE_LOGGING_CATEGORY(python)
Q_LOGGING_CATEGORY(python, "PYTHON")

void StdoutProxy::setupProxy()
{
    py::module_ sys = py::module_::import("sys");
    py::module_ stdout_proxy = py::module_::import("stdout_proxy");

    py::object proxy_instance = stdout_proxy.attr("StdoutProxy")();

    sys.attr("stdout") = proxy_instance;
    sys.attr("stderr") = proxy_instance;
}

void StdoutProxy::_private_write(const std::string &message)
{
    if (message != "\n") {
        qInfo(python).noquote() << QString::fromStdString(message);
    }
}

PYBIND11_EMBEDDED_MODULE(stdout_proxy, m)
{
    py::class_<StdoutProxy>(m, "StdoutProxy")
        .def(py::init<>())
        .def("write", &StdoutProxy::_private_write)
        .def("flush", &StdoutProxy::_private_flush);
}