#pragma once

#include "../node.hpp"

class TextCombine : public Node
{
    Q_OBJECT
public:
    explicit TextCombine(QObject *parent = nullptr);

private:
    void valueChanged();
};
