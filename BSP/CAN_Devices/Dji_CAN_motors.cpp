#include "Dji_CAN_motors.h"

/* 
********************************
defines
********************************
*/
#define get_motor_measure(ptr, data)                                   \
	{                                                                  \
		(ptr)->last_ecd = (ptr)->ecd;                                  \
		(ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);           \
		(ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);     \
		(ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]); \
		(ptr)->temperate = (data)[6];                                  \
	}

/* 
********************************
enum & struct
********************************
*/
typedef enum
{
	CAN_DJI_L4ALL_ID = 0x200,
	CAN_DJI_H4ALL_ID = 0x1FF,
	CAN_DJI_M1_ID = 0x201,
	CAN_DJI_M2_ID = 0x202,
	CAN_DJI_M3_ID = 0x203,
	CAN_DJI_M4_ID = 0x204,
	CAN_DJI_M5_ID = 0x205,
	CAN_DJI_M6_ID = 0x206,
	CAN_DJI_M7_ID = 0x207,
	CAN_DJI_M8_ID = 0x208,
} dji_motors_can_id_e;


/* 
********************************
global variabal
********************************
*/
DjiMotorGroup * djiMotorGroupLowerId = NULL;
DjiMotorGroup * djiMotorGroupHigherId = NULL;


/* 
********************************
c function part
********************************
*/

void DjiCanMotorsInit(void){
	djiMotorGroupLowerId = new DjiMotorGroup(&hfdcan2, false);
	djiMotorGroupLowerId = new DjiMotorGroup(&hfdcan2, false);
}

// rx process
static bsp_can_rx_cb_ret_e __dii_motors_rx_process(FDCAN_RxHeaderTypeDef *pRxHeader, uint8_t *pRxData)
{
	int32_t id = pRxHeader->Identifier - CAN_DJI_M1_ID;
	DjiMotorGroup * gp;
	if (id > 7 || id < 0)
	{
		return BSP_CAN_RX_CB_VALUE_INVALID;
	}

	// TODO add data to adsobhufwqaehniko;
	gp = id > 3 ? djiMotorGroupHigherId : djiMotorGroupLowerId;
	if (djiMotorGroupHigherId == NULL){
		return BSP_CAN_RX_CB_VALUE_INVALID;
	}

	id = id > 3 ? id - 4 : id;

	get_motor_measure(&(gp->motor[id].currentState), pRxData);

	if(gp->motor[id].currentState.ecd - gp->motor[id].currentState.last_ecd > 4096)
		gp->motor[id].currentState.circle--;
	else if(gp->motor[id].currentState.ecd - gp->motor[id].currentState.last_ecd < -4096)
		gp->motor[id].currentState.circle++;
	
	gp->motor->angle = gp->motor->get_angle(true);
	gp->motor->speed = gp->motor->currentState.speed_rpm * 1.f;
	

	return BSP_CAN_RX_CB_VALUE_VALID;
}

/* 
********************************
c++ class: motor group part
********************************
*/

DjiMotorGroup::DjiMotorGroup(FDCAN_HandleTypeDef *_hfdcan, bool _isLowerIdentityGroup)
{
	can_devices.hfdcan = _hfdcan;
	can_devices.rx_cb = __dii_motors_rx_process;
	ID_tx = _isLowerIdentityGroup == true ? CAN_DJI_L4ALL_ID : CAN_DJI_H4ALL_ID;
	bsp_can_add_device(&can_devices);
}

void DjiMotorGroup::SetInput(uint8_t id, float p, float v){
	if (id > 4){
		return;
	}
	motor[id].update(p, v);
}

void DjiMotorGroup::output(void){
	int16_t val[4];
	for (int i = 0; i < 4; i++){
		val[i] = motor[i].is_update ? motor[i].output : 0;
		motor[i].is_update = false;
	}
	setCurrent(val);
}

void DjiMotorGroup::setCurrent(
	int16_t val[4]
){

	static uint8_t chassis_can_send_data[8];

	chassis_can_send_data[0] = val[0] >> 8;
	chassis_can_send_data[1] = val[0];
	chassis_can_send_data[2] = val[1] >> 8;
	chassis_can_send_data[3] = val[1];
	chassis_can_send_data[4] = val[2] >> 8;
	chassis_can_send_data[5] = val[2];
	chassis_can_send_data[6] = val[3] >> 8;
	chassis_can_send_data[7] = val[3];

	bsp_can_send_message(&can_devices, ID_tx, chassis_can_send_data);
}


/* 
********************************
c++ class: motor single part
********************************
*/

float DjiMotor::get_angle(bool if_cal_circle){
	float phrase = currentState.ecd * 360 / 8192.0;
	if (if_cal_circle == true){
		return (currentState.circle * 360.f + phrase) / ratio - base_angle;
	} else {
		return phrase;
	}
}

float DjiMotor::update(float _inputPos, float _inputVel){
	is_update = true;
	output = pid.CalcDceOutput(_inputPos, _inputVel);
	return output;
}
