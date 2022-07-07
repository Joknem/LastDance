#ifndef DJI_CAN_MOTORS_H
#define DJI_CAN_MOTORS_H

#include "ControllerPID/MotorPID.h"

class DjiMotor {
public:
    DjiMotor(){
        pid.SetTorqueLimit(0.80);
        pid.dce.kp = 12.0;
        pid.dce.ki = 1.223;
        pid.dce.kv = 3.0;
        pid.dce.kd = 1.23;
        pid.dce.setPointPos = 0;
        pid.SetEnable(true);
        
    }

    MotorPID pid;


private:

};





#endif