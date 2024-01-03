//
// Created by lenovo on 2024/1/2.
//

#include "gpio.h"

#include "constants.h"

void initGPIO() {
#ifdef SIMULATION
#else
	//配置及初始化MIO07引脚的相关寄存器，MIO07作为LED灯控制的输出引脚
	MIO_PIN_7 = 0x00003600;
	DIRM_0 = DIRM_0|0x00000080;
	OEN_0 = OEN_0|0x00000080;
    //配置及初始化MIO50、MIO51引脚的相关寄存器，MIO50、MIO51作为按键输入引脚
    MIO_PIN_50 = 0x00003600;
    MIO_PIN_51 = 0x00003600;
    DIRM_1 = DIRM_1 & 0xFFF3FFFF;
    //初始化EMIO54~EMIO58的引脚，它们对应BTNU、BTND、BTNL、BTNR、BTNC按键，输入
    DIRM_2 = DIRM_2 & 0xFFFFFFE0;
    //初始化EMIO59~EMIO66的引脚，它们对应SW7~SW0拨动开关，输入
	DIRM_2 = DIRM_2 & 0xFFFFE01F;
	//初始化EMIO67~EMIO74的引脚，它们对应LED7~LED0，输出
	DIRM_2 = DIRM_2 | 0x001FE000;
	OEN_2 = OEN_2 | 0x001FE000;
#endif
}

bool sw(const int num) {
#ifndef SIMULATION
	const unsigned int data = DATA_2_RO;
    return (data & (0x1000 >> num)) == (0x1000 >> num);
#else
	return true;
#endif
}

bool isBtn8On() {
	return (DATA_1_RO & 0x40000) == 0x40000;
}

bool isBtn9On() {
	return (DATA_1_RO & 0x80000) == 0x80000;
}

bool isBtnOn(const enum BtnType btnType) {
	const unsigned int data = DATA_2_RO;
	return (data & (0x10 >> btnType)) == (0x10 >> btnType);
}
