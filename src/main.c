#include "car_and_door.h"
#include "conveyer_belt.h"
#include "logger.h"
#include <stdbool.h>
#include "uart.h"

#include "init.h"
#include "mechanical_arm.h"
#include "chest.h"
#include "time_utils.h"

int main() {
    init();
    struct ArmCtrl ctrl = {ARM_1_ADDRESS, {NO_ROTATE, CLOCKWISE_5_DEGREE, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE}};
    struct CarAndDoorCtrl cdCtrl = defaultCarAndDoorCtrl();
    cdCtrl.door1 = DOOR_UP;
    cdCtrl.tower = TOWER_CLOCKWISE;
    while (true) {
        /*for (int i = 0; i < 100; ++i) {
            showLed(i);
            sleep_ms(200);
        }*/
        /*for (int i = 0; i < 20; ++i) {
            sendCarAndDoorCtrl(cdCtrl);
        }*/
        for (int i = 0; i < 5; ++i) {
            arm2TransformChest();
            sleep_ms(1000);
        }
    }
#ifdef SIMULATION
    finalize();
#endif
    return 0;
}
