#ifndef MOTORPID_H
#define MOTORPID_H

#include <cstdint>

class MotorPID
{
public:
    MotorPID()
    {}

    const float DCE_INTEGRAL_LIMIT = 500;

    struct DCE_t
    {
        float kp;
        float kv;
        float ki;
        float kd;
        float setPointPos;
        float setPointVel;
        float integralPos;
        float integralVel;
        float lastError;
        float output;
    };
    DCE_t dce;

    float angle;
    float velocity;

    void UpdateVelocity();
    void SetEnable(bool _enable);
    void SetTorqueLimit(float _percent);
    float CalcDceOutput(float _inputPos, float _inputVel);

private:
    bool isEnabled;
    float lastAngle;
    float limitAngleMin;
    float limitAngleMax;
    float limitVelocity;
    float limitTorque; // 0~0.1
};


#endif
