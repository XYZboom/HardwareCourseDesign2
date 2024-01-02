//
// Created by XYZboom on 2023/9/19.
//

#include <stdbool.h>
#include "mechanical_arm.h"

#include "chest.h"
#include "conveyer_belt.h"
#include "logger.h"

#include "uart.h"
#include "constants.h"
#include "time_utils.h"
#include "gpio.h"

enum ChestColor getChestColor() {
    const unsigned char data = receiveChar();
    LOG("DEBUG", "getChestColor %c", data);
    switch (data) {
        case 'R':
            return RED;
        case 'B':
            return BLUE;
        case 'G':
            return GREEN;
        default:
            return GREEN;
    }
}

int armNow[] = {0, 0, 0, 0, 0, 0};
int armPos = 0;

struct ArmCtrl defaultArmCtrl(enum ArmAddress address) {
    struct ArmCtrl ctrl;
    ctrl.address = address;
    ctrl.moveCtrl = NO_MOVE;
    for (int i = 0; i < AXLE_NUM; ++i) {
        ctrl.rotateCtrl[i] = NO_ROTATE;
    }
    return ctrl;
}

void sendArmCtrl(struct ArmCtrl ctrl) {
    sendChar(MODBUS_PREAMBLE);
    sendChar(ctrl.address);
    for (int i = 0; i < AXLE_NUM; ++i) {
        switch (ctrl.rotateCtrl[i]) {
            case NO_ROTATE:
                break;
            case CLOCKWISE_1_DEGREE:
                armNow[i] += 1;
                break;
            case CLOCKWISE_2_DEGREE:
                armNow[i] += 2;
                break;
            case CLOCKWISE_3_DEGREE:
                armNow[i] += 3;
                break;
            case CLOCKWISE_5_DEGREE:
                armNow[i] += 5;
                break;
            case ANTICLOCKWISE_1_DEGREE:
                armNow[i] -= 1;
                break;
            case ANTICLOCKWISE_2_DEGREE:
                armNow[i] -= 2;
                break;
            case ANTICLOCKWISE_3_DEGREE:
                armNow[i] -= 3;
                break;
            case ANTICLOCKWISE_5_DEGREE:
                armNow[i] -= 5;
                break;
        }
        sendChar(ctrl.rotateCtrl[i]);
    }
    if (ctrl.address == ARM_1_ADDRESS) {
        sendChar(NO_MOVE);
    } else {
        switch (ctrl.moveCtrl) {
            case RIGHT_FASTEST:
                armPos += 10;
                break;
            case RIGHT_MIDDLE:
                armPos += 5;
                break;
            case RIGHT_SLOWEST:
                armPos += 3;
                break;
            case LEFT_FASTEST:
                armPos -= 10;
                break;
            case LEFT_MIDDLE:
                armPos -= 5;
                break;
            case LEFT_SLOWEST:
                armPos -= 3;
                break;
            case NO_MOVE:
                break;
        }
        sendChar(ctrl.moveCtrl);
    }
}


void sendEmptyArmCtrl(enum ArmAddress address) {
    const struct ArmCtrl emptyCtrl = {
        address, {NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE}, NO_MOVE
    };
    sendArmCtrl(emptyCtrl);
}

void armTo(enum ArmAddress address, struct ArmState target) {
    sendEmptyArmCtrl(address);
    int sleepTime = 3;
    while (sw(0)) {
        struct ArmCtrl armCtrl = {address};
        armCtrl.moveCtrl = NO_MOVE;
        bool end = true;
        for (int i = 0; i < AXLE_NUM; ++i) {
            int need = (target.degree[i] - armNow[i]) % 360;
            if (need == 0) {
                armCtrl.rotateCtrl[i] = NO_ROTATE;
            } else {
                end = false;
                if (need > 180) {
                    need = (need - 360) % 360;
                } else if (need < -180) {
                    need = (need + 360) % 360;
                }
                if (need >= 5) {
                    armCtrl.rotateCtrl[i] = CLOCKWISE_5_DEGREE;
                } else if (need >= 3) {
                    armCtrl.rotateCtrl[i] = CLOCKWISE_3_DEGREE;
                } else if (need == 2) {
                    armCtrl.rotateCtrl[i] = CLOCKWISE_2_DEGREE;
                } else if (need == 1) {
                    armCtrl.rotateCtrl[i] = CLOCKWISE_1_DEGREE;
                } else if (need == -1) {
                    armCtrl.rotateCtrl[i] = ANTICLOCKWISE_1_DEGREE;
                } else if (need == -2) {
                    armCtrl.rotateCtrl[i] = ANTICLOCKWISE_2_DEGREE;
                } else if (need >= -4) {
                    armCtrl.rotateCtrl[i] = ANTICLOCKWISE_3_DEGREE;
                } else {
                    armCtrl.rotateCtrl[i] = ANTICLOCKWISE_5_DEGREE;
                }
            }
        }
        if (end) {
            break;
        }
        sendArmCtrl(armCtrl);
        sleep_ms(sleepTime);
    }
    while (sw(0)) {
        struct ArmCtrl armCtrl = {address};
        for (int i = 0; i < AXLE_NUM; ++i) {
            armCtrl.rotateCtrl[i] = NO_ROTATE;
        }
        bool end = true;
        if (target.pos != armPos) {
            end = false;
            if (target.pos - armPos >= 10) {
                armCtrl.moveCtrl = RIGHT_FASTEST;
            } else if (target.pos - armPos >= 5) {
                armCtrl.moveCtrl = RIGHT_MIDDLE;
            } else if (target.pos - armPos >= 3) {
                armCtrl.moveCtrl = RIGHT_SLOWEST;
            } else if (armPos - target.pos >= 10) {
                armCtrl.moveCtrl = LEFT_FASTEST;
            } else if (armPos - target.pos >= 5) {
                armCtrl.moveCtrl = LEFT_MIDDLE;
            } else if (armPos - target.pos >= 3) {
                armCtrl.moveCtrl = LEFT_SLOWEST;
            }
        } else {
            armCtrl.moveCtrl = NO_MOVE;
        }
        if (end) {
            break;
        }
        sendArmCtrl(armCtrl);
        sleep_ms(sleepTime);
    }
    sendEmptyArmCtrl(address);
}

/**
* 机械臂1抓取左侧传送带的角度
* 306 18 293 0 136 54
* 54 18 -67 0 136 54
*/

void arm1PickUpPlace() {
    // struct ArmState state = {{53, 21, -76, 0, 139, 40}};
    // struct ArmState state = {{54, 18, -67, 0, 136, 54}};
    struct ArmState state = {{54, 18, -67, 0, 136, 126}};
    armTo(ARM_1_ADDRESS, state);
}

/**
* 机械臂2抓取左侧传送带的角度
* 270 22 292 0 135 0
*/

void arm2PickUpPlace() {
    struct ArmState state = {{90, 22, 292, 0, 135, 0}, 0};
    armTo(ARM_2_ADDRESS, state);
}

/**
* 机械臂2抬高角度
* 270 320 292 0 135 0
*/
void arm2TransformPlace() {
    struct ArmState state = {{armNow[0], 320, armNow[2], armNow[3], armNow[4], armNow[5]}, armPos};
    armTo(ARM_2_ADDRESS, state);
}

/**
* 机械臂2抬高角度
* 90 9 293 0 148 0
*/
enum ChestColor arm2PickDownPlace() {
    enum ChestColor color = getChestColor();
    int pos = 0;
    const int RedOrBluePos = 230;
    switch (color) {
        case RED:
            pos = RedOrBluePos;
            break;
        case GREEN:
            break;
        case BLUE:
            pos = -RedOrBluePos;
    }
    struct ArmState state = {{270, 9, 293, 0, 148, 0}, pos};
    armTo(ARM_2_ADDRESS, state);
    return color;
}

enum ChestColor arm2TransformChest() {
    clearData();
    LOG("DEBUG", "%s", "Chest2 out");
    sendChestCtrl(NO_CHEST, OUT_CHEST, NO_CHEST);
    sleep_ms(2);
    LOG("DEBUG", "%s", "OPEN_BELT3");
    sendConveyerBeltCtrl(NO_BELT, OPEN_BELT);
    sleep_ms(2);
    LOG("DEBUG", "%s", "arm2PickUpPlace");
    arm2PickUpPlace();
    clearData();
    sleep_ms(6000);
    LOG("DEBUG", "%s", "arm2 suck");
    sendSuckCtrl(NO_SUCK_ACTION, DO_SUCK, NO_SUCK_ACTION);
    // printf("%c\n", receiveChar());
    LOG("DEBUG", "%s", "arm2TransformPlace");
    arm2TransformPlace();
    LOG("DEBUG", "%s", "arm2PickDownPlace");
    enum ChestColor color = arm2PickDownPlace();
    LOG("DEBUG", "%s", "arm2 unsuck");
    sendSuckCtrl(NO_SUCK_ACTION, UNDO_SUCK, NO_SUCK_ACTION);
    LOG("DEBUG", "%s", "arm2TransformPlace");
    arm2TransformPlace();
    clearData();
    return color;
}

void arm1Up() {
    struct ArmState state = {{0, 0, -90, 0, 180, 0}};
    armTo(ARM_1_ADDRESS, state);
}

void arm2Reset() {
    struct ArmState state;
    for (int i = 0; i < AXLE_NUM; ++i) {
        state.degree[i] = 0;
    }
    state.pos = 0;
    armTo(ARM_2_ADDRESS, state);
}

void sendSuckCtrl(enum ArmSuckCtrl ctrl1, enum ArmSuckCtrl ctrl2, enum ArmSuckCtrl ctrl3) {
    sendChar(MODBUS_PREAMBLE);
    sendChar(ARM_SUCK_ADDRESS);
    sendChar(ctrl1);
    sendChar(ctrl2);
    sendChar(ctrl3);
    for (int i = 0; i < 4; ++i) {
        sendChar('0');
    }
}
