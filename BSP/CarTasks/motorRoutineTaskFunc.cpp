#include "CarTasks.h"

#include "CAN_Devices/Dji_CAN_motors.h"
#include "tim.h"

extern DjiMotorGroup djiMotorGroupLowerId;
extern DjiMotorGroup djiMotorGroupHigherId;

extern osMessageQId qMotorTimeupHandle;

static void motorTimeupCallback(TIM_HandleTypeDef * htim){
    char descript[2] = "n";
    // if (tim_queue_enable){
    //   // ST_LOGD("T");
        xQueueSendFromISR(qMotorTimeupHandle, descript, NULL);
    // }

    
}
extern volatile float angle_test;
void motorRoutineTaskFunc(void const * argument)
{
    // TODO
    char * __ptr;
    uint32_t cnt;
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
                djiMotorGroupLowerId.SetInput(i, angle_test, 0);
                // djiMotorGroupHigherId.SetInput(i, 0, 0);
            }
            djiMotorGroupLowerId.output();
            
            // djiMotorGroupHigherId->output();

            // odrive motor cal area


            // end process
            cnt ++;
            if (cnt > 200){
                // ST_LOGI("m1 out : (%.2f,%.2f)\t(%.2f,%.2f)\t(%.2f,%.2f)",   djiMotorGroupLowerId.motor[0].angle, djiMotorGroupLowerId.motor[0].output,
                //                                                             djiMotorGroupLowerId.motor[1].angle, djiMotorGroupLowerId.motor[1].output,
                //                                                             djiMotorGroupLowerId.motor[2].angle, djiMotorGroupLowerId.motor[2].output);
                cnt = 0;
            }
        }
    }
}