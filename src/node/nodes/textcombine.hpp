#pragma once

#include "../node.h"

class TextCombine : public Node
{
    Q_OBJECT
public:
    explicit TextCombine(QObject *parent = nullptr);

private:
    void valueChanged();
};
