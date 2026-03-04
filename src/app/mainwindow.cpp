#include "mainwindow.h"
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

    panel = new Panel;
    ui->tabWidget->addTab(panel, "Panel");

    NodeEditor *nodeeditor = new NodeEditor;
    ui->tabWidget->addTab(nodeeditor, "Node Editor");
    ui->tabWidget->setCurrentWidget(nodeeditor);

    focus_document = new Document(this, panel, nodeeditor);

    loadInsertVisualMenu();
    loadDebugMenu();

    loadAlignTools();

    connect(this, &MainWindow::modeChanged, panel, &Panel::setMode);
    //focus_document->createVisual(VisualType::Slider);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadInsertVisualMenu()
{
    const VisualMenuAction wrapped_actions[] = {
        {"Label", VisualType::Test},
        {"Slider", VisualType::Slider},
        {"SerialSend", VisualType::SerialSend},
        {"LineEdit", VisualType::LineEdit},
        {"TextCombine", VisualType::TextCombine},
    };

    for (const VisualMenuAction &wraped_action : wrapped_actions) {
        QAction *menu_insert_action = new QAction(wraped_action.name);
        connect(menu_insert_action, &QAction::triggered, this, [this, wraped_action] {
            focus_document->createVisual(wraped_action.type);
            //connect(this, &MainWindow::modeChanged, visual, &Visual::setMode);
        });

        menu_insert_action->setText(wraped_action.name);
        ui->menuInsertVisual->addAction(menu_insert_action);
    }
}

void MainWindow::loadDebugMenu()
{
#ifdef QT_DEBUG
    QMenu *debug_menu = menuBar()->addMenu("Debug");
    debug_menu->addAction("Insert routed Slider => Laber", this, [this]() {
        Slider *slider = new Slider(this);
        slider->setPanel(panel);
        Label *label = new Label(this);
        label->setPanel(panel);
        qDebug() << "Routing Slider to Label";
        qDebug() << slider->getInterfaces().first().routeTo(&label->getInterfaces().first());
    });
    debug_menu->addAction("Add Test Node", this, [this]() {
        focus_document->createVisual(VisualType::Slider);
        qDebug() << "Added test slider node via Document";
    });
    debug_menu->addAction("Add Label Node", this, [this]() {
        focus_document->createVisual(VisualType::Test); // Test creates Label
        qDebug() << "Added test label node via Document";
    });
    debug_menu->addAction("Add Label Node", this, [this]() {
        Label *test_label = new Label(this);
        test_label->setPanel(panel, QPoint(300, 100));
        // Find the nodeeditor from the tab widget
        for (int i = 0; i < ui->tabWidget->count(); ++i) {
            NodeEditor *nodeeditor = qobject_cast<NodeEditor *>(ui->tabWidget->widget(i));
            if (nodeeditor) {
                nodeeditor->addNode(test_label);
                qDebug() << "Added test label node to NodeEditor";
                break;
            }
        }
    });
#endif
}

const QIcon MainWindow::loadIcon(const QString &identifier)
{
    return QIcon("./assets/breeze_icon/" + identifier);
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
            panel->triggeredAlign(align_tool.direction);
        });
    }
}

void MainWindow::on_mode_changed_button_clicked()
{
    if (focus_document->display_mode == DisplayMode::Run) {
        focus_document->display_mode = DisplayMode::Edit;
        ui->mode_changed_button->setText("Run Mode");
        qInfo(gui) << "Display mode changed to Edit";
    } else {
        focus_document->display_mode = DisplayMode::Run;
        ui->mode_changed_button->setText("Edit Mode");
        qInfo(gui) << "Display mode to Run";
    }
    emit modeChanged(focus_document->display_mode);
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
