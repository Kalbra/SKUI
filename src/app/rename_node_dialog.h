#pragma once

#include <QDialog>

class QDialogButtonBox;
class QLabel;
class QLineEdit;

class RenameNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameNodeDialog(const QString &current_name, QWidget *parent = nullptr);
    QString nodeName() const;

private slots:
    void validateInput(const QString &text);

private:
    QLabel *m_label = nullptr;
    QLineEdit *m_name_edit = nullptr;
    QDialogButtonBox *m_button_box = nullptr;
};
