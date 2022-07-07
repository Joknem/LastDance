#ifndef BSP_CAN_H
#define BSP_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fdcan.h"


typedef struct _bsp_can_device_t
{
    struct _bsp_can_device_t * next;
    FDCAN_HandleTypeDef * hfdcan;
    void (*rx_cb)(FDCAN_RxHeaderTypeDef *pRxHeader, uint8_t *pRxData);
} bsp_can_device_t;


void __bsp_fdcan_send8(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *pTxData);
#define BSP_FDCAN_SendMsg_8(bus, id, msg) __bsp_fdcan_send8(&hfdcan ## bus, id, msg)
#define BSP_FDCAN1_SendMsg_8(id, msg) FDCAN_SendMsg_8(1, id, msg)
#define BSP_FDCAN2_SendMsg_8(id, msg) FDCAN_SendMsg_8(2, id, msg)

void bsp_can_add_device(bsp_can_device_t * device);
void bsp_can_delete_device(const bsp_can_device_t * device);

void bsp_can_send_message(bsp_can_device_t * dev, uint32_t id, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif