#include "CAN_Devices/Dji_CAN_motors.h"

extern DjiMotorGroup *djiMotorGroupLowerId;
extern DjiMotorGroup *djiMotorGroupHigherId;

void MotorRoutinsTask(void *pvArgc)
{
    // TODO

    // motor init
    DjiCanMotorsInit();
    for (;;)
    {
        if (1)
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