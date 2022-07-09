#ifndef F4_CAN_TEMPBOARD_H
#define F4_CAN_TEMPBOARD_H


#include "bsp_can.h"

class TemperBoard : public CanDevice{
public:
    TemperBoard(FDCAN_HandleTypeDef *_hfdcan);
    
    bool set_angle_routate(float angle);
    bool set_angle_expand(float angle);
    bool set_height_lower(float height);
    bool set_height_higher(float height);
    bool set_pull(bool p);
    void output(void);
    
    bsp_can_rx_cb_ret_e rx_cb(FDCAN_RxHeaderTypeDef *pRxHeader, uint8_t *pRxData) override;
private:
    typedef union _temper_board_tx_msg_t{
        struct __packed{
            int8_t a_e;
            int8_t a_r;
            uint16_t pull;
            uint16_t h1;
            uint16_t h2;
        }val;
        uint8_t raw[8];
    } temper_board_tx_msg_t;

    temper_board_tx_msg_t info;
};




#endif