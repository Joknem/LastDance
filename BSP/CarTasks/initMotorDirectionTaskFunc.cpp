#include "CarTasks.h"
#include "CAN_Devices/bsp_can.h"
#include "CAN_Devices/Dji_CAN_motors.h"
#include "CAN_Devices/Odrive_CAN_motors.h"
#include "main.h"

enum _init_flags{
    HALL1_OK = 1 << 2ul,
    HALL2_OK = 1 << 1ul,
    HALL3_OK = 1 << 0ul,
}init_flags;
#define LOW_LEVEL 0
#define HALL_OK 1
#define ALL_OK 0b111

uint16_t HALL_PIN[3] = {HALL_INPUT_1_Pin, 
                        HALL_INPUT_2_Pin, 
                        HALL_INPUT_3_Pin
                        };
GPIO_TypeDef *HALL_Port[3]= {GPIOA,
                             GPIOA,
                             GPIOA
                            };      
int init_status[3] = {1, 1, 1};

extern DjiMotorGroup  djiMotorGroupLowerId;


EventGroupHandle_t init_event_handle = xEventGroupCreate();

void initMotorDirectionTaskFunc(void const * argument){
    EventBits_t ret_val;
    //TODO: give dji initial speed
    for(int i = 0; i < 3; i++){
        // djiMotorGroupLowerId.SetInput();
        }
    for(;;){
        for(int i = 0; i < 3; i++){
            init_status[i] = HAL_GPIO_ReadPin(HALL_Port[i], HALL_PIN[i]);
            if(init_status[i] == LOW_LEVEL){
                xEventGroupSetBits(init_event_handle, HALL_OK << i);
                //TODO: stop relevant 2006 and get base angle
            }
        }
        ret_val = xEventGroupWaitBits(init_event_handle, HALL1_OK|HALL2_OK|HALL3_OK, pdFAIL, pdTRUE, 5);
        if(ret_val & ALL_OK){
            //TODO: angle control
            vTaskDelete(NULL);
            }
        osDelay(5);
    }
}