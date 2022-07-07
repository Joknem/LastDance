#ifndef MOTORPID_H
#define MOTORPID_H

#include <cstdint>

class MotorPID
{
public:
    MotorPID()
    {}

    const float DCE_INTEGRAL_LIMIT = 4000;

    struct DCE_t
    {
        float kp;
        float kv;
        float ki;
        float kd;
        // float setPointPos;
        // float setPointVel;
        float integralPos = 0.f;
        float integralVel = 0.f;
        float lastError;
        float output;
    };
    DCE_t dce;

    float * angle;
    float * velocity;

    void SetTorqueLimit(float _percent);
    float CalcDceOutput(float _inputPos, float _inputVel);

private:
    float limitTorque; // 0~0.1
};


#endif
