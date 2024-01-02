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
    state.carX = -10;
    state.carAngle = 0;
    state.carY = 0;
    state.door1Height = 0;
    state.door2Height = 0;
    state.door3Height = 0;
    state.carLiftingRodHeight = 0;
    while (true) {
        while (sw(0)) {
            /*for (int i = 0; i < 100; ++i) {
            showLed(i);
            sleep_ms(200);
        }*/
            /*for (int i = 0; i < 20; ++i) {
            sendCarAndDoorCtrl(cdCtrl);
        }*/
            carAndDoorTo(state);
            for (int i = 0; i < 5; ++i) {
                arm2TransformChest();
                sleep_ms(1000);
            }
        }
        if (!sw(0)) {
            // 00 机械臂手动
            if (!sw(1) && !sw(2)) {
                struct ArmCtrl arm_ctrl = defaultArmCtrl(ARM_2_ADDRESS);
                if (isBtn8On()) {
                    arm_ctrl.rotateCtrl[0] = rotateCtrlFromSpeed(getSpeedType(), true);
                } else if (isBtn9On()) {
                    arm_ctrl.rotateCtrl[0] = rotateCtrlFromSpeed(getSpeedType(), false);
                }
                if (isBtnOn(BTNL)) {
                    arm_ctrl.rotateCtrl[1] = rotateCtrlFromSpeed(getSpeedType(), true);
                } else if (isBtnOn(BTNR)) {
                    arm_ctrl.rotateCtrl[1] = rotateCtrlFromSpeed(getSpeedType(), false);
                }
                if (isBtnOn(BTNU)) {
                    arm_ctrl.rotateCtrl[2] = rotateCtrlFromSpeed(getSpeedType(), true);
                } else if (isBtnOn(BTND)) {
                    arm_ctrl.rotateCtrl[2] = rotateCtrlFromSpeed(getSpeedType(), false);
                }
                if (isBtn8On() || isBtn9On() || isBtnOn(BTNL) || isBtnOn(BTNR) || isBtnOn(BTNU) ||
                    isBtnOn(BTND)) {
                    sendArmCtrl(arm_ctrl);
                    sleep_ms(80);
                }
            } else if (sw(1) && !sw(2)) {
                struct ArmCtrl arm_ctrl = defaultArmCtrl(ARM_2_ADDRESS);

            }
        }
    }
#ifdef SIMULATION
    finalize();
#endif
    return 0;
}
