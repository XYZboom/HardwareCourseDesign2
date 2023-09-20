#include <stdbool.h>
#include "init.h"
#include "mechanical_arm.h"
#include "chest.h"
#include "time_utils.h"

int main() {
	init();
	struct ArmCtrl ctrl = {ARM_1_ADDRESS, NO_ROTATE, CLOCKWISE_5_DEGREE, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE};
	while (true) {
		sendArmCtrl(ctrl);
		sendChestCtrl(OUT_CHEST, OUT_CHEST, OUT_CHEST);
		sleep_ms(2500);
	}
	return 0;
}
