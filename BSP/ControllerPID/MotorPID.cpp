#include "MotorPID.h"
#include "tim.h"

// float MotorPID::CalcDceOutput(float _inputPos, float _inputVel)
// {
//     float errorPos = _inputPos - *angle;
//     float errorVel = _inputVel - *velocity;
//     float deltaPos = errorPos - dce.lastError;
//     dce.lastError = errorPos;
//     dce.integralPos += errorPos; // + deltaPos * dce.kd
//     if (dce.integralPos > DCE_INTEGRAL_LIMIT) dce.integralPos = DCE_INTEGRAL_LIMIT;
//     else if (dce.integralPos < -DCE_INTEGRAL_LIMIT) dce.integralPos = -DCE_INTEGRAL_LIMIT;
//     dce.integralVel += errorVel;
//     if (dce.integralVel > DCE_INTEGRAL_LIMIT) dce.integralVel = DCE_INTEGRAL_LIMIT;
//     else if (dce.integralVel < -DCE_INTEGRAL_LIMIT) dce.integralVel = -DCE_INTEGRAL_LIMIT;

//     if (errorPos * dce.lastError < 0.f){
//         dce.integralPos *= 0.1;
//         dce.integralVel *= 0.1;
//     } else {
//         dce.integralPos *= 0.999;
//         dce.integralVel *= 0.999;
//     }

//     dce.output = dce.kp * errorPos +
//                  dce.ki * dce.integralPos +
//                  dce.kd * deltaPos +
//                  dce.kv * dce.integralVel;

//     if (dce.output > limitTorque) dce.output = limitTorque;
//     else if (dce.output < -limitTorque) dce.output = -limitTorque;

//     return dce.output;
// }

void MotorPID::SetTorqueLimit(float _percent)
{
    if (_percent > 1)
        _percent = 1;
    else if (_percent < 0)
        _percent = 0;

    limitTorque = _percent * 14000; // TODO : check 3508 & 2006 max value here
}

void MotorPID::SetSpeedLimit(float speed)
{
    if (speed > 10000)
        speed = 10000;
    else if (speed < 0)
        speed = 0;

    limitTorque = speed; // TODO : check 3508 & 2006 max value here
}

float MotorPID::CalPeng(float _input, peng_ctrl_type_t type)
{
    float posErr, pos_out, velErr, vel_out;
    if (last_ctrl_type != type) {
        pPos.last_err = 0.f;
        pPos.integral = 0.f;
        pVel.last_err = 0.f;
        pVel.integral = 0.f;
    }

    if (type == PENG_CTRL_TYPE_POSITION){
        posErr = _input - *angle;
        pPos.integral += posErr;
        pos_out =   pPos.kp * posErr +
                    pPos.integral * pPos.ki +
                    (posErr - pPos.last_err) * pPos.kd;
        pPos.last_err = posErr;

        pos_out =   pos_out > limitSpeed ? limitSpeed :
                    pos_out < -limitSpeed ? -limitSpeed : 
                    pos_out;
    } else {
        pos_out = _input;
    }

    velErr = pos_out - *velocity;
    pVel.integral += velErr;
    vel_out = pVel.kp * velErr +
                    pVel.integral * pVel.ki +
                    (velErr - pVel.last_err) * pVel.kd;

    if (vel_out > limitTorque)
        vel_out = limitTorque;
    else if (vel_out < -limitTorque)
        vel_out = -limitTorque;
        
    return vel_out;
}
