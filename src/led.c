//
// Created by XYZboom on 2023/9/20.
//

#include "led.h"
#include "uart.h"
#include "constants.h"

unsigned char ledMap[10][7] = {
		// 0 f e d c b a
		{OFF_LED, ON_LED, ON_LED, ON_LED, ON_LED, ON_LED, ON_LED},
		// 1 c b
		{OFF_LED, OFF_LED, OFF_LED, OFF_LED, ON_LED, ON_LED, OFF_LED},
		// 2 g e d b a
		{ON_LED, OFF_LED, ON_LED, ON_LED, OFF_LED, ON_LED, ON_LED},
		// 3 g d c b a
		{ON_LED, OFF_LED, OFF_LED, ON_LED, ON_LED, ON_LED, ON_LED},
		// 4 g f c b
		{ON_LED, ON_LED, OFF_LED, OFF_LED, ON_LED, ON_LED, OFF_LED},
		// 5 g f d c a
		{ON_LED, ON_LED, OFF_LED, ON_LED, ON_LED, OFF_LED, ON_LED},
		// 6 g f e d c a
		{ON_LED, ON_LED, ON_LED, ON_LED, ON_LED, OFF_LED, ON_LED},
		// 7 c b a
		{OFF_LED, OFF_LED, OFF_LED, OFF_LED, ON_LED, ON_LED, ON_LED},
		// 8 all on
		{ON_LED, ON_LED, ON_LED, ON_LED, ON_LED, ON_LED, ON_LED},
		// 9 except e
		{ON_LED, ON_LED, OFF_LED, ON_LED, ON_LED, ON_LED, ON_LED},
};

void showLed(int num) {
	int single = num % 10;
	sendChar(MODBUS_PREAMBLE);
	sendChar(LED_SINGLE_ADDRESS);
	for (int i = 0; i < 7; ++i) {
		sendChar(ledMap[single][i]);
	}
	int tens = num / 10;
	sendChar(MODBUS_PREAMBLE);
	sendChar(LED_TENS_ADDRESS);
	for (int i = 0; i < 7; ++i) {
		sendChar(ledMap[tens][i]);
	}
}
