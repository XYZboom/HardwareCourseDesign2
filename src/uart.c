//
// Created by XYZboom on 2023/9/19.
//

#include "uart.h"
#include "constants.h"

void sendChar(unsigned char data) {
	// 队列不满时发送
	while ((UART_CHANNEL_STS_REG0 & 0x10) == 0x10);
	UART_Tx_Rx_FIFO0 = data;
}

void sendString(const char *string) {
	while (*string) {
		sendChar(*string++);
	}
}
