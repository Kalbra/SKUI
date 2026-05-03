#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>

#include "document.h"
#include "nodeeditor.h"
#include "panel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct AlignTool
{
    QString tool_tip;
    AlignDirection direction;
    QIcon icon;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool event(QEvent *);

signals:
    void modeChanged(DisplayMode);

private slots:
    void on_mode_changed_button_clicked();

private:
    Ui::MainWindow *ui;

    void loadInsertVisualMenu();
    void loadDebugMenu();
    void loadAlignTools();

    DisplayMode m_display_mode = DisplayMode::Run;
};
#endif // MAINWINDOW_H
