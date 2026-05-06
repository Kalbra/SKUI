#pragma once

#include <QDesktopServices>
#include <QDir>
#include <QMainWindow>
#include <QMessageBox>

#include "document.hpp"
#include "nodeeditor.hpp"
#include "panel.hpp"

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
    void loadAboutMenu();
    void loadAlignTools();

    DisplayMode m_display_mode = DisplayMode::Run;
};
