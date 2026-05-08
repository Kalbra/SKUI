#include "textcombine.hpp"

TextCombine::TextCombine(QObject *parent)
    : Node{parent}
{
    config();
    Interface first_interface = Interface(QVariant(""), InterfaceDirection::Input, "first");
    Interface second_interface = Interface(QVariant(""), InterfaceDirection::Input, "second");
    Interface output_interface = Interface(QVariant(""), InterfaceDirection::Output, "output");
    setInterfaces({first_interface, second_interface, output_interface});

    getInterface("first")->setCallback([this]() { valueChanged(); });
    getInterface("second")->setCallback([this]() { valueChanged(); });
}

void TextCombine::valueChanged()
{
    QString combined_string = getInterface("first")->toString()
                              + getInterface("second")->toString();

    getInterface("output")->setValue(combined_string);
    getInterface("output")->update();
}