#ifndef DJI_CAN_MOTORS_H
#define DJI_CAN_MOTORS_H

#include "ControllerPID/MotorPID.h"
#include "bsp_can.h"

class DjiMotor {
public:
    DjiMotor(){
        pid.SetTorqueLimit(0.80);
        pid.dce.kp = 12.0;
        pid.dce.ki = 1.223;
        pid.dce.kv = 3.0;
        pid.dce.kd = 1.23;

        pid.angle = &angle;
        pid.velocity = &speed;

        ratio = (36 * 90 / 17);
        base_angle = 0.f;
    }

    struct motor_measure_t{
        uint16_t ecd;
        int16_t speed_rpm;
        int16_t given_current;
        uint8_t temperate;
        uint16_t last_ecd;
        int16_t circle;
    };

    motor_measure_t currentState = {
        .circle = 0,
    };

    float get_angle(bool if_cal_circle);

    float update(float _inputPos, float _inputVel);

    float angle;
    float speed;

    float output;

    float ratio;
    float base_angle;

    bool is_update = false;

private:
    MotorPID pid;
    


};



class DjiMotorGroup{
public:
    DjiMotorGroup(FDCAN_HandleTypeDef * _hfdcan, bool _isLowerIdentityGroup);
    

    DjiMotor motor[4];
    void SetInput(uint8_t id, float p, float v);
    void output(void);
private:

    void setCurrent(int16_t val[4]); 
    bsp_can_device_t can_devices;
    uint32_t ID_tx;


};


void DjiCanMotorsInit(void);



#endif