#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <functional>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QObject>

#include "nodeeditor.h"
#include "nodes/label.h"
#include "nodes/lineedit.h"
#include "nodes/serialsend.h"
#include "nodes/slider.h"
#include "nodes/textcombine.h"
#include "panel.h"

/** @brief The Document class represents a document in the application.
 *
 * It manages the node editor and panel for the document.
 * The Document class also provides methods to create visual nodes and access the available node types.
 * There is always one active document in the application, which can be accessed via the static method activeDocument().
 */
class Document : public QObject
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
    template<typename NodeType, typename Parent>
    std::function<Node *()> createNodeFactory(Parent *parent)
    {
        return [parent]() -> Node * { return new NodeType(parent); };
    }

    const QMap<QString, std::function<Node *()>> m_node_factories
        = {{"Label",       createNodeFactory<Label>(this)},
           {"LineEdit",    createNodeFactory<LineEdit>(this)},
           {"TextCombine", createNodeFactory<TextCombine>(this)},
           {"Slider",      createNodeFactory<Slider>(this)},
           {"SerialSend",  createNodeFactory<SerialSend>(this)},
           {"LineEdit",    createNodeFactory<LineEdit>(this)},
           {"TextCombine", createNodeFactory<TextCombine>(this)}};

    Panel *m_panel = nullptr;
    NodeEditor *m_nodeeditor = nullptr;
    static Document *s_active_document;
};

#endif // DOCUMENT_H
