#pragma once

#include <functional>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QObject>

#include "node/nodes/label.hpp"
#include "node/nodes/lineedit.hpp"
#include "node/nodes/serialsend.hpp"
#include "node/nodes/slider.hpp"
#include "node/nodes/textcombine.hpp"
#include "nodeeditor/nodeeditor.hpp"
#include "panel/panel.hpp"

/** @brief The Document class represents a document in the application.
 *
 * It manages the node editor and panel for the document.
 * The Document class also provides methods to create visual nodes and access the available node types.
 * There is always one active document in the application, which can be accessed via the static method activeDocument().
 */
class Document : QObject
{
    Q_OBJECT

public:
    Document(QObject *parent = nullptr);
    ~Document();

    /** @brief Returns the active document
     * 
     * This method returns a pointer to the active document.
     * The active document is the document that is currently being edited.
     * 
     * @return A pointer to the active document.
     */
    static Document *activeDocument() { return s_active_document; }

    /** @brief Returns the panel of the document
     *
     * This method returns a pointer to the panel of the document.
     * 
     * @return A pointer to the panel of the document.
     * @see Panel::Panel(QWidget *)
     */
    Panel *panel() const { return m_panel; }

    /** @brief Returns the node editor of the document
     *
     * This method returns a pointer to the node editor of the document.
     * 
     * @return A pointer to the node editor of the document.
     * @see NodeEditor
     */
    NodeEditor *nodeEditor() const { return m_nodeeditor; }

    /** @brief Returns the list of available node types
     *
     * This method returns a list of strings representing the types of nodes that can be created.
     *
     * @return A list of available node types.
     * @see Document::createVisual(const QString &name)
     */
    QList<QString> availableNodes() const;

    /** @brief Creates a node of the specified type
     *
     * This method creates a node of the specified type by the name and adds it to the node editor.
     *
     * @param name The type of the node to create.
     * @see Document::availableNodes()
     */
    void createNode(const QString &name, const QPoint position_hint = QPoint(0, 0));

    void deleteNode(Node *node);

private:
    const QMap<QString, std::function<Node *()>> m_node_factories
        = {{"Label", [this]() { return new Label(this); }},
           {"Slider", [this]() { return new Slider(this); }},
           {"SerialSend", [this]() { return new SerialSend(this); }},
           {"LineEdit", [this]() { return new LineEdit(this); }},
           {"TextCombine", [this]() { return new TextCombine(this); }}};

    Panel *m_panel = nullptr;
    NodeEditor *m_nodeeditor = nullptr;
    static Document *s_active_document;
};
