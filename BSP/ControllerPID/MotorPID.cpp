#include "MotorPID.h"
#include "tim.h"


float MotorPID::CalcDceOutput(float _inputPos, float _inputVel)
{
    float errorPos = _inputPos - *angle;
    float errorVel = _inputVel - *velocity;
    float deltaPos = errorPos - dce.lastError;
    dce.lastError = errorPos;
    dce.integralPos += errorPos; // + deltaPos * dce.kd
    if (dce.integralPos > DCE_INTEGRAL_LIMIT) dce.integralPos = DCE_INTEGRAL_LIMIT;
    else if (dce.integralPos < -DCE_INTEGRAL_LIMIT) dce.integralPos = -DCE_INTEGRAL_LIMIT;
    dce.integralVel += errorVel;
    if (dce.integralVel > DCE_INTEGRAL_LIMIT) dce.integralVel = DCE_INTEGRAL_LIMIT;
    else if (dce.integralVel < -DCE_INTEGRAL_LIMIT) dce.integralVel = -DCE_INTEGRAL_LIMIT;

    if (errorPos * dce.lastError < 0.f){
        dce.integralPos *= 0.1;
        dce.integralVel *= 0.1;
    } else {
        dce.integralPos *= 0.999;
        dce.integralVel *= 0.999;
    }

    dce.output = dce.kp * errorPos +
                 dce.ki * dce.integralPos +
                 dce.kd * deltaPos +
                 dce.kv * dce.integralVel;

    if (dce.output > limitTorque) dce.output = limitTorque;
    else if (dce.output < -limitTorque) dce.output = -limitTorque;

    return dce.output;
}

void MotorPID::SetTorqueLimit(float _percent)
{
    if (_percent > 1)_percent = 1;
    else if (_percent < 0)_percent = 0;

    limitTorque = _percent * 13000; // TODO : check 3508 & 2006 max value here
}
