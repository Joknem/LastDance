#include "CarTasks.h"


void initMotorDirectionTaskFunc(void * pvArgc){
    for(;;){
        osDelay(1000);
    }
    vTaskDelete(NULL);
}