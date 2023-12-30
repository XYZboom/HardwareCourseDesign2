#include <logger.h>
#include <stdbool.h>

#include "init.h"
#include "mechanical_arm.h"
#include "chest.h"
#include "time_utils.h"

int main() {
    init();
    struct ArmCtrl ctrl = {ARM_1_ADDRESS, {NO_ROTATE, CLOCKWISE_5_DEGREE, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE}};
    while (true) {
        /*for (int i = 0; i < 100; ++i) {
            showLed(i);
            sleep_ms(200);
        }*/
        LOG("DEBUG", "%s", "sendChestCtrl 1 out");
        sendChestCtrl(OUT_CHEST, NO_CHEST, NO_CHEST);
        LOG("DEBUG", "%s", "arm1Up");
        arm1Up();
        sleep_ms(1000);
        LOG("DEBUG", "%s", "arm1PickUpPlace");
        arm1PickUpPlace();
        sleep_ms(6000);
        sendSuckCtrl(DO_SUCK, NO_SUCK_ACTION, NO_SUCK_ACTION);
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
