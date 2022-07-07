#include "bsp_can.h"

#define get_motor_measure(ptr, data)                                   \
	{                                                                  \
		(ptr)->last_ecd = (ptr)->ecd;                                  \
		(ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);           \
		(ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);     \
		(ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]); \
		(ptr)->temperate = (data)[6];}

typedef union
{
	uint8_t raw[8];

	struct __packed
	{
		uint32_t axis_error;
		uint8_t axis_current_state;
		uint8_t __reserved_1;
		uint8_t __reserved_2;
		uint8_t ctrl_status;
	} hb_info;

	struct __packed
	{
		float pos;
		float vel;
	} enc_info;

}odrv_heartbeat_data_t;


typedef enum{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,

    CAN_YAW_MOTOR_ID = 0x205,
    CAN_PIT_MOTOR_ID = 0x206,
    CAN_TRIGGER_MOTOR_ID = 0x207,
    CAN_GIMBAL_ALL_ID = 0x1FF,
    CAN_ODRV1_AXIS1_ID = 0x010
} can_msg_id_e;

extern Vel_Motor_t *vel_motors[3];
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	FDCAN_RxHeaderTypeDef rx_header;
	uint8_t odrv_id;
	uint8_t odrv_type;
	uint8_t __aligned(4) rx_data[8];
	odrv_heartbeat_data_t * odrv_data = (odrv_heartbeat_data_t *)rx_data;
	Vel_Motor_t * motor;
	//TODO:control 3508
	HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, rx_data);
	switch(rx_header.Identifier) {
		case H750:
		{

		}break;
		case CAN_3508_M1_ID:
		case CAN_3508_M2_ID:
		case CAN_3508_M3_ID:
		case CAN_3508_M4_ID:
		case CAN_YAW_MOTOR_ID:
		case CAN_PIT_MOTOR_ID:
		case CAN_TRIGGER_MOTOR_ID:
		{
			static uint8_t i = 0;
			i = rx_header.Identifier - CAN_3508_M1_ID;
			get_motor_measure(&motor_chassis[i], rx_data);
			field[i].rudder_motor.speed = motor_chassis[i].speed_rpm;

			if(motor_chassis[i].ecd - motor_chassis[i].last_ecd > 4096)
				motor_chassis[i].circle--;
			else if(motor_chassis[i].ecd - motor_chassis[i].last_ecd < -4096)
				motor_chassis[i].circle++;

			field[i].rudder_motor.angle = motor_chassis[i].ecd * 360 / 8192.0;
			field[i].rudder_motor.circle = motor_chassis[i].circle;
			field[i].rudder_motor.absolute_angle =
				(float32_t)((field[i].rudder_motor.angle +
							 field[i].rudder_motor.circle * 360) /
							ratio) -
				field[i].rudder_motor.base_angle;
		} break;
		
		case AXIS0_NODE_ID | 0x01:
		case AXIS1_NODE_ID | 0x01:
		case AXIS2_NODE_ID | 0x01:
		case AXIS0_NODE_ID | 0x09:
		case AXIS1_NODE_ID | 0x09:
		case AXIS2_NODE_ID | 0x09:
		{
			// odrv encoder estimate
			odrv_id   = rx_header.Identifier & 0b11111100000;
			odrv_type = rx_header.Identifier & 0b00000011111;
			switch (odrv_id)
			{
			case AXIS0_NODE_ID:
				motor = vel_motors[0];
				break;
			case AXIS1_NODE_ID:
				motor = vel_motors[1];
				break;
			case AXIS2_NODE_ID:
				motor = vel_motors[2];
				break;
			
			default:
				return;
				break;
			}

			switch (odrv_type)
			{
			case 0x001:{
				motor->odrive_get_axis.axis_error = odrv_data->hb_info.axis_error;
				motor->odrive_get_axis.axis_current_state = odrv_data->hb_info.axis_current_state;
			} break;

			case 0x009:{
				motor->odrive_get_axis.encoder_vel_estimate = odrv_data->enc_info.vel;
				motor->odrive_get_axis.encoder_pos_estimate = odrv_data->enc_info.pos;
			} break;
			
			default:
				break;
			}
		}break;

		default: {
			break;
		}
	}
}