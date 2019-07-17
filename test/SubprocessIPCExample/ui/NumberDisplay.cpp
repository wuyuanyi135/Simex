//
// Created by wuyuanyi on 15/07/19.
//

#include "NumberDisplay.h"

void NumberDisplay::updateContent(double content) {
    setText(QString::number(content));
}
NumberDisplay::NumberDisplay(QWidget *parent) : QPushButton(parent) {
    setText("Waiting for data...");
}

