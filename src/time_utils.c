//
// Created by XYZboom on 2023/9/20.
//

#include "time_utils.h"
#include <unistd.h>

void sleep_ms(long ms) {
    usleep(ms * 1000);
}
