//
// Created by XYZboom on 2023/9/19.
//

#include "uart.h"
#include "constants.h"

#ifdef SIMULATION

#include <assert.h>
#include "logger.h"

#endif

void sendChar(unsigned char data) {
#ifdef SIMULATION
	assert(COM_HANDLE != NULL);
	BOOL flag = FALSE;
	DWORD writeSize = 0;
	flag = -WriteFile(COM_HANDLE, &data, sizeof(data), &writeSize, NULL);
	if (!flag) {
		LOG("DEBUG", "%s %s", "Send Message Error,COM:", SIMULATION_COM);
	}
#else
	// 队列不满时发送
	while ((UART_CHANNEL_STS_REG0 & 0x10) == 0x10);
	UART_Tx_Rx_FIFO0 = data;
#endif
}

void sendString(const char *string) {
	while (*string) {
		sendChar(*string++);
	}
}
