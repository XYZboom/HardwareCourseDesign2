#include "logger.h"

#include "constants.h"
#include "manual.h"
#include "gpio.h"
#include "car_and_door.h"
#include <stdbool.h>
#include "init.h"
#include "mechanical_arm.h"
#include "time_utils.h"

int main() {
    init();
    struct ArmCtrl ctrl = {ARM_1_ADDRESS, {NO_ROTATE, CLOCKWISE_5_DEGREE, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE}};
    struct CarAndDoorState state;
    state.carX = carRedEndX;
    state.carAngle = 90;
    state.carY = carEndY;
    state.door1Height = 10;
    state.door2Height = 10;
    state.door3Height = 10;
    state.carLiftingRodHeight = 0;
    bool armRotateType = false;
    while (true) {
        if (sw(0)) {
            /*for (int i = 0; i < 100; ++i) {
            showLed(i);
            sleep_ms(200);
        }*/
            /*for (int i = 0; i < 20; ++i) {
            sendCarAndDoorCtrl(cdCtrl);
        }*/
            /*bool arrive;
            while (!arrive) {
                struct CarAndDoorState stateTo = getTarget(&arrive, state);
                carAndDoorTo(stateTo);
            }
            state.carY = 32.9f;
            carAndDoorTo(state);*/
            LOG("DEBUG", "%s", "carAndDoorTo MainStart");
            carAndDoorTo(MainStart);
            for (int i = 0; i < 5; ++i) {
#ifndef SIMULATION
                DATA_2 = (DATA_2 & ~0x1FE000) | 0x100000;
#endif
                enum ChestColor color = arm2TransformChest();
                carTransform(color);
                arm2Reset();
                if (!sw(0)) {
#ifndef SIMULATION
                    DATA_2 = (DATA_2 & ~0x1FE000) | 0x000000;
#endif
                    break;
                }
                sleep_ms(9000);
#ifndef SIMULATION
                DATA_2 = (DATA_2 & ~0x1FE000) | 0x000000;
#endif
            }
        }
        if (!sw(0)) {
            // 00 机械臂手动
            if (!sw(1) && !sw(2)) {
                struct ArmCtrl arm_ctrl = defaultArmCtrl(ARM_2_ADDRESS);
                if (isBtnOn(BTNC)) {
                    armRotateType = !armRotateType;
                    sleep_ms(800);
                }
                if (isBtn8On()) {
                    arm_ctrl.rotateCtrl[0 + (armRotateType ? 3 : 0)] = armRotateCtrlFromSpeed(getSpeedType(), true);
                } else if (isBtn9On()) {
                    arm_ctrl.rotateCtrl[0 + (armRotateType ? 3 : 0)] = armRotateCtrlFromSpeed(getSpeedType(), false);
                }
                if (isBtnOn(BTNL)) {
                    arm_ctrl.rotateCtrl[1 + (armRotateType ? 3 : 0)] = armRotateCtrlFromSpeed(getSpeedType(), true);
                } else if (isBtnOn(BTNR)) {
                    arm_ctrl.rotateCtrl[1 + (armRotateType ? 3 : 0)] = armRotateCtrlFromSpeed(getSpeedType(), false);
                }
                if (isBtnOn(BTNU)) {
                    arm_ctrl.rotateCtrl[2 + (armRotateType ? 3 : 0)] = armRotateCtrlFromSpeed(getSpeedType(), true);
                } else if (isBtnOn(BTND)) {
                    arm_ctrl.rotateCtrl[2 + (armRotateType ? 3 : 0)] = armRotateCtrlFromSpeed(getSpeedType(), false);
                }
                if (isBtn8On() || isBtn9On() || isBtnOn(BTNL) || isBtnOn(BTNR) || isBtnOn(BTNU) ||
                    isBtnOn(BTND)) {
                    sendArmCtrl(arm_ctrl);
                    sleep_ms(80);
                }
            } else if (sw(1) && !sw(2)) {
                struct ArmCtrl arm_ctrl = defaultArmCtrl(ARM_2_ADDRESS);
                if (isBtnOn(BTNL)) {
                    arm_ctrl.moveCtrl = armMoveCtrlFromSpeed(getSpeedType(), true);
                } else if (isBtnOn(BTNR)) {
                    arm_ctrl.moveCtrl = armMoveCtrlFromSpeed(getSpeedType(), false);
                }
                if (isBtn8On() || isBtn9On() || isBtnOn(BTNL) || isBtnOn(BTNR) || isBtnOn(BTNU) ||
                    isBtnOn(BTND)) {
                    sendArmCtrl(arm_ctrl);
                    sleep_ms(80);
                }
            } else if (!sw(1) && sw(2)) {
                struct CarAndDoorCtrl car_and_door_ctrl = defaultCarAndDoorCtrl();
                if (isBtn8On()) {
                    car_and_door_ctrl.liftingRod = UP;
                } else if (isBtn9On()) {
                    car_and_door_ctrl.liftingRod = DOWN;
                }
                if (isBtnOn(BTNL)) {
                    car_and_door_ctrl.y = LEFT;
                } else if (isBtnOn(BTNR)) {
                    car_and_door_ctrl.y = RIGHT;
                }
                if (isBtnOn(BTNU)) {
                    car_and_door_ctrl.x = FORWARD;
                } else if (isBtnOn(BTND)) {
                    car_and_door_ctrl.x = BACKWARD;
                }
                if (isBtnOn(BTNC)) {
                    car_and_door_ctrl.tower = TOWER_ANTICLOCKWISE + getSpeedType();
                }
                if (isBtn8On() || isBtn9On() || isBtnOn(BTNL) || isBtnOn(BTNR) || isBtnOn(BTNU) ||
                    isBtnOn(BTND) || isBtnOn(BTNC)) {
                    sendCarAndDoorCtrl(car_and_door_ctrl);
                    sleep_ms(80);
                }
            } else if (sw(1) && sw(2)) {
                if (isBtnOn(BTNC)) {
                    arm2Reset();
                    bool arrive;
                    while (!arrive) {
                        carAndDoorTo(getTarget(&arrive, Reset));
                    }
                    sleep_ms(800);
                }
            }
        }
#ifndef SIMULATION
        // int ledCtrl = 0x100000 >> getCarPositionType(CarAndDoorStateNow);
        // DATA_2 = (DATA_2 & ~0x1FE000) | ledCtrl;
#endif
    }
#ifdef SIMULATION
    finalize();
#endif
    return 0;
}
