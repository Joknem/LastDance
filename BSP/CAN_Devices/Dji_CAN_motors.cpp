#include "ControllerPID/MotorPID.h"
#include "bsp_can.h"
#include "Dji_CAN_motors.h"


	

	// uint8_t odrv_id;
	// uint8_t odrv_type;
	// odrv_heartbeat_data_t * odrv_data = (odrv_heartbeat_data_t *)rx_data;
	// Vel_Motor_t * motor;
	//TODO:control 3508
	// switch(rx_header.Identifier) {
	// 	case H750:
	// 	{

	// 	}break;
	// 	case CAN_3508_M1_ID:
	// 	case CAN_3508_M2_ID:
	// 	case CAN_3508_M3_ID:
	// 	case CAN_3508_M4_ID:
	// 	case CAN_YAW_MOTOR_ID:
	// 	case CAN_PIT_MOTOR_ID:
	// 	case CAN_TRIGGER_MOTOR_ID:
	// 	{
	// 		static uint8_t i = 0;
	// 		i = rx_header.Identifier - CAN_3508_M1_ID;
	// 		get_motor_measure(&motor_chassis[i], rx_data);
	// 		field[i].rudder_motor.speed = motor_chassis[i].speed_rpm;

	// 		if(motor_chassis[i].ecd - motor_chassis[i].last_ecd > 4096)
	// 			motor_chassis[i].circle--;
	// 		else if(motor_chassis[i].ecd - motor_chassis[i].last_ecd < -4096)
	// 			motor_chassis[i].circle++;

	// 		field[i].rudder_motor.angle = motor_chassis[i].ecd * 360 / 8192.0;
	// 		field[i].rudder_motor.circle = motor_chassis[i].circle;
	// 		field[i].rudder_motor.absolute_angle =
	// 			(float32_t)((field[i].rudder_motor.angle +
	// 						 field[i].rudder_motor.circle * 360) /
	// 						ratio) -
	// 			field[i].rudder_motor.base_angle;
	// 	} break;
		
	// 	case AXIS0_NODE_ID | 0x01:
	// 	case AXIS1_NODE_ID | 0x01:
	// 	case AXIS2_NODE_ID | 0x01:
	// 	case AXIS0_NODE_ID | 0x09:
	// 	case AXIS1_NODE_ID | 0x09:
	// 	case AXIS2_NODE_ID | 0x09:
	// 	{
	// 		// odrv encoder estimate
	// 		odrv_id   = rx_header.Identifier & 0b11111100000;
	// 		odrv_type = rx_header.Identifier & 0b00000011111;
	// 		switch (odrv_id)
	// 		{
	// 		case AXIS0_NODE_ID:
	// 			motor = vel_motors[0];
	// 			break;
	// 		case AXIS1_NODE_ID:
	// 			motor = vel_motors[1];
	// 			break;
	// 		case AXIS2_NODE_ID:
	// 			motor = vel_motors[2];
	// 			break;
			
	// 		default:
	// 			return;
	// 			break;
	// 		}

	// 		switch (odrv_type)
	// 		{
	// 		case 0x001:{
	// 			motor->odrive_get_axis.axis_error = odrv_data->hb_info.axis_error;
	// 			motor->odrive_get_axis.axis_current_state = odrv_data->hb_info.axis_current_state;
	// 		} break;

	// 		case 0x009:{
	// 			motor->odrive_get_axis.encoder_vel_estimate = odrv_data->enc_info.vel;
	// 			motor->odrive_get_axis.encoder_pos_estimate = odrv_data->enc_info.pos;
	// 		} break;
			
	// 		default:
	// 			break;
	// 		}
	// 	}break;

	// 	default: {
	// 		break;
	// 	}
	// }

static void __dii_motors_rx_process(FDCAN_RxHeaderTypeDef *pRxHeader, uint8_t *pRxData);

static bsp_can_device_t dji_motors = {
	.hfdcan = &hfdcan2,
	.rx_cb = __dii_motors_rx_process
};

DjiMotorGroup::DjiMotorGroup(){
	bsp_can_add_device(&dji_motors);
}


typedef enum{
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

void CAN_cmd_chassis(int16_t motor1,
					 int16_t motor2,
					 int16_t motor3,
					 int16_t motor4)
{
	static uint8_t chassis_can_send_data[8];

	chassis_can_send_data[0] = motor1 >> 8;
	chassis_can_send_data[1] = motor1;
	chassis_can_send_data[2] = motor2 >> 8;
	chassis_can_send_data[3] = motor2;
	chassis_can_send_data[4] = motor3 >> 8;
	chassis_can_send_data[5] = motor3;
	chassis_can_send_data[6] = motor4 >> 8;
	chassis_can_send_data[7] = motor4;

	bsp_can_send_message(&dji_motors, CAN_CHASSIS_ALL_ID, chassis_can_send_data);
}




#define get_motor_measure(ptr, data)                                   \
	{                                                                  \
		(ptr)->last_ecd = (ptr)->ecd;                                  \
		(ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);           \
		(ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);     \
		(ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]); \
		(ptr)->temperate = (data)[6];}

static void __dii_motors_rx_process(FDCAN_RxHeaderTypeDef *pRxHeader, uint8_t *pRxData){
	int32_t id =  pRxHeader->Identifier - CAN_DJI_M1_ID;
	if (id > 7 || id < 0){
		return;
	}

	// TODO add data to adsobhufwqaehniko;


}
