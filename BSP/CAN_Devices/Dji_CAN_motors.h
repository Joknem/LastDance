#ifndef DJI_CAN_MOTORS_H
#define DJI_CAN_MOTORS_H

#include "ControllerPID/MotorPID.h"



typedef struct{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
	int16_t circle;
} motor_measure_t;

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

private:
    MotorPID pid;

};



class DjiMotorGroup{
public:
    DjiMotorGroup();


private:
    DjiMotor motor[4];


};





#endif