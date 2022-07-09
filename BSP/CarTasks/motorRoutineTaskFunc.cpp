#include "CarTasks.h"

#include "CAN_Devices/Dji_CAN_motors.h"
#include "CAN_Devices/Odrive_CAN_motors.h"
#include "tim.h"


extern osMessageQId qMotorTimeupHandle;

motors_output_t motor_values;


DjiMotorGroup  djiMotorGroupLowerId(&hfdcan2, true);
DjiMotorGroup  djiMotorGroupHigherId(&hfdcan2, false);

static void motorTimeupCallback(TIM_HandleTypeDef * htim){
    char descript[2] = "n";
    // if (tim_queue_enable){
        xQueueSendFromISR(qMotorTimeupHandle, descript, NULL);
    // }

    
}


void motorRoutineTaskFunc(void const * argument)
{
    // TODO
    char * __ptr;
    uint32_t cnt = 0;
    // motor init
    // Odrive_CAN_motors odrv_motors[3]{
    //     Odrive_CAN_motors(&hfdcan2, 1),
    //     Odrive_CAN_motors(&hfdcan2, 2),
    //     Odrive_CAN_motors(&hfdcan2, 3)
    // };


    // start tim
    HAL_TIM_RegisterCallback(&htim6, HAL_TIM_PERIOD_ELAPSED_CB_ID, motorTimeupCallback);
    HAL_TIM_Base_Start_IT(&htim6);
    
    //? test can
    motor_values.vel_motors[0] = 5.0;

    for (;;)
    {

        if (xQueueReceive(qMotorTimeupHandle, &__ptr, portMAX_DELAY) == pdTRUE)
        { // wait for timer semaphore
            // dji motor cal area
            for (int i = 0; i < 4; i++)
            {
                djiMotorGroupLowerId.SetInput(i, motor_values.rudder_motors[i],
                    motor_values.type == CTRL_TYPE_ANGLE ? 
                    MotorPID::PENG_CTRL_TYPE_POSITION : MotorPID::PENG_CTRL_TYPE_SPEED);
                // djiMotorGroupHigherId.SetInput(i, 0, 0);
            }
            djiMotorGroupLowerId.output();
            // djiMotorGroupHigherId->output();

            // odrive motor cal area
            for (int i = 0; i < 3; i++){
                odrv_motors[i].setSpeed(motor_values.vel_motors[i]);
            }


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