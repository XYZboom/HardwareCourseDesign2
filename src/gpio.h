//
// Created by lenovo on 2024/1/2.
//

#ifndef GPIO_H
#define GPIO_H
#include <stdbool.h>

void initGPIO();

bool sw(int num);

bool isBtn8On();

bool isBtn9On();

enum BtnType {
    BTNC = 0,
    BTNR,
    BTNL,
    BTND,
    BTNU,
};

bool isBtnOn(enum BtnType btnType);

#endif //GPIO_H
