//
// Created by XYZboom on 2023/9/19.
//

#include "init.h"
#include "constants.h"

void initUart() {
	// 设置MIO[48]，电压3.3V
	MIO_PIN_48 = 0x000026E0;
	// 设置MIO[49]，电压3.3V
	MIO_PIN_49 = 0x000026E0;
	// 设置UART 20 分频
	UART_CLK_CTRL = 0x00001402;
	UART_CONTROL_REG0 = 0x00000017;
	// 模式：8位数据，1位停止，无校验
	UART_MODE_REG0 = 0x00000020;
	// 波特率 115200 b/s
	UART_BAUD_RATE_GEN_REG0 = 62;
	UART_BAUD_RATE_DIVIDER_REG0 = 6;
}

void init() {
	initUart();
}