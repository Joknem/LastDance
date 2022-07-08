#ifndef MOTORPID_H
#define MOTORPID_H

#include <cstdint>

class MotorPID
{
public:
    MotorPID()
    {}

    const float DCE_INTEGRAL_LIMIT = 4000;

    // struct DCE_t
    // {
    //     float kp;
    //     float kv;
    //     float ki;
    //     float kd;
    //     // float setPointPos;
    //     // float setPointVel;
    //     float integralPos = 0.f;
    //     float integralVel = 0.f;
    //     float lastError;
    //     float output;
    // };
    // DCE_t dce;

    struct pid_t
    {
        float kp;
        float ki;
        float kd;
        float last_err;
        float integral;
    };

    pid_t pVel, pPos;

    enum peng_ctrl_type_t {
        PENG_CTRL_TYPE_SPEED,
        PENG_CTRL_TYPE_POSITION
    };
    

    float * angle;
    float * velocity;

    void SetTorqueLimit(float _percent);
    void SetSpeedLimit(float speed);
    // float CalcDceOutput(float _inputPos, float _inputVel);
    float CalPeng(float _inputPos, peng_ctrl_type_t type);

private:
    float limitTorque;
    float limitSpeed; 
    peng_ctrl_type_t last_ctrl_type;
};


#endif
