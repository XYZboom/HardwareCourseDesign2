//
// Created by XYZboom on 2023/9/20.
//

#ifndef DESIGN2_PROJECT_LED_H
#define DESIGN2_PROJECT_LED_H

#define LED_SINGLE_ADDRESS ((unsigned char) 0x37)
#define LED_TENS_ADDRESS   ((unsigned char) 0x38)
enum LEDCtrl {
	OFF_LED = 0x30,
	ON_LED
};

void showLed(int num);

#endif //DESIGN2_PROJECT_LED_H
