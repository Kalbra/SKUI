#pragma once

#include <pybind11/embed.h>
#include <QDebug>
#include <QLoggingCategory>

namespace py = pybind11;

/** @brief A proxy for redirecting Python's stdout and stderr to Qt's logging system. */
class StdoutProxy
{
public:
    /** @brief Sets up the proxy for redirecting Python's stdout and stderr.
     *
     * This method imports the StdoutProxy class into the Python interpreter and redirects
     * sys.stdout and sys.stderr to an instance of StdoutProxy. This allows Python's print
     * statements and error messages to be captured and logged through Qt's logging system.
     */
    static void setupProxy();

    /** @brief Writes a message to the proxy.
     * Used by pybind11 to redirect stdout to this proxy, do not use this method yourself!
     * 
     * @param message The message to write.
     */
    void _private_write(const std::string &message);

    /** @brief Flushes the proxy.
     * 
     * Not implemented yet.
     */
    void _private_flush() {};
};