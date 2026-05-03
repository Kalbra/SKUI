#ifndef SERIALSEND_H
#define SERIALSEND_H

#include <QComboBox>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "visual.h"

class SerialSend : public Visual
{
    Q_OBJECT

public:
    explicit SerialSend(QObject *parent = nullptr);

private:
    QWidget *paintWidget(VisualContainer *) override;
    void send();

    QComboBox *m_combo_box;
    QSerialPort m_serial_port;
};

#endif // SERIALSEND_H
