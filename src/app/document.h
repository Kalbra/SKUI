#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QJsonObject>
#include <QObject>

#include "nodeeditor.h"
#include "nodes/label.h"
#include "nodes/lineedit.h"
#include "nodes/serialsend.h"
#include "nodes/slider.h"
#include "nodes/textcombine.h"
#include "panel.h"

enum class VisualType { Parent, Test, Slider, SerialSend, LineEdit, TextCombine };

class Document : QObject
{
    Q_OBJECT

public:
    Document(QObject *, Panel *, NodeEditor *);
    ~Document();

    QJsonObject save();
    void load(QJsonObject);

    void createVisual(VisualType type);

    DisplayMode display_mode = DisplayMode::Run;

private:
    Panel *m_panel;
    NodeEditor *m_nodeeditor;
};

#endif // DOCUMENT_H
