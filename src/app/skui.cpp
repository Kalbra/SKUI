#include "skui.hpp"

Skui::Skui(int &argc, char **argv)
    : QApplication(argc, argv)
{}

bool Skui::notify(QObject *receiver, QEvent *event)
{
    return QApplication::notify(receiver, event);
}
