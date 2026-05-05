#pragma once

#include "../node/node.h"
#include <QChildEvent>
#include <QLayout>
#include <QWidget>

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
