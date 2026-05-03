#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

#include "visual.h"

class LineEdit : public Visual
{
    Q_OBJECT

public:
    explicit LineEdit(QObject *parent = nullptr);

private slots:
    void onLineEditValueChanged(const QString &value);

private:
    QWidget *paintWidget(VisualContainer *) override;
};

#endif // LINEEDIT_H
