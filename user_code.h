#ifndef _USER_CODE_H_
#define _USER_CODE_H_
   
/*----------------------------------------------------------------------------*/
/**
*   \file         user_code.h
*   \brief        Usercode function header. 
*/
/*----------------------------------------------------------------------------*/
/** 
*   \addtogroup   doc_user_code User Code 
*   \{
*   \brief        This group contains the main function for mrs lib users. See usercode() function.
*   \details      Here the main setup is done and library is prepared for usage.
*               This document offers a detailed explanation of the the MRS Developers Studio user functions.
*               The user_code.c file contains two different kinds of functions. The first category contains the 
*               usercode() function which is called each loop in the main-loop function. Put your main application 
*				code into this function. If you need to do time critical operations, use the functions
*               of the second category.  
*               The second category are callback-functions which are called by events / interrupt service routines. If you
*               have to react on time critical events like sending CAN messages you have to use the corresponding functions
*               in the "Events" section in the user_code.c file. Take care not to put too much code into the 
*               callback-functions.
*               
*               This document should offer an overview of the functions which can be used by the user. Each function
*               has a documentation which offers the necessary knowledge to use it. You can find the documented 
*               functions under the "Modules" or "Files" entry in the left or top menu. They are organized on a file basis in the 
*               "File List" entry or alphabetically sorted in the "Globals" entry. In the "Modules" category they are organized in
*               logical groups, like CAN, LIN or IO functions.
*
*               This documentation does not offer general information about general MRS Developers Studio usage, like how 
*               to define data-points or how to use the graphical programming. You can find this information in the manual
*               which is located in the MRS Developers Studio GUI under "Help" then "Manual".
*
*/
/*----------------------------------------------------------------------------*/

#include "bios_can.h"
#include "os_can.h"



// --------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------

#define ECU_CABIN 0

// enum AUXVALVESTATECMD {
// 	BLOCK = 0,
// 	EXTEND,
// 	RETRACT,
// 	FLOAT,
// 	DONTCARE = 0xF
// };

typedef struct {
	const uint8_t pos_bit;
	const uint8_t length_bit;
	void *variable;
} J1939_SPN;

typedef struct {
	const uint32_t pgn;
	const uint16_t repetition_rate_ms;
	const uint16_t data_length_bytes;
	const uint16_t spn_list_start;
	const uint8_t spn_list_len;
	const uint8_t ecu_id;
	const uint8_t tx_enable;
	uint32_t next_tx_timestamp;
} J1939_PGN;

typedef struct {
	uint8_t		spn_2676_y_axis_neutral_position_status;
	uint8_t		spn_2671_y_axis_negative_position_status;
	uint8_t		spn_2666_y_axis_positive_position_status;
	uint16_t	spn_2661_y_axis_position;
	uint8_t		spn_2688_button_4_pressed;
	uint8_t		spn_2687_button_3_pressed;
	uint8_t		spn_2686_button_2_pressed;
	uint8_t		spn_2685_button_1_pressed;
	uint8_t		spn_2692_button_8_pressed;
	uint8_t		spn_2691_button_7_pressed;
	uint8_t		spn_2690_button_6_pressed;
	uint8_t		spn_2689_button_5_pressed;
	uint8_t		spn_2696_button_12_pressed;
	uint8_t		spn_2695_button_11_pressed;
	uint8_t		spn_2694_button_10_pressed;
	uint8_t		spn_2693_button_9_pressed;
} PGN_64982_BJM1;

typedef struct {
	uint8_t		spn_2725_button_4_pressed;
	uint8_t		spn_2724_button_3_pressed;
	uint8_t		spn_2723_button_2_pressed;
	uint8_t		spn_2722_button_1_pressed;
	uint8_t		spn_2729_button_8_pressed;
	uint8_t		spn_2728_button_7_pressed;
	uint8_t		spn_2727_button_6_pressed;
	uint8_t		spn_2726_button_5_pressed;
	uint8_t		spn_2730_button_9_pressed;
} PGN_64984_BJM2;

typedef struct {
	uint8_t		spn_1856_seat_belt_sw;
	uint8_t		spn_5630_park_brake_cmd;
} PGN_57434_CM1;

typedef struct {
	uint8_t		spn_1504_operator_seat_sw;	// SEAT SWITCH
	uint16_t		joystick_Y_possition_2;
} PGN_65280_PROPB_00;

typedef struct {
	uint8_t		spn_69_two_speed_axle_sw;
	uint8_t		spn_70_parking_brake_sw;
	uint8_t		spn_597_brake_sw;
} PGN_65265_CCVS1;

typedef struct {
	uint16_t	spn_159_engine_gaseous_fuel_supply_presure_1;
} PGN_65277_AF1;

typedef struct {
	uint8_t		spn_525_transmission_requested_gear;
} PGN_256_TC1;

typedef struct {
	uint8_t		spn_9842_momentary_operator_key_sw;
	uint8_t		spn_10145_operator_key_sw_ignition_power;
	uint8_t		spn_10146_operator_key_sw_start;
	uint8_t		spn_10147_operator_key_sw_delayed_battery_off;
} PGN_64980_CM3;

typedef struct {
	uint8_t		isobus_a1904_rear_hitch_pos_cmd;
} PGN_65090_HAPC;

// typedef struct {
// 	uint8_t		auga_menu_selector_cnt;
// 	uint8_t		auga_menu_selector_pos;
// } AUGA_HMI_CTRL_1;

typedef struct {
	uint8_t		cooling_on;
} AUGA_COOLING_1;

typedef struct {
	uint8_t		spn_5101_engine_coolant_level_low_lamp;
} PGN_64773_DLCD1;

typedef struct {
	uint16_t		auxilaryI_OChannel6;
	uint16_t		auxilaryI_OChannel5;
	uint16_t		auxilaryI_OChannel4;
	uint16_t		auxilaryI_OChannel3;
} PGN_2094_AUXIO5;

typedef struct {
	uint16_t		auxiliaryI_OChannel_1;
	uint16_t		auxiliaryI_OChannel_2;
	uint8_t			auxiliaryI_O_1;
	uint8_t			auxiliaryI_O_2;
	uint8_t			auxiliaryI_O_3;
	uint8_t			auxiliaryI_O_4;
	uint8_t			auxiliaryI_O_5;
	uint8_t			auxiliaryI_O_6;
	uint8_t			auxiliaryI_O_7;
	uint8_t			auxiliaryI_O_8;
	uint8_t			auxiliaryI_O_9;
	uint8_t			auxiliaryI_O_10;
	uint8_t			auxiliaryI_O_11;
	uint8_t			auxiliaryI_O_12;
	uint8_t			auxiliaryI_O_13;
	uint8_t			auxiliaryI_O_14;
	uint8_t			auxiliaryI_O_15;
	uint8_t			auxiliaryI_O_16;
} PGN_65241_AUXIO1;

typedef struct {
	uint8_t		spn_234_software_identification_1;
	uint8_t		spn_235_software_identification_2;
	uint8_t		spn_236_software_identification_3;
	uint8_t		spn_237_software_identification_year;
	uint8_t		spn_238_software_identification_month;
	uint8_t		spn_239_software_identification_day;
} PGN_65284_PROPB_04;

// Menu buttons on CLAAS button panel - Buttons
typedef struct {
	uint8_t		spn_301_Button_2_Push;
	uint8_t		spn_302_Button_2_Counter;
	uint8_t		spn_303_Button_4_Push;
	uint8_t		spn_304_Button_4_Counter;
	uint8_t		spn_305_Button_7_Push;
	uint8_t		spn_306_Button_7_Counter;
} PGN_65463_PROPB_B7;

// Menu buttons on CLAAS button panel - Knobs
typedef struct {
	uint8_t		spn_401_Knob_1_Rotate;
	uint8_t		spn_402_Knob_1_Push;
	uint8_t		spn_403_Knob_1_Push_Counter;
	uint8_t		spn_404_Knob_3_Rotate;
	uint8_t		spn_405_Knob_5_Rotate;
	uint8_t		spn_406_Knob_6_Rotate;
	uint8_t		spn_407_Knob_6_Push;
	uint8_t		spn_408_Knob_6_Push_Counter;
} PGN_65464_PROPB_B8;

// Danfoss PVED-CLS status and commands
typedef struct {
	uint8_t		pvedcls_state;
	uint8_t		reserved;
} PGN_65503_PROPB_DF;

// TEST MESSAGE (8 datapoints)
typedef struct {
	uint8_t		spn_901_Data_Field_1;
	uint8_t		spn_902_Data_Field_2;
	uint8_t		spn_903_Data_Field_3;
	uint8_t		spn_904_Data_Field_4;
	uint8_t		spn_905_Data_Field_5;
	uint8_t		spn_906_Data_Field_6;
	uint8_t		spn_907_Data_Field_7;
	uint8_t		spn_908_Data_Field_8;
} PGN_65433_PROPB_99;

typedef struct {
	uint8_t		isobus_a2110_auxilary_valve_port_flow_cmd;
	uint8_t		isobus_a2111_auxilary_valve_state_cmd;
	uint8_t		isobus_a2112_auxilary_valve_fail_safe_mode_cmd;
} PGN_65072_AVC;

typedef struct {
	uint16_t	Estimated_EH_Flow;
	uint16_t	Estimated_Wheel_Angle;
	uint8_t		Steering_Wheel_Revolutions;
	uint8_t		Steering_Wheel_Status;
} PGN_65304_DF_SAS;

typedef struct {
	uint16_t	PVED_CLS_AD1_X;
	uint16_t	PVED_CLS_AD2_X;
} PGN_65280_DF_STATUS1;

typedef struct {
	PGN_64982_BJM1			pgn_64982_Basic_Joystick_Message_1;
	PGN_64984_BJM2			pgn_64984_Basic_Joystick_Message_2;
	PGN_57434_CM1			pgn_57434_Cabin_Message_1;
	PGN_65280_PROPB_00		pgn_65280_Proprietary_B_00;
	PGN_65265_CCVS1			pgn_65265_Cruise_Control_Vehicle_Speed_1;
	PGN_65277_AF1			pgn_65277_Alternate_Fuel_1;
	PGN_256_TC1				pgn_256_Transmission_Control_1_GBR;
	PGN_256_TC1				pgn_256_Transmission_Control_1_GBF;
	PGN_64980_CM3			pgn_64980_Cabin_Message_3;
	PGN_65090_HAPC			pgn_65090_Hitch_And_PTO_Cmd;
	PGN_2094_AUXIO5			pgn_2094_Auxiliary_Input_5;
	PGN_65241_AUXIO1 		pgn_65241_AUXIO1;
	AUGA_COOLING_1			auga_cooling_1;
	PGN_64773_DLCD1			pgn_64773_DLCD_1;	
	PGN_65284_PROPB_04		pgn_65284_Proprietary_B_04;
	PGN_65463_PROPB_B7		pgn_65463_Proprietary_B_B7;
	PGN_65464_PROPB_B8		pgn_65464_Proprietary_B_B8;
	PGN_65503_PROPB_DF		pgn_65503_Proprietary_B_DF;
	PGN_65433_PROPB_99		pgn_65433_Proprietary_B_99;
	PGN_65072_AVC			pgn_65072_Auxilary_Valve_0_Cmd;
	PGN_65072_AVC			pgn_65072_Auxilary_Valve_1_Cmd;
	PGN_65072_AVC			pgn_65072_Auxilary_Valve_2_Cmd;
	PGN_65072_AVC			pgn_65072_Auxilary_Valve_3_Cmd;
	PGN_65304_DF_SAS		pgn_65304_Steering_Angle;
	PGN_65280_DF_STATUS1	pgn_65280_pved_cls_status1;
} J1939_db;

// --------------------------------------------------------------------------------
/// Use the define STAND_ALONE_VERSION to activate Codewarrior debugging:
/// Only for DEBUG with IndartOne or PEMicro-Multilink:
// --------------------------------------------------------------------------------
// #define STAND_ALONE_VERSION     // Debug => Runs without bootloader
// --------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Enable or disable RS232/485 serial communication interface (THIS IS NEEDED):
//#define SCI_USER_ACTIVE

//#define SCI_USER_TX_INT_HANDLE_ACTIVE
#define USER_BUFFER_SIZE 100

// Activate if needed RS485 type (for switching driver output enable "DE" - pin)
//#define SCI_USER_RS485

// Parity: 0=disabled, 1=even parity, 2=odd parity
#define SCI_USER_PARITY   0

// Data bits: 0=8 data bits, 1=9 data bits
#define SCI_USER_DATA_BITS_9  0

//Don't change that line:
#define SCI_NR  BIOS_SCI4
//--------------------------------------------------------------------------------
// For RS232 communication of SCI0 (e.g. mGateway) set the baudrate
// Choose Baudrate from the list:
#define SCI_USER_BAUDRATE BIOS_SCI_19200_BAUD
// Possible baudrates:
// BIOS_SCI_4800_BAUD
// BIOS_SCI_9600_BAUD
// BIOS_SCI_19200_BAUD
// BIOS_SCI_28800_BAUD
// BIOS_SCI_38400_BAUD
// BIOS_SCI_115200_BAUD

// Set a user-defined baudrate of SCIxBD:   (Calculation: Baud_Nr = BUS_FREQ_HZ/(16*Baud); BUS_FREQ_HZ=50*10^6, Baud= z.B. 19200)
//#define SCI_USER_DEFINED_BAUDRATE 163   //(Example: (50000000 / (16*19200)) = 163

// Copy these 3 lines to usercode_init() to enable the parity bit (SCI0 is RS232 on mGateway):
//SCI0CR1_M = 1;    // 0 = 8 data bits, 1 = 9 data bits
//SCI0CR1_PE = 1;   // 0 = No parity, 1 = Parity enabled (the last data bit is the parity bit)
//SCI0CR1_PT = 0;   // 0 = Even parity, 1 = Odd parity


//#define M2600_ACTIVATE_RS232_RX_INTERRUPT     //Activate SCI2 (e.g. RS232 of M2600)
//--------------------------------------------------------------------------------

// Read PWM duty cycle:

// Activate PWM duty cycle read
//#define PWM_DUTYCYCLE_READ_ACTIVE

// Specify expected PWM frequency in Hz
#define PWM_DUTYCYCLE_READ_FREQ   1000

// Specify desired duty cycle resolution in digits, measured value will be between 0 and res
#define PWM_DUTYCYCLE_READ_RES    100

// Specify Timer channel:
// M2600: 0=DI_26, 1=DI_25, 2=DI_24, 3=DI_23, 4=DI_22, 5=DI_21
#define PWM_DUTYCYCLE_READ_CH     0

// It's also possible to use the enhanced capture timer on your own
//#define ECT_ACTIVE

//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// Set the user-defined timer, see user_defined_int_timer() in user_code.c
//#define PID2_USER_DEFINED_CYCLE 1000    //microseconds, maximum value: 1311�s [ < (65535/50) ]

//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// Set the size of uint16 eeprom_userdaten[]
#define USERDATENSATZ_SIZE 56

// Set the size of uint32 user_variable[]
#define USER_VARIABLE_SIZE 10

// - optional if values are without effect -

// Set the size of uint16 eeprom_hsddaten[]
//#define HSDDATENSATZ_SIZE_MASTER 48

// Set the size of uint16 user_variable[]
//#define USER_VARIABLE_SIZE_MASTER 20
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// Set the size of CAN0 receive buffer A and B
#define OS_CAN0_MSG_QUEUE_A_SIZE 15
#define OS_CAN0_MSG_QUEUE_B_SIZE 15

// Set the size of CAN1 receive buffer A and B
#define OS_CAN1_MSG_QUEUE_A_SIZE 15
#define OS_CAN1_MSG_QUEUE_B_SIZE 15

// Set the size of CAN2 receive buffer A and B
#define OS_CAN2_MSG_QUEUE_A_SIZE 15
#define OS_CAN2_MSG_QUEUE_B_SIZE 15

// Set the size of CAN3 receive buffer A and B
#define OS_CAN3_MSG_QUEUE_A_SIZE 15
#define OS_CAN3_MSG_QUEUE_B_SIZE 15

// Set the size of CAN4 receive buffer A and B
#define OS_CAN4_MSG_QUEUE_A_SIZE 15
#define OS_CAN4_MSG_QUEUE_B_SIZE 15
//--------------------------------------------------------------------------------



// --------------------------------------------------------------------------------
// Function definition
// --------------------------------------------------------------------------------


/*----------------------------------------------------------------------------*/
/** 
* \brief    Initialize user components.  
* \details  This function is called once after startup. ADC-Values are available, but remember that 
*           Sampling can take up to 20 ms.
*           Here you should read your EEPROM values and initialize your functions.
*           
*           
* \return   void      
*/
void usercode_init(void);
/*----------------------------------------------------------------------------*/
/** 
* \brief    Cyclic main call for User.   
* \details  Write your own C-Code in here. Depending on the graph_cycle_time, 
*           this function is called more or less often (default is 20 ms). 
*           
* \return   void      
*/
void usercode(void);

void Open_SAE_J1939_Transmit_DB(uint8_t ecu_sa, uint32_t current_timestamp);

/*----------------------------------------------------------------------------*/
/** 
* \brief    Receive a message from CAN bus.   
* \details  Each received can message is placed into a receive buffer.
*           The main loop walks through this buffer and handles all the messages. This can be
*           either internally as part of the MRS protocol or a direct call of this function.
*           Remember: Only when CAN filter is open ( Set by DeveloperStudio or at  ) you get this message.
*           
* \param    hw_id [in] uint8    ID on which bus the message was from. Values are eg. #CAN_BUS_0.
* \param    msg   [in] bios_can_msg_typ* The message that was received. Use -> operator for access:
*      
* \return   void      
*/
void user_can_message_receive(uint8 hw_id, bios_can_msg_typ* msg);


/*----------------------------------------------------------------------------*/
/** 
* \brief   Serial Line receive interrupt. 
* \details This function is called, when a byte is received over serial line directly in interrupt. 
*          It is located in user_code.c directly below main usercode() function. 
*           
* \param    module [in] uint8_t The Module ID where message was received from (eg. BIOS_SCI0)
* \param    data   [in] uint8 The byte that was received
* \return   void 
*
*   Example Code:
*   \code{.c}
*   user_int_rx_sci(uint8_t module, uint8 data)   
*   {
*      if( module == BIOS_SCI0 )
*      {
*         fifo_put(data);
*      }
*   }
*   \endcode
*/
void user_int_rx_sci(uint8_t module, uint8 data);

/*----------------------------------------------------------------------------*/
/** 
* \brief    Serial line transmit interrupt.    
* \details  This function is called, when a byte transfer on serial line has finnished.
*           
*           
* \param    module [in] uint8_t  The Module that finnished the transfer.
* \return   void      
*/
void user_int_tx_sci(uint8_t module);

/*----------------------------------------------------------------------------*/
/** 
* \brief    This is a cyclic call, that is handled every ms.  
* \details  Please put not too much code into it, or timings will fail.
*           
*           
* \return   void      
*/
void user_int_timer_1ms(void);

/*----------------------------------------------------------------------------*/
/** 
* \brief    User-defined Timer PID2 interrupt Callback.   
* \details  This function is called, when user timer expires. This depends on its setup from
*           PID2_USER_DEFINED_CYCLE in this file.
*           
*           
* \return   void      
*/
void user_defined_int_timer(void);



/*----------------------------------------------------------------------------*/
/** 
* \brief    Enhanced Capture timer interrupt callback. 
* \details  This function is called, when an interrupt from enhanced capture timer happened.
*           This means eg. a PWM-in signal has changed its level. 
*           
*           
* \param    ch   [in] uint8_t    The timer channel that created this interrupt.
* \return   void      
*/
void user_int_ect(uint8_t ch);





#include "bios.h"
#include "os_timer.h"
#include "os_io.h"
#include "CAN_HCS12_BL.h"
#include "can_db.h"
#include "os_io_tables.h"
#include "lin_stack.h"
#include "os_sci.h"
#include "os_flash.h"
/** \}*/
#endif
