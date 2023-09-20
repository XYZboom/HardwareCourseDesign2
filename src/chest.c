//
// Created by XYZboom on 2023/9/20.
//

#include "chest.h"
#include "uart.h"
#include "constants.h"

void sendChestCtrl(enum ChestCtrl ctrl1, enum ChestCtrl ctrl2, enum ChestCtrl ctrl3) {
	sendChar(MODBUS_PREAMBLE);
	sendChar(CHEST_CTRL_ADDRESS);
	sendChar(ctrl1);
	sendChar(ctrl2);
	sendChar(ctrl3);
	// 剩余4个控制命令为默认值
	for (int i = 0; i < 4; ++i) {
		sendChar(0);
	}
}
