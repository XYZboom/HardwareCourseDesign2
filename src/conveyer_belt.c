//
// Created by lenovo on 2023/12/30.
//
#include "conveyer_belt.h"

#include <constants.h>
#include <uart.h>

void sendConveyerBeltCtrl(enum ConveyerBeltCtrl ctrl2, enum ConveyerBeltCtrl ctrl3) {
    sendChar(MODBUS_PREAMBLE);
    sendChar(CONVEYER_BELT_ADDRESS);
    sendChar(ctrl2);
    sendChar(ctrl3);
    for (int i = 0; i < 6; ++i) {
        sendChar('0');
    }
}
