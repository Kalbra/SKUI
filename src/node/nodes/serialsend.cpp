#include "serialsend.hpp"

SerialSend::SerialSend(QObject *parent)
    : Visual{parent}
{
    config();
    Interface send_interface = Interface(QVariant(""), InterfaceDirection::Input, "send");

    setInterfaces({send_interface});
    getInterface("send")->setCallback([this]() { send(); });
}

QWidget *SerialSend::paintWidget(VisualContainer *visual_container)
{
    m_combo_box = new QComboBox(visual_container);

    for (QSerialPortInfo port_info : QSerialPortInfo::availablePorts()) {
        m_combo_box->addItem(port_info.portName());
    }

    m_combo_box->show();

    return m_combo_box;
}

void SerialSend::send()
{
    QString port = m_combo_box->currentText();
    QString msg = getInterface("send")->toString();

    if (!m_serial_port.isOpen()) {
        m_serial_port.setPortName(port);
        m_serial_port.setBaudRate(9600);
        m_serial_port.setDataBits(QSerialPort::Data8);
        m_serial_port.setParity(QSerialPort::NoParity);
        m_serial_port.setStopBits(QSerialPort::OneStop);
        m_serial_port.setFlowControl(QSerialPort::NoFlowControl);
        m_serial_port.open(QSerialPort::WriteOnly);
    }

    m_serial_port.write(msg.toUtf8() + "\n");

    qDebug() << "Virtual Send: " << port << msg;
}