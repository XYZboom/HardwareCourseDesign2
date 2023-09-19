//
// Created by XYZboom on 2023/9/19.
//

#include "mechanical_arm.h"
#include "uart.h"
#include "constants.h"

void sendArmCtrl(struct ArmCtrl ctrl) {
	sendChar(MODBUS_PREAMBLE);
	sendChar(ctrl.address);
	sendChar(ctrl.ctrl1);
	sendChar(ctrl.ctrl2);
	sendChar(ctrl.ctrl3);
	sendChar(ctrl.ctrl4);
	sendChar(ctrl.ctrl5);
	sendChar(ctrl.ctrl6);
	if (ctrl.address == ARM_1_ADDRESS) {
		sendChar(NO_MOVE);
	} else {
		sendChar(ctrl.moveCtrl);
	}
}
