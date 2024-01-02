//
// Created by lenovo on 2024/1/2.
//

#include "manual.h"

#include "gpio.h"
#include "mechanical_arm.h"

int getSpeedType() {
    return (sw(4) << 1) | sw(3);
}

enum ArmRotateCtrl rotateCtrlFromSpeed(const int speed, bool anticlock) {
    if (anticlock) {
        return ANTICLOCKWISE_1_DEGREE + speed;
    }
    return CLOCKWISE_1_DEGREE + speed;
}
