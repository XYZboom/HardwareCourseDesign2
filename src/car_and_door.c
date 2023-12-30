//
// Created by lenovo on 2023/12/30.
//

#include "car_and_door.h"

#include "constants.h"
#include "uart.h"

int door1Height = 0;
int door2Height = 0;
int door3Height = 0;
int carX = 0;
int carY = 0;

void sendCarAndDoorCtrl(struct CarAndDoorCtrl ctrl) {
    sendChar(MODBUS_PREAMBLE);
    sendChar(CAR_AND_DOOR_CTRL_ADDRESS);
    sendChar(ctrl.x);
    sendChar(ctrl.y);
    sendChar(ctrl.tower);
    sendChar(ctrl.liftingRod);
    sendChar(ctrl.door1);
    sendChar(ctrl.door2);
    sendChar(ctrl.door3);
    if (ctrl.x == FORWARD) {
        carX++;
    } else if (ctrl.x == BACKWARD) {
        carX--;
    }
}



struct CarAndDoorCtrl defaultCarAndDoorCtrl() {
    const struct CarAndDoorCtrl ctrl = {
        NO_X_MOVE, NO_Y_MOVE, NO_TOWER_ROTATE, NO_LR_MOVE, NO_DOOR_MOVE, NO_DOOR_MOVE, NO_DOOR_MOVE
    };
    return ctrl;
}
