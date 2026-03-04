#ifndef TEXTCOMBINE_H
#define TEXTCOMBINE_H

#include "../node.h"

class TextCombine : public Node
{
    Q_OBJECT
public:
    explicit TextCombine(QObject *parent = nullptr);

private:
    void valueChanged();

};

#endif // TEXTCOMBINE_H