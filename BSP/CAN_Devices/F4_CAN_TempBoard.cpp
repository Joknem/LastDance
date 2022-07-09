#include "F4_CAN_TempBoard.h"

#define COMMAND_PACK_ID 0x494
#define RETURNS_PACK_ID 0x496

#define H1_LIMIT 320.0
#define H2_LIMIT 320.0




bsp_can_rx_cb_ret_e TemperBoard::rx_cb(FDCAN_RxHeaderTypeDef *pRxHeader, uint8_t *pRxData) {
    if (pRxHeader->Identifier != RETURNS_PACK_ID){
        return BSP_CAN_RX_CB_VALUE_INVALID;
    }

    // TODO : temperboard rx process here

    return BSP_CAN_RX_CB_VALUE_VALID;
}


TemperBoard::TemperBoard(FDCAN_HandleTypeDef *_hfdcan):CanDevice(_hfdcan){
    for (int i = 0; i <  8; i++){
        info.raw[i] = 0;
    }
}

bool TemperBoard::set_angle_routate(float angle){
    float m = (angle / 2);
    if (m > 180.f || m < -180.f){
        ST_LOGE("Not valid input");
        return false;
    }

    info.val.a_r = (int16_t)(m);
    return true;
}

bool TemperBoard::set_angle_expand(float angle){
    float m = (angle / 2);
    if (m > 180.f || m < 0.f){
        ST_LOGE("Not valid input");
        return false;
    }

    info.val.a_e = (int16_t)(m);
    return true;
}

bool TemperBoard::set_height_lower(float height){
    float m = height;
    if (m > H1_LIMIT || m < 0.f){
        ST_LOGE("Not valid input");
        return false;
    }

    info.val.h1 = (int16_t)(m);
    return true;
}

bool TemperBoard::set_height_higher(float height){
    float m = height;
    if (m > H2_LIMIT || m < 0.f){
        ST_LOGE("Not valid input");
        return false;
    }

    info.val.h2 = (int16_t)(m);
    return true;
}

bool TemperBoard::set_pull(bool p){

    info.val.pull = (p == true) ? 1 : 0;
    return true;

}

void TemperBoard::output(void){
    can_send_message8(COMMAND_PACK_ID, info.raw);
}