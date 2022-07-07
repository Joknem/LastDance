#include "MotorPID.h"
#include "tim.h"


float MotorPID::CalcDceOutput(float _inputPos, float _inputVel)
{
    float errorPos = _inputPos - dce.setPointPos;
    float errorVel = _inputVel - dce.setPointVel;
    float deltaPos = errorPos - dce.lastError;
    dce.lastError = errorPos;
    dce.integralPos += errorPos;
    if (dce.integralPos > DCE_INTEGRAL_LIMIT) dce.integralPos = DCE_INTEGRAL_LIMIT;
    else if (dce.integralPos < -DCE_INTEGRAL_LIMIT) dce.integralPos = -DCE_INTEGRAL_LIMIT;
    dce.integralVel += errorVel;
    if (dce.integralVel > DCE_INTEGRAL_LIMIT) dce.integralVel = DCE_INTEGRAL_LIMIT;
    else if (dce.integralVel < -DCE_INTEGRAL_LIMIT) dce.integralVel = -DCE_INTEGRAL_LIMIT;

    dce.output = dce.kp * errorPos +
                 dce.ki * dce.integralPos + dce.kv * dce.integralVel +
                 dce.kd * deltaPos;

    if (dce.output > limitTorque) dce.output = limitTorque;
    else if (dce.output < -limitTorque) dce.output = -limitTorque;

    return dce.output;
}

void MotorPID::SetTorqueLimit(float _percent)
{
    if (_percent > 1)_percent = 1;
    else if (_percent < 0)_percent = 0;

    limitTorque = _percent * 13000;
}


void MotorPID::UpdateVelocity()
{
    velocity = angle - lastAngle;
    lastAngle = angle;
}


void MotorPID::SetEnable(bool _enable)
{
    isEnabled = _enable;
}