#include "CarTasks.h"

#include "CAN_Devices/Dji_CAN_motors.h"
#include "tim.h"

extern DjiMotorGroup *djiMotorGroupLowerId;
extern DjiMotorGroup *djiMotorGroupHigherId;

extern osMessageQId qMotorTimeupHandle;

static void motorTimeupCallback(TIM_HandleTypeDef * htim){
    char descript[2] = "n";
    // if (tim_queue_enable){
    //   // ST_LOGD("T");
        xQueueSendFromISR(qMotorTimeupHandle, descript, NULL);
    // }

    
}

void MotorRoutinsTask(void *pvArgc)
{
    // TODO
    char * __ptr;
    // motor init
    DjiCanMotorsInit();

    // start tim
    HAL_TIM_RegisterCallback(&htim6, HAL_TIM_PERIOD_ELAPSED_CB_ID, motorTimeupCallback);
    HAL_TIM_Base_Start_IT(&htim6);
    for (;;)
    {

        if (xQueueReceive(qMotorTimeupHandle, &__ptr, portMAX_DELAY) == pdTRUE)
        { // wait for timer semaphore
            // dji motor cal area
            for (int i = 0; i < 4; i++)
            {
                djiMotorGroupLowerId->SetInput(i, 0, 0);
                djiMotorGroupHigherId->SetInput(i, 0, 0);
            }
            djiMotorGroupLowerId->output();
            djiMotorGroupHigherId->output();

            // odrive motor cal area
        }
    }
}