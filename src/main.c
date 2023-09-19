#include <stdbool.h>
#include "init.h"
#include "mechanical_arm.h"

int main() {
	init();
	struct ArmCtrl ctrl = {ARM_1_ADDRESS, CLOCKWISE_FASTEST, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE, NO_ROTATE};
	while (true) {
		sendArmCtrl(ctrl);
	}
	return 0;
}
