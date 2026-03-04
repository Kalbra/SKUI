#include "document.h"

Document::Document(QObject *parent, Panel *panel, NodeEditor *nodeeditor)
    : QObject{parent}
    , m_panel(panel)
    , m_nodeeditor(nodeeditor)
{}

Document::~Document() {}

QJsonObject Document::save()
{
    return QJsonObject();
}

void Document::load(QJsonObject content)
{
    return;
}

void Document::createVisual(VisualType type)
{
    if (type == VisualType::Test) {
        Label *label = new Label(this);
        label->setPanel(m_panel, QPoint(200, 200));
        m_nodeeditor->addNode(label);
    } else if (type == VisualType::Slider) {
        Slider *slider = new Slider(this);
        slider->setPanel(m_panel, QPoint(400, 200));
        m_nodeeditor->addNode(slider);
    } else if (type == VisualType::SerialSend) {
        SerialSend *serialsend = new SerialSend(this);
        serialsend->setPanel(m_panel, QPoint(300, 300));
        m_nodeeditor->addNode(serialsend);
    } else if (type == VisualType::LineEdit) {
        LineEdit *line_edit = new LineEdit(this);
        line_edit->setPanel(m_panel, QPoint(100, 100));
        m_nodeeditor->addNode(line_edit);
    } else if (type == VisualType::TextCombine) {
        TextCombine *text_combine = new TextCombine(this);
        m_nodeeditor->addNode(text_combine);
    }
}