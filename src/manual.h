//
// Created by lenovo on 2024/1/2.
//

#ifndef MANUAL_H
#define MANUAL_H
#include <stdbool.h>

int getSpeedType();

enum ArmRotateCtrl armRotateCtrlFromSpeed(int speed, bool anticlock);

enum ArmMoveCtrl armMoveCtrlFromSpeed(int speed, bool left);

#endif //MANUAL_H
