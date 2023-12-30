//
// Created by XYZboom on 2023/9/19.
//

#include <stdbool.h>
#include "mechanical_arm.h"
#include "uart.h"
#include "constants.h"
#include "time_utils.h"

void sendArmCtrl(struct ArmCtrl ctrl) {
    sendChar(MODBUS_PREAMBLE);
    sendChar(ctrl.address);
    for (int i = 0; i < AXLE_NUM; ++i) {
        sendChar(ctrl.rotateCtrl[i]);
    }
    if (ctrl.address == ARM_1_ADDRESS) {
        sendChar(NO_MOVE);
    } else {
        sendChar(ctrl.moveCtrl);
    }
}

int arm1Now[] = {0, 0, 0, 0, 0, 0};

void sendEmptyArmCtrl(enum ArmAddress address) {
    const struct ArmCtrl emptyCtrl = {address, {NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE}, NO_MOVE};
    sendArmCtrl(emptyCtrl);
}

void armTo(enum ArmAddress address, struct ArmState target) {
    sendEmptyArmCtrl(address);
    struct ArmCtrl armCtrl = {address};
    while (true) {
        bool end = true;
        for (int i = 0; i < AXLE_NUM; ++i) {
            int need = (target.degree[i] - arm1Now[i]) % 360;
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
                    arm1Now[i] += 5;
                } else if (need >= 3) {
                    armCtrl.rotateCtrl[i] = CLOCKWISE_3_DEGREE;
                    arm1Now[i] += 3;
                } else if (need == 2) {
                    armCtrl.rotateCtrl[i] = CLOCKWISE_2_DEGREE;
                    arm1Now[i] += 2;
                } else if (need == 1) {
                    armCtrl.rotateCtrl[i] = CLOCKWISE_1_DEGREE;
                    arm1Now[i] += 1;
                } else if (need == -1) {
                    armCtrl.rotateCtrl[i] = ANTICLOCKWISE_1_DEGREE;
                    arm1Now[i] -= 1;
                } else if (need == -2) {
                    armCtrl.rotateCtrl[i] = ANTICLOCKWISE_2_DEGREE;
                    arm1Now[i] -= 2;
                } else if (need >= -4) {
                    armCtrl.rotateCtrl[i] = ANTICLOCKWISE_3_DEGREE;
                    arm1Now[i] -= 3;
                } else {
                    armCtrl.rotateCtrl[i] = ANTICLOCKWISE_5_DEGREE;
                    arm1Now[i] -= 5;
                }
            }
        }
        if (end) {
            break;
        }
        sendArmCtrl(armCtrl);
        sleep_ms(80);
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

void arm1Up() {
    struct ArmState state = {{0, 0, -90, 0, 180, 0}};
    armTo(ARM_1_ADDRESS, state);
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
