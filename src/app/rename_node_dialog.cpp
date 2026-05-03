#include "rename_node_dialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

RenameNodeDialog::RenameNodeDialog(const QString &current_name, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Rename Node");

    m_label = new QLabel("Rename:", this);
    m_name_edit = new QLineEdit(current_name, this);
    m_button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_label);
    layout->addWidget(m_name_edit);
    layout->addWidget(m_button_box);

    connect(m_button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_name_edit, &QLineEdit::textChanged, this, &RenameNodeDialog::validateInput);

    m_name_edit->selectAll();
    validateInput(m_name_edit->text());
}

QString RenameNodeDialog::nodeName() const
{
    return m_name_edit->text().trimmed();
}

void RenameNodeDialog::validateInput(const QString &text)
{
    QPushButton *ok_button = m_button_box->button(QDialogButtonBox::Ok);
    if (ok_button) {
        ok_button->setEnabled(!text.trimmed().isEmpty());
    }
}
