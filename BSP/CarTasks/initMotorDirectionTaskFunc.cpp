#include "CarTasks.h"
#include "CAN_Devices/bsp_can.h"
#include "CAN_Devices/Dji_CAN_motors.h"
#include "CAN_Devices/Odrive_CAN_motors.h"
#include "main.h"

enum _init_flags{
    HALL1_OK = 1 << 0ul,
    HALL2_OK = 1 << 1ul,
    HALL3_OK = 1 << 2ul,
}init_flags;
int init_status[3];

EventGroupHandle_t init_event_handle = xEventGroupCreate();

void initMotorDirectionTaskFunc(void const * argument){
    for(;;){
        for(int i = 0; i < 3; i++){
            // init_status[0] = HAL_GPIO_ReadPin(HALL_INPUT_1_GPIO_Port, HALL_INPUT_1_Pin);
        }
        osDelay(5);
    }
    vTaskDelete(NULL);
}