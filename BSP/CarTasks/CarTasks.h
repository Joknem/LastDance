#ifndef CARTASKS_H
#define CARTASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "cmsis_os.h"

void initMotorDirectionTaskFunc(void const * argument);
void motorRoutineTaskFunc(void const * argument);
void serialCmdProcTaskFunc(void const * argument);

#ifdef __cplusplus
}
#endif

#endif
