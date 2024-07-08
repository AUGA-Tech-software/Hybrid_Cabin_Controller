#ifndef _GRAPH_DEFINES_H_
#define _GRAPH_DEFINES_H_

/*--------------------------------------------------------------------------*/
/** \file     graph_defines.h
*   \brief    Generated code from FUP
*             Project name: M3600_CAN_SPS_D_CabinControllerM1.sch
*
*   \date     2/7/2023 11:50 AM    \author   dr/
*
*   \platform HCS08DZ / HCS12XE / HCS12XD
* --------------------------------------------------------------------------
*   \defgroup Name      graph_func
* --------------------------------------------------------------------------*/

#define MEMBITARRAY_INDICES  4
#define MEM16ARRAY_INDICES   0
#define MEM32ARRAY_INDICES   2
#define CONST32ARRAY_INDICES 2

#define MEMBITARRAY_SIZE     1
#define MEM16ARRAY_SIZE      1
#define MEM32ARRAY_SIZE      2
#define CONST32ARRAY_SIZE    2

#define I2C_ARRAY_SIZE       1
#define I2C_BYTE_SIZE        1
//#define I2C_AKTIV

//#define MODBUS_AKTIV
//#define MODBUS_MODE 0                 // 0 = CAN I/O 1 = SHOWCASE HW

//#define PWM_ANALOG_IO_TICK            // If PWM_CONTROL, PROP_CAN_60A or CAN_FULLBRIDGE is used

//#define PORT_INTERRUPT_ACTIVE         // Activate the calculation on port interrupt (FREQUENCY_CNT or INCREMENTAL ENCODER)

//#define MOT_CTRL_1X_FULLBRIDGE_AKTIV     // Activate block mot_ctrl_1x_fullbridge for modules MRS_30A_MOTORCONTROLLER_WP and MRS_60A_MOTORCONTROLLER
//#define PWM_OUTPUT_FULLBRIDGE_10A_AKTIV     // Activate pwm_output_fullbridge_10A device only possible for pwm_output_fullbridge_10A 
//#define MOT_CTRL_4X_FULLBRIDGE_AKTIV     // Activate mot ctrl 4x fullbridge device only possible for mot ctrl 4x fullbridge 
//#define MIRROR_CTRL_AKTIV     // Activate mirror ctrl device 
//#define PWM_DUTYCYCLE_READ_ACTIVE       // Activate PWM_DUTYCYCLE_READ_ACTIVE here for frequency measurement because define is in usercode disabled by default  

#endif