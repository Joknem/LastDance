#include "bsp_can.h"

#define BSP_CAN_LOG_Error(format)  LOG_COLOR_E "[bsp_can.c]" LOG_RESET_COLOR ":\t" format "\n"
#define BSP_CAN_LOGE(format, ...) uart_printf(BSP_CAN_LOG_Error(format), ##__VA_ARGS__)


static bsp_can_device_t * root_dev = NULL;


inline void __bsp_fdcan_send8(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *pTxData){
	static FDCAN_TxHeaderTypeDef txHeader;
	uint32_t freeLevel;
	txHeader.Identifier = id;
	txHeader.IdType = FDCAN_STANDARD_ID;
	txHeader.TxFrameType = FDCAN_DATA_FRAME;
	txHeader.DataLength = FDCAN_DLC_BYTES_8;
	txHeader.BitRateSwitch = FDCAN_BRS_OFF;
	txHeader.FDFormat = FDCAN_CLASSIC_CAN;
	txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	txHeader.ErrorStateIndicator = FDCAN_ESI_PASSIVE;
	HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txHeader, pTxData);
	freeLevel = HAL_FDCAN_GetTxFifoFreeLevel(hfdcan);
	if (freeLevel == 0){
		BSP_CAN_LOGE("no free fifo");
	}
}


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	FDCAN_RxHeaderTypeDef rx_header;
	bsp_can_device_t const * dev = root_dev;
	uint8_t __aligned(4) rx_data[8];

	HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, rx_data);
	while (dev != NULL){
		dev->rx_cb(&rx_header, rx_data);
		dev = dev->next;
	}
}


void bsp_can_add_device(bsp_can_device_t * device){
	bsp_can_device_t * p = root_dev;
	if (device == NULL){
		return;
	}
	device->next = NULL;

	if (root_dev == NULL){
		root_dev = device;
	} else {
		while (p->next != NULL) {
			p = root_dev->next;
		}

		p->next = device;

	}
}

void bsp_can_delete_device(const bsp_can_device_t * device){
	bsp_can_device_t * p = root_dev;
	if (device == NULL){
		return;
	}

	if (root_dev == device){
		root_dev = root_dev->next;
	} else {
		while (p != NULL){
			if (p->next == device) {
				p = p->next->next;
			}
			p = p->next;
		}
	}
}

void bsp_can_send_message(bsp_can_device_t * dev, uint32_t id, uint8_t *data){
	__bsp_fdcan_send8(dev->hfdcan, id, data);
}