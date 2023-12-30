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

unsigned char receiveChar() {
#ifdef SIMULATION
	assert(COM_HANDLE != NULL);
	unsigned char recv_data[1];
	DWORD recv_len;
	BOOL flag = FALSE;
	flag = ReadFile(COM_HANDLE, recv_data, sizeof(recv_data), &recv_len, NULL);
	if (!flag) {
		LOG("DEBUG", "%s %s", "Receive Message Error,COM:", SIMULATION_COM);
	} else if (recv_len != 1) {
		LOG("DEBUG", "%s %s", "Receive Message Length Error,COM:", SIMULATION_COM);
	}
	return recv_data[0];
#else
	while ((UART_CHANNEL_STS_REG0 & 0x2) == 0x2);
	const char data = UART_Tx_Rx_FIFO0;
	return data;
#endif
}

void sendString(const char *string) {
	while (*string) {
		sendChar(*string++);
	}
}
