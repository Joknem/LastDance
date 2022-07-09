#include "CarTasks.h"


void initMotorDirectionTaskFunc(void const * argument){
    for(;;){
        osDelay(1000);
    }

    xTaskCreate(userInputTaskFunc, "UserInput", 1024, NULL, osPriorityAboveNormal);
    vTaskDelete(NULL);
}