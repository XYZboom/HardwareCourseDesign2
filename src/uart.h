//
// Created by XYZboom on 2023/9/19.
//

#ifndef DESIGN2_PROJECT_UART_H
#define DESIGN2_PROJECT_UART_H
#include <stdbool.h>

void sendChar(unsigned char data);

unsigned char receiveChar();

void sendString(const char *string);

bool hasData();

void clearData();

#endif //DESIGN2_PROJECT_UART_H
