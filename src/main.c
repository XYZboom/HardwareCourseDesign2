#include <car_and_door.h>
#include <conveyer_belt.h>
#include <logger.h>
#include <stdbool.h>
#include <uart.h>

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
        LOG("DEBUG", "%s", "Chest2 out");
        sendChestCtrl(NO_CHEST, OUT_CHEST, NO_CHEST);
        LOG("DEBUG", "%s", "OPEN_BELT3");
        sendConveyerBeltCtrl(NO_BELT, OPEN_BELT);
        LOG("DEBUG", "%s", "arm2PickUpPlace");
        arm2PickUpPlace();
        sleep_ms(9000);
        LOG("DEBUG", "%s", "arm2 suck");
        sendSuckCtrl(NO_SUCK_ACTION, DO_SUCK, NO_SUCK_ACTION);
        printf("%c\n", receiveChar());
        LOG("DEBUG", "%s", "arm2TransformPlace");
        arm2TransformPlace();
        LOG("DEBUG", "%s", "arm2PickDownPlace");
        arm2PickDownPlace();
        LOG("DEBUG", "%s", "arm2 unsuck");
        sendSuckCtrl(NO_SUCK_ACTION, UNDO_SUCK, NO_SUCK_ACTION);
        LOG("DEBUG", "%s", "arm1PickUpPlace");
        arm1PickUpPlace();
        sleep_ms(6000);
        sleep_ms(2500000);
        sendArmCtrl(ctrl);
        sendArmCtrl(ctrl);
        sendArmCtrl(ctrl);
        sendSuckCtrl(UNDO_SUCK, NO_SUCK_ACTION, NO_SUCK_ACTION);
        sendChestCtrl(OUT_CHEST, OUT_CHEST, OUT_CHEST);
        sleep_ms(2500);
    }
#ifdef SIMULATION
    finalize();
#endif
    return 0;
}
