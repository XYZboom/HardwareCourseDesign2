#include "constants.h"

#include "gpio.h"

#include "car_and_door.h"
#include "conveyer_belt.h"
#include "logger.h"
#include <stdbool.h>
#include <stdio.h>

#include "uart.h"

#include "init.h"
#include "mechanical_arm.h"
#include "chest.h"
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
        DATA_2 = DATA_2 | 0x00006000;
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
    }
#ifdef SIMULATION
    finalize();
#endif
    return 0;
}
