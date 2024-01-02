//
// Created by lenovo on 2024/1/2.
//

#ifndef MANUAL_H
#define MANUAL_H
#include <stdbool.h>

int getSpeedType();

enum ArmRotateCtrl rotateCtrlFromSpeed(int speed, bool anticlock);

#endif //MANUAL_H
