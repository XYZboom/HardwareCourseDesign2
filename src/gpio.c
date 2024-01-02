//
// Created by lenovo on 2024/1/2.
//

#include "gpio.h"

#include "constants.h"

void initGPIO() {
#ifdef SIMULATION
#else
    DIRM_2 = DIRM_2 & 0xFFFFE01F;
    DIRM_2 = DIRM_2 | 0x001FE000;
    OEN_2 = OEN_2 | 0x001FE000;
#endif
}

bool sw(const int num) {
    unsigned int data = DATA_2_RO;
    return (data & (0x10000 >> num)) == (0x10000 >> num);
}
