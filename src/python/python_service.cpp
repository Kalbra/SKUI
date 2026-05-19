#include "python_service.hpp"

void PythonService::startInterpreter()
{
    // The python servie run in a separate thread, so we need to acquire the GIL.
    // Because the interpreter is started in the main thread.
    py::gil_scoped_acquire acquire;

    StdoutProxy::setupProxy();

    try {
        py::exec(R"(
            import sys
            print("Hello from Python!")
            print("Python version:", sys.version)
        )");
        Q_EMIT crashed("Python interpreter started successfully.");

    } catch (const py::error_already_set &e) {
        Q_EMIT crashed(QString::fromStdString(e.what()));
    }
}