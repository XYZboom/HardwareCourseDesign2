//
// Created by XYZboom on 2023/11/13.
//

#ifndef DESIGN2_PROJECT_LOGGER_H
#define DESIGN2_PROJECT_LOGGER_H

#ifdef SIMULATION

#include <stdio.h>

#define LOG(TAG, FORMAT, ...) fprintf(stdout, "[" TAG "] %s %s [%s:%d] " FORMAT "\r\n", __DATE__, __TIME__, __FILE__, __LINE__, __VA_ARGS__)
#endif

#endif //DESIGN2_PROJECT_LOGGER_H
