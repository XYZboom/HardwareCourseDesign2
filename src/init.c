//
// Created by XYZboom on 2023/9/19.
//

#include "init.h"
#include "constants.h"
#include "logger.h"

#ifdef SIMULATION
#include <windows.h>
#include <assert.h>
HANDLE COM_HANDLE;
#endif

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
#ifdef SIMULATION
	DCB dcb;
	BOOL flag = FALSE;
	COMMTIMEOUTS commTimeouts;
	COM_HANDLE = INVALID_HANDLE_VALUE;
	COM_HANDLE = CreateFile(SIMULATION_COM, GENERIC_READ | GENERIC_WRITE, 0, NULL,
	                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == COM_HANDLE) {
		LOG("DEBUG", "%s %s %s", "Open", SIMULATION_COM, "Fail");
		exit(-1);
	}
	flag = SetupComm(COM_HANDLE, DEBUG_COM_BUFFER_SIZE, DEBUG_COM_BUFFER_SIZE);
	if (!flag) {
		LOG("DEBUG", "%s %s %s", "Setup", SIMULATION_COM, "Fail");
		exit(-1);
	}

	commTimeouts.ReadIntervalTimeout = MAXWORD;
	commTimeouts.ReadTotalTimeoutMultiplier = 0;
	commTimeouts.ReadTotalTimeoutConstant = 0;
	commTimeouts.WriteTotalTimeoutMultiplier = 1;
	commTimeouts.WriteTotalTimeoutConstant = 1;
	flag = SetCommTimeouts(COM_HANDLE, &commTimeouts);
	if (!flag) {
		LOG("DEBUG", "%s %s", "Setup timeout settings Fail COM:", SIMULATION_COM);
		exit(-1);
	}

	GetCommState(COM_HANDLE, &dcb);
	dcb.BaudRate = DEBUG_COM_BAUD_RATE;
	dcb.ByteSize = 8;
	dcb.Parity = ODDPARITY;
	dcb.StopBits = ONESTOPBIT;
	flag = SetCommState(COM_HANDLE, &dcb);
	if (!flag) {
		LOG("DEBUG", "%s %s %s", "Init", SIMULATION_COM, "Fail");
		exit(-1);
	}
#else
	initUart();
#endif
}

#ifdef SIMULATION
void finalize() {
	assert(COM_HANDLE != NULL);
	if (CloseHandle(COM_HANDLE)) {
		LOG("DEBUG", "%s %s", SIMULATION_COM, "Has Closed");
		return;
	}
	LOG("DEBUG", "%s %s", SIMULATION_COM, "Close Failed");
}
#endif