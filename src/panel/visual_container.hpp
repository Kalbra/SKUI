#pragma once

#include <QChildEvent>
#include <QLayout>
#include <QWidget>

#include "../node/node.hpp"
class VisualContainer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Node *node READ node WRITE setNode NOTIFY nodeChanged MEMBER m_node)
public:
    explicit VisualContainer(QWidget *parent = nullptr);

    Node *node() const { return m_node; }
    void setNode(Node *node);

signals:
    void nodeChanged(Node *node);

protected:
    void childEvent(QChildEvent *) override;

private:
    Node *m_node = nullptr;
};
