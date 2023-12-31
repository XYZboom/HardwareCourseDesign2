//
// Created by lenovo on 2023/12/30.
//

#include "car_and_door.h"

#include "time_utils.h"

#include "constants.h"
#include "uart.h"

struct CarAndDoorState stateNow;

const float doorMovePerFrame = 0.5f;
const float carXMovePerFrame = 1.0f;
const float carYMovePerFrame = 1.0f;
const float carLRMovePerFrame = 1.0f;
const float carRotatePerFrame = 5.0f;

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
        stateNow.carX += carXMovePerFrame;
    } else if (ctrl.x == BACKWARD) {
        stateNow.carX -= carXMovePerFrame;
    }
    if (ctrl.y == LEFT) {
        stateNow.carY -= carYMovePerFrame;
    } else if (ctrl.y == RIGHT) {
        stateNow.carY += carYMovePerFrame;
    }
    if (ctrl.tower == TOWER_CLOCKWISE) {
        stateNow.carAngle += carRotatePerFrame;
    } else if (ctrl.tower == TOWER_ANTICLOCKWISE) {
        stateNow.carAngle -= carRotatePerFrame;
    }
    if (ctrl.liftingRod == UP) {
        stateNow.carLiftingRodHeight += carLRMovePerFrame;
    } else if (ctrl.liftingRod == DOWN) {
        stateNow.carLiftingRodHeight -= carLRMovePerFrame;
    }
    if (ctrl.door1 == DOOR_UP) {
        stateNow.door1Height += doorMovePerFrame;
    } else if (ctrl.door1 == DOOR_DOWN) {
        stateNow.door1Height -= doorMovePerFrame;
    }
    if (ctrl.door2 == DOOR_UP) {
        stateNow.door2Height += doorMovePerFrame;
    } else if (ctrl.door2 == DOOR_DOWN) {
        stateNow.door2Height -= doorMovePerFrame;
    }
    if (ctrl.door3 == DOOR_UP) {
        stateNow.door3Height += doorMovePerFrame;
    } else if (ctrl.door3 == DOOR_DOWN) {
        stateNow.door3Height -= doorMovePerFrame;
    }
}

void carAndDoorTo(struct CarAndDoorState state) {
    struct CarAndDoorCtrl ctrl;
    while (true) {
        bool end = true;
        if (state.carX - stateNow.carX > carXMovePerFrame) {
            end = false;
            ctrl.x = FORWARD;
        } else if (stateNow.carX - state.carX > carXMovePerFrame) {
            end = false;
            ctrl.x = BACKWARD;
        } else {
            ctrl.x = NO_X_MOVE;
        }
        if (state.carY - stateNow.carY > carXMovePerFrame) {
            end = false;
            ctrl.y = RIGHT;
        } else if (stateNow.carY - state.carY > carXMovePerFrame) {
            end = false;
            ctrl.y = LEFT;
        } else {
            ctrl.y = NO_Y_MOVE;
        }
        if (state.carAngle - stateNow.carAngle > carRotatePerFrame) {
            end = false;
            ctrl.tower = TOWER_CLOCKWISE;
        } else if (stateNow.carAngle - state.carAngle > carRotatePerFrame) {
            end = false;
            ctrl.tower = TOWER_ANTICLOCKWISE;
        } else {
            ctrl.tower = NO_TOWER_ROTATE;
        }
        if (state.carLiftingRodHeight - stateNow.carLiftingRodHeight > carLRMovePerFrame) {
            end = false;
            ctrl.liftingRod = UP;
        } else if (stateNow.carLiftingRodHeight - state.carLiftingRodHeight > carLRMovePerFrame) {
            end = false;
            ctrl.liftingRod = DOWN;
        } else {
            ctrl.liftingRod = NO_LR_MOVE;
        }
        float *doorHeight = &state.door1Height;
        float *doorHeightNow = &stateNow.door1Height;
        enum DoorCtrl *doorCtrl = &ctrl.door1;
        for (int i = 0; i < 3; ++i) {
            if (doorHeight[i] - doorHeightNow[i] > doorMovePerFrame) {
                end = false;
                doorCtrl[i] = DOOR_UP;
            } else if (doorHeightNow[i] - doorHeight[i] > doorMovePerFrame) {
                end = false;
                doorCtrl[i] = DOOR_DOWN;
            } else {
                doorCtrl[i] = NO_DOOR_MOVE;
            }
        }
        if (end) break;
        sendCarAndDoorCtrl(ctrl);
        sleep_ms(80);
    }
}

struct CarAndDoorCtrl defaultCarAndDoorCtrl() {
    const struct CarAndDoorCtrl ctrl = {
        NO_X_MOVE, NO_Y_MOVE, NO_TOWER_ROTATE, NO_LR_MOVE, NO_DOOR_MOVE, NO_DOOR_MOVE, NO_DOOR_MOVE
    };
    return ctrl;
}
