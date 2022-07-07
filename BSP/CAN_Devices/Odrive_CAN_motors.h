#ifndef ODRIVE_CAN_MOTORS_H
#define ODRIVE_CAN_MOTORS_H

#include <cstdint>

#define AXIS0_NODE_ID  (1<<5) // 逆时针为正，CAN_ID分别为123
#define AXIS1_NODE_ID  (2<<5)
#define AXIS2_NODE_ID  (3<<5)
#define F405 0x106
#define H750 0x107

typedef enum
{
    AXIS_0 = 0,
    AXIS_1 = 1,
	AXIS_2 = 2
} Axis_t;

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


typedef struct OdriveAxisGetState_t{
    uint32_t axis_error;
    uint32_t axis_current_state;
    uint32_t motor_error;
    uint32_t encoder_error;
    uint32_t sensorless_error;
    float encoder_pos_estimate;
    float encoder_vel_estimate;
    int32_t encoder_shadow_count;
    int32_t encoder_cpr_count;
    float iq_setpoint;
    float iq_measured;
    float sensorless_pos_estimate;
    float sensorless_vel_estimate;
    float vbus_voltage;
} OdriveAxisGetState_t;

typedef struct OdriveAxisSetState_t
{
    uint16_t axis_node_id;
    uint32_t requested_state;
    int32_t control_mode;
    int32_t input_mode;
    int16_t vel_ff;
    int16_t current_ff;
    int32_t input_pos;
    float input_vel;
    float torque_vel;
    int32_t input_current;
    float vel_limit;
    float traj_vel_limit;
    float traj_accel_limit;
    float traj_decel_limit;
    float traj_a_per_css;
} OdriveAxisSetState_t;

typedef enum
{
    MSG_CO_NMT_CTRL = 0x000, // CANOpen NMT Message REC
    MSG_ODRIVE_HEARTBEAT,
    MSG_ODRIVE_ESTOP,
    MSG_GET_MOTOR_ERROR, // Errors
    MSG_GET_ENCODER_ERROR,
    MSG_GET_SENSORLESS_ERROR,
    MSG_SET_AXIS_NODE_ID,
    MSG_SET_AXIS_REQUESTED_STATE,
    MSG_SET_AXIS_STARTUP_CONFIG,
    MSG_GET_ENCODER_ESTIMATES,
    MSG_GET_ENCODER_COUNT,
    MSG_SET_CONTROLLER_MODES,
    MSG_SET_INPUT_POS,
    MSG_SET_INPUT_VEL = 0x00D,
    MSG_SET_INPUT_CURRENT,
    MSG_SET_VEL_LIMIT,
    MSG_START_ANTICOGGING,
    MSG_SET_TRAJ_VEL_LIMIT,
    MSG_SET_TRAJ_ACCEL_LIMITS,
    MSG_SET_TRAJ_A_PER_CSS,
    MSG_GET_IQ,
    MSG_GET_SENSORLESS_ESTIMATES,
    MSG_RESET_ODRIVE,
    MSG_GET_VBUS_VOLTAGE,
    MSG_CLEAR_ERRORS,
    MSG_CO_HEARTBEAT_CMD = 0x700, // CANOpen NMT Heartbeat  SEND
} Odrive_Command;

typedef enum {
    AXIS_STATE_UNDEFINED = 0,           //<! will fall through to idle
    AXIS_STATE_IDLE = 1,                //<! disable PWM and do nothing
    AXIS_STATE_STARTUP_SEQUENCE = 2, //<! the actual sequence is defined by the config.startup_... flags
    AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3,   //<! run all calibration procedures, then idle
    AXIS_STATE_MOTOR_CALIBRATION = 4,   //<! run motor calibration
    AXIS_STATE_SENSORLESS_CONTROL = 5,  //<! run sensorless control
    AXIS_STATE_ENCODER_INDEX_SEARCH = 6, //<! run encoder index search
    AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 7, //<! run encoder offset calibration
    AXIS_STATE_CLOSED_LOOP_CONTROL = 8  //<! run closed loop control
} Odrive_AxisState_t;

typedef enum {
    ODRV_CONTROL_INPUTMODE_IDSABLE = 0,
    ODRV_CONTROL_INPUTMODE_PASSTHROUGH = 1,
    ODRV_CONTROL_INPUTMODE_VEL_RAMP = 2,
    ODRV_CONTROL_INPUTMODE_POS_FILTER = 3,
    ODRV_CONTROL_INPUTMODE_TRAP_TRAJ = 5,
    ODRV_CONTROL_INPUTMODE_TORQUE_RAMP = 6,
} Odrive_Controller_inputMode;

typedef enum {
    ODRV_CONTROL_CTRLMODE_VOLTAGE = 0,
    ODRV_CONTROL_CTRLMODE_TORQUE = 1,
    ODRV_CONTROL_CTRLMODE_VELOCITY = 2,
    ODRV_CONTROL_CTRLMODE_POSITION = 3,
} Odrive_Controller_ctrlMode;

typedef struct _Pos{
    float Pos_x;
    float Pos_y;
    float Pos_angle;
}Pos_t;
typedef struct _Chassis_Info {
	float Pan_Speed;	// *平移速度 
	float Spinning_speed;	// *旋转速度
	Pos_t position;	// *车身现在的位置
}Chassis_Info_t;

#endif