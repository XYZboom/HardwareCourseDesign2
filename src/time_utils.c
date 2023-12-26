//
// Created by XYZboom on 2023/9/20.
//

#include "time_utils.h"
#include <unistd.h>

void sleep_ms(long ms) {
#ifdef SIMULATION
    ms *= 0.7f;
#else
    ms *= 0.4f;
#endif
    usleep(ms * 1000);
}
