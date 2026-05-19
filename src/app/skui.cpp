#include "skui.hpp"

Skui::Skui(int &argc, char **argv)
    : QApplication(argc, argv)
{
    // Initialize the Python interpreter.
    m_python_interpreter = std::make_unique<py::scoped_interpreter>();
    m_python_gil_release = std::make_unique<py::gil_scoped_release>();

    m_python_service_thread = new QThread(this);
    PythonService *python_service = new PythonService();

    python_service->moveToThread(m_python_service_thread);

    connect(m_python_service_thread,
            &QThread::started,
            python_service,
            &PythonService::startInterpreter);
    connect(python_service, &PythonService::crashed, this, [](const QString &error_message) {
        qWarning() << "Python interpreter crashed:" << error_message;
    });

    m_python_service_thread->start();
}

Skui::~Skui()
{
    Q_ASSERT(m_python_service_thread);

    // Delete and wait for delete of python service thread.
    if (m_python_service_thread->isRunning()) {
        m_python_service_thread->quit();
        m_python_service_thread->wait();
    }

    // Aquire the GIL, than shutdown.
    m_python_gil_release.reset();
    m_python_interpreter.reset();
}