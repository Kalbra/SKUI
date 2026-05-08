#include "mainwindow.hpp"
#include "ui_mainwindow.h"

Q_DECLARE_LOGGING_CATEGORY(gui)
Q_LOGGING_CATEGORY(gui, "GUI")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("SKUI");

    QPixmap icon;
    icon.fill(QColor("#FF1FA9"));

    setWindowIcon(QIcon(icon));

    Document *new_document = new Document(this);
    ui->tabWidget->addTab(new_document->panel(), "Panel");
    ui->tabWidget->addTab(new_document->nodeEditor(), "Node Editor");
    ui->tabWidget->setCurrentWidget(new_document->nodeEditor());

    loadInsertVisualMenu();
    loadDebugMenu();
    loadAboutMenu();
    loadAlignTools();

    connect(this, &MainWindow::modeChanged, new_document->panel(), &Panel::setMode);
    //focus_document->createVisual(VisualType::Slider);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadInsertVisualMenu()
{
    for (const QString &visual_name : Document::activeDocument()->availableNodes()) {
        QAction *menu_insert_action = new QAction(visual_name);
        connect(menu_insert_action, &QAction::triggered, this, [this, visual_name] {
            Document::activeDocument()->createNode(visual_name, mapToGlobal(QPoint(100, 200)));
            //connect(this, &MainWindow::modeChanged, visual, &Visual::setMode);
        });
        menu_insert_action->setText(visual_name);
        ui->menuInsertVisual->addAction(menu_insert_action);
    }
}

void MainWindow::loadDebugMenu()
{
#ifdef QT_DEBUG
    QMenu *debug_menu = menuBar()->addMenu("Debug");
#endif
}

void MainWindow::loadAboutMenu()
{
    QMenu *about_menu = menuBar()->addMenu("About");

    about_menu->addAction("Repository", this, [] {
        QDesktopServices::openUrl(QUrl("https://dev.skui.eu/SKUI/SKUI"));
    });

    about_menu->addAction("Report a Bug", this, [] {
        QDesktopServices::openUrl(QUrl("https://dev.skui.eu/SKUI/SKUI/issues/"
                                       "new?template=.gitea%2fissue_template%2fbug-report.yml"));
    });

    about_menu->addAction("Wiki", this, [] {
        QDesktopServices::openUrl(QUrl("https://dev.skui.eu/SKUI/SKUI/wiki"));
    });

    about_menu->addAction("Contact Mail", this, [] {
        QDesktopServices::openUrl(QUrl("mailto:info@skui.eu"));
    });

    about_menu->addAction("License (GPLv3)", this, [] {
        QDesktopServices::openUrl(QUrl("https://dev.skui.eu/SKUI/SKUI/src/branch/main/LICENSE"));
    });

    const QString build_commit = QString::fromLatin1(SKUI_BUILD_COMMIT);
    const QString build_tag = QString::fromLatin1(SKUI_BUILD_TAG);
    QString info_title = QStringLiteral("Commit (") + build_commit;
    if (!build_tag.isEmpty()) {
        info_title += QStringLiteral(", ") + build_tag;
    }
    info_title += QLatin1Char(')');
    about_menu->addAction(info_title);
}

void MainWindow::loadAlignTools()
{
    const AlignTool align_tools[]
        = {{"Align Top", AlignDirection::Top, QIcon(":/icons/align-vertical-top.svg")},
           {"Align Right", AlignDirection::Right, QIcon(":/icons/align-horizontal-right.svg")},
           {"Align Bottom", AlignDirection::Bottom, QIcon(":/icons/align-vertical-bottom.svg")},
           {"Align Left", AlignDirection::Left, QIcon(":/icons/align-horizontal-left.svg")},
           {"Align Horizontal",
            AlignDirection::Horizontal,
            QIcon(":/icons/align-horizontal-center.svg")},
           {"Align Vertical", AlignDirection::Vertical, QIcon(":/icons/align-vertical-center.svg")}};

    for (const AlignTool &align_tool : align_tools) {
        QPushButton *align_button = new QPushButton(this);
        ui->align->addWidget(align_button);

        align_button->setIcon(align_tool.icon);
        align_button->setToolTip(align_tool.tool_tip);

        connect(align_button, &QPushButton::clicked, this, [this, align_tool] {
            Document::activeDocument()->panel()->triggeredAlign(align_tool.direction);
        });
    }
}

void MainWindow::on_mode_changed_button_clicked()
{
    if (m_display_mode == DisplayMode::Run) {
        m_display_mode = DisplayMode::Edit;
        ui->mode_changed_button->setText("Run Mode");
        qInfo(gui) << "Display mode changed to Edit";
    } else {
        m_display_mode = DisplayMode::Run;
        ui->mode_changed_button->setText("Edit Mode");
        qInfo(gui) << "Display mode changed to Run";
    }
    emit modeChanged(m_display_mode);
}

bool MainWindow::event(QEvent *event)
{
// Shot eixt option using Escape in Debug Mode
#ifdef QT_DEBUG
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
        if (key_event->key() == Qt::Key_Escape) {
            QApplication::quit();
            return true;
        }
    }
#endif
    return QMainWindow::event(event);
}
