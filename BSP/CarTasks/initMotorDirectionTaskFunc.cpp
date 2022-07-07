#include "CarTasks.h"


void initMotorDirectionTaskFunc(void const * argument){
    for(;;){
        osDelay(1000);
    }
    vTaskDelete(NULL);
}