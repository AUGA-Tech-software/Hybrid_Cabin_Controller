//--------------------------------------------------------------------------
/// \file     user_code.c
/// \brief    user code
/// \author   R. Bacevicius / D. Vaitiekus
/// \date     2024-07-08
/// \version  2.0
/// \comment  file to write user specific code
//--------------------------------------------------------------------------
// AUGA Tech Hybrid M2 Tractror Cabin Controller Software on MRS M3600
//--------------------------------------------------------------------------

#include "user_code.h"
#include <stdio.h>
#include <stdlib.h>
#define LENGTH_OF(someArray) (sizeof(someArray)/sizeof(someArray[0]))

// --------------------------------------------------------------------------------
// Example variables
// --------------------------------------------------------------------------------
#define SoftwareVersion "V0.2.0" // Testing versions will have V9 at the start. Normal version - V0
#define SoftwareDate "2024.07.08"
uint16_t uint16_1, uint16_2, uint16_3, uint16_4;    // For DEBUG
uint8_t uint8_1, uint8_2, uint8_3, uint8_4;         // For DEBUG
uint32_t time_val, last_time, last_time1, last_time2, time_diff;
uint8_t i, j;
uint8_t can_data_bytes[8];
uint8_t current_gear = 0xFB;
bool gearR_en = FALSE;
uint8_t gearR_lastCnt = 0xFF;
uint16_t CalibratedY_Max = 4400;
uint16_t CalibratedY_Min = 700;  
uint16_t CalibratedY_Mid = 2300;
uint32_t sample_sum = 0;
uint16_t sample_count = 0;
uint16_t sensor_outputV = 0;
uint32_t average = 0;
uint32_t last_sample_time;
bool AUX1_flag = FALSE, AUX2_flag = FALSE, AUX3_flag = FALSE, AUX4_flag = FALSE;
uint8_t Left_cnt = 0, Up_cnt = 0, Right_cnt = 0, Down_cnt = 0, BackUp_cnt = 0, BackDown_cnt = 0;
uint16_t crc16_table[16] = {
    0x0000, 0xC86C, 0x58B4, 0x90D8, 0xB168, 0x7904, 0xE9DC, 0x21B0,
    0xAABC, 0x62D0, 0xF208, 0x3A64, 0x1BD4, 0xD3B8, 0x4360, 0x8B0C
};

// Counters for MMI messages
int spd_counter = 240,rsw_counter = 240;

J1939_db j1939_db;
uint32_t LastSampleTimeAux;
uint32_t sampleSumJoy=0,sampleSumJoy2=0, sampleSumRef=0, sampleSumAux4 = 0, sampleSumAux3 = 0, sampleSumAux2 = 0, sampleSumAux1 = 0, sampleCountAux = 0;
J1939_PGN pgn_list[] = {
		// PGN			RR		DL		SPN_st	SPN_len	ECU_ID		TXEn	nextTX
		{0x0CFDD6,		90,		8,		0,		16,		ECU_CABIN,	1,		0},	// PGN_64982_BJM1
        {0x0CFDD8,		90,		8,		16,		9,		ECU_CABIN,	1,		0},	// PGN_64984_BJM2
        {0x18E05A,		990,	8,		25,		2,		ECU_CABIN,	1,		0},	// PGN_57434_CM1
        {0x18FF00,		90, 	8,		27,		2,		ECU_CABIN,	1,		0},	// PGN_65280_PROPB_00
        {0x18FEF1,		90, 	8,		29,		3,		ECU_CABIN,	1,		0},	// PGN_65265_CCVS1
        {0x18FEFD,		490, 	8,		32,		1,		ECU_CABIN,	1,		0},	// PGN_65277_AF1
        {0x0C0103,		90, 	8,		33,		1,		ECU_CABIN,	1,		0},	// PGN_256_TC1
        {0x0C0104,		90, 	8,		34,		1,		ECU_CABIN,	1,		0},	// PGN_256_TC1
        {0x0CFDD4,		90,	    8,		35,		4,		ECU_CABIN,	1,		0},	// PGN_64980_CM3
        {0x0CFE42,		90,	    8,		39,		1,		ECU_CABIN,	1,		0},	// PGN_65090_HAPC
        {0x0C082E,		90,	    8,		40,		4,		ECU_CABIN,	1,		0},	// PGN_2094_AUXIO5 (AuxilaryInput_OutputStatus5)
        {0x18FED9,		90,	    8,		44,		18,		ECU_CABIN,	1,		0},	// PGN_65241_AUXIO1 (AuxilaryInput_OutputStatus1)
        {0x18EF3A,		990, 	8,		62,		1,		ECU_CABIN,	1,		0},	// AUGA_COOLING_1
        {0x18FD05,		990, 	8,		63,		1,		ECU_CABIN,	1,		0},	// PGN_64773_DLCD1
        {0x18FF04,		4990, 	8,		64,		6,		ECU_CABIN,	1,		0},	// PGN_65284_PROPB_04 (Software Version info msg)
        {0x18FFB7,		90, 	8,		70,		6,		ECU_CABIN,	1,		0},	// PGN_65463_PROPB_B7 (BTN panel (Buttons), Menu buttons msg)
        {0x18FFB8,		90, 	8,		76,		8,		ECU_CABIN,	1,		0},	// PGN_65464_PROPB_B8 (BTN panel (Knobs), Menu knobs msg)
        {0x18FFDF,		90, 	8,		84,		2,		ECU_CABIN,	1,		0},	// PGN_65503_PROPB_DF (Danfoss steering controller status)
        {0x18FF99,		90, 	8,		86,		8,		ECU_CABIN,	1,		0},	// PGN_65433_PROPB_99 (TEST MESSAGE DEBUG)
        {0x0CFE30,		90,	    8,		94,		3,		ECU_CABIN,	1,		0},	// PGN_65072_AV0C
        {0x0CFE31,		90,	    8,		97,		3,		ECU_CABIN,	1,		0},	// PGN_65072_AV1C
        {0x0CFE32,		90,	    8,		100,	3,		ECU_CABIN,	1,		0},	// PGN_65072_AV2C
        {0x0CFE33,		90,	    8,		103,	3,		ECU_CABIN,	1,		0}	// PGN_65072_AV3C
};

const J1939_SPN spn_list[] = {
	//	posBit	lenBit	variableInRAM
		// 0x00FDD6   ECU_0     PGN_64982_BJM1
		{16,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2676_y_axis_neutral_position_status},
        {18,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2671_y_axis_negative_position_status},
        {20,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2666_y_axis_positive_position_status},
        {22,	10,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2661_y_axis_position},           //  MUST BE FROM 22 AND 10 BIT LONG
        {40,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2688_button_4_pressed},
        {42,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2687_button_3_pressed},
        {44,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2686_button_2_pressed},
        {46,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2685_button_1_pressed},
        {48,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2692_button_8_pressed},
        {50,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2691_button_7_pressed},
        {52,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2690_button_6_pressed},
        {54,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2689_button_5_pressed},
        {56,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2696_button_12_pressed},
        {58,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2695_button_11_pressed},
        {60,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2694_button_10_pressed},
        {62,	2,		&j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2693_button_9_pressed},
    // 0x00FDD8   ECU_0     PGN_64984_BJM2
		{40,	2,		&j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2725_button_4_pressed},
        {42,	2,		&j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2724_button_3_pressed},
        {44,	2,		&j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2723_button_2_pressed},
        {46,	2,		&j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2722_button_1_pressed},
        {48,	2,		&j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2729_button_8_pressed},
        {50,	2,		&j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2728_button_7_pressed},
        {52,	2,		&j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2727_button_6_pressed},
        {54,	2,		&j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2726_button_5_pressed},
        {62,	2,		&j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2730_button_9_pressed},
    // 0x18E05A   ECU_0     PGN_57434_CM1
		{30,	2,		&j1939_db.pgn_57434_Cabin_Message_1.spn_1856_seat_belt_sw},
        {32,	2,		&j1939_db.pgn_57434_Cabin_Message_1.spn_5630_park_brake_cmd},
    //0x18FF00   ECU_0     PGN_65280_PROPB_00
        {0, 	2,		&j1939_db.pgn_65280_Proprietary_B_00.spn_1504_operator_seat_sw},
        {16, 	16,		&j1939_db.pgn_65280_Proprietary_B_00.joystick_Y_possition_2},
    // 0x00FEF1   ECU_0     PGN_65265_CCVS1
		{0, 	2,		&j1939_db.pgn_65265_Cruise_Control_Vehicle_Speed_1.spn_69_two_speed_axle_sw},
        {2, 	2,		&j1939_db.pgn_65265_Cruise_Control_Vehicle_Speed_1.spn_70_parking_brake_sw},
        {28, 	2,		&j1939_db.pgn_65265_Cruise_Control_Vehicle_Speed_1.spn_597_brake_sw},
    // 0x18FEFD   ECU_0     PGN_65277_AF1
		{8, 	16,		&j1939_db.pgn_65277_Alternate_Fuel_1.spn_159_engine_gaseous_fuel_supply_presure_1},
    // 0x000103   ECU_0     PGN_256_TC1
		{16, 	8,		&j1939_db.pgn_256_Transmission_Control_1_GBR.spn_525_transmission_requested_gear},
    // 0x000104   ECU_0     PGN_256_TC1
		{16, 	8,		&j1939_db.pgn_256_Transmission_Control_1_GBF.spn_525_transmission_requested_gear},
    // 0x00FDD4   ECU_0     PGN_64980_CM3
		{6, 	2,		&j1939_db.pgn_64980_Cabin_Message_3.spn_9842_momentary_operator_key_sw},
        {20, 	2,		&j1939_db.pgn_64980_Cabin_Message_3.spn_10145_operator_key_sw_ignition_power},
        {22, 	2,		&j1939_db.pgn_64980_Cabin_Message_3.spn_10146_operator_key_sw_start},
        {24, 	2,		&j1939_db.pgn_64980_Cabin_Message_3.spn_10147_operator_key_sw_delayed_battery_off},
    // 0x0CFE42   ECU_0     PGN_65090_HAPC
		{8, 	8,		&j1939_db.pgn_65090_Hitch_And_PTO_Cmd.isobus_a1904_rear_hitch_pos_cmd},
    // 0x0C082E   ECU_0     PGN_2094_AUXIO5
        {0, 	16,		&j1939_db.pgn_2094_Auxiliary_Input_5.auxilaryI_OChannel6},
        {16, 	16,		&j1939_db.pgn_2094_Auxiliary_Input_5.auxilaryI_OChannel5},
        {32, 	16,		&j1939_db.pgn_2094_Auxiliary_Input_5.auxilaryI_OChannel4},
        {48, 	16,		&j1939_db.pgn_2094_Auxiliary_Input_5.auxilaryI_OChannel3},
    // 0x18FED9   ECU_0     PGN_65241_AUXIO1
        {0,     2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_4},
        {2,     2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_3},
        {4,     2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_2},
        {6,     2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_1},
        {8,     2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_8},
        {10,    2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_7},
        {12,    2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_6},
        {14,    2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_5},
        {16, 	2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_12},
        {18, 	2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_11},
        {20, 	2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_10},
        {22, 	2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_9},
        {24, 	2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_16},
        {26, 	2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_15},
        {28, 	2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_14},
        {30, 	2, 	    &j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_13},
        {32, 	16, 	&j1939_db.pgn_65241_AUXIO1.auxiliaryI_OChannel_1},
        {48, 	16, 	&j1939_db.pgn_65241_AUXIO1.auxiliaryI_OChannel_2},
    // 0x18EF3A   ECU_0     Cooling_1
		{17, 	1,		&j1939_db.auga_cooling_1.cooling_on},
    // 0x18FD05   ECU_0     PGN_64773_DLCD_11
        {16, 	2,		&j1939_db.pgn_64773_DLCD_1.spn_5101_engine_coolant_level_low_lamp},
    // 0x18FF04   ECU_0     PGN_65284_PROPB_04
        {0, 	8,		&j1939_db.pgn_65284_Proprietary_B_04.spn_234_software_identification_1},
        {8, 	8,		&j1939_db.pgn_65284_Proprietary_B_04.spn_235_software_identification_2},
        {16, 	8,		&j1939_db.pgn_65284_Proprietary_B_04.spn_236_software_identification_3},
        {24, 	8,		&j1939_db.pgn_65284_Proprietary_B_04.spn_237_software_identification_year},
        {32, 	8,		&j1939_db.pgn_65284_Proprietary_B_04.spn_238_software_identification_month},
        {40, 	8,		&j1939_db.pgn_65284_Proprietary_B_04.spn_239_software_identification_day},
    // 0x18FFB7   ECU_0     PGN_65463_PROPB_B7 (BTN panel (Buttons), Menu buttons msg)
        {0, 	1,		&j1939_db.pgn_65463_Proprietary_B_B7.spn_301_Button_2_Push},        // BTN info
        {8, 	4,		&j1939_db.pgn_65463_Proprietary_B_B7.spn_302_Button_2_Counter},
        {16, 	1,		&j1939_db.pgn_65463_Proprietary_B_B7.spn_303_Button_4_Push},        // BTN settings / tab
        {24, 	4,		&j1939_db.pgn_65463_Proprietary_B_B7.spn_304_Button_4_Counter},
        {32, 	1,		&j1939_db.pgn_65463_Proprietary_B_B7.spn_305_Button_7_Push},        // BTN esc
        {40, 	4,		&j1939_db.pgn_65463_Proprietary_B_B7.spn_306_Button_7_Counter},
    // 0x18FFB8   ECU_0     PGN_65464_PROPB_B8 (BTN panel (Knobs), Menu knobs msg)
        {0, 	8,		&j1939_db.pgn_65464_Proprietary_B_B8.spn_401_Knob_1_Rotate},        // Encoder upper right
        {8, 	1,		&j1939_db.pgn_65464_Proprietary_B_B8.spn_402_Knob_1_Push},
        {16, 	4,		&j1939_db.pgn_65464_Proprietary_B_B8.spn_403_Knob_1_Push_Counter},
        {24, 	8,		&j1939_db.pgn_65464_Proprietary_B_B8.spn_404_Knob_3_Rotate},        // Knob lower right
        {32, 	8,		&j1939_db.pgn_65464_Proprietary_B_B8.spn_405_Knob_5_Rotate},        // Knob lower left
        {40, 	8,		&j1939_db.pgn_65464_Proprietary_B_B8.spn_406_Knob_6_Rotate},        // Encoder upper left
        {48, 	1,		&j1939_db.pgn_65464_Proprietary_B_B8.spn_407_Knob_6_Push},
        {56, 	4,		&j1939_db.pgn_65464_Proprietary_B_B8.spn_408_Knob_6_Push_Counter},
    // 0x18FFDF    ECU_0    PGN_65503_PROPB_DF (Danfoss steering controller status)
        {0, 	8,		&j1939_db.pgn_65503_Proprietary_B_DF.pvedcls_state},            // PVED-CLS State / Steering mode
        {63, 	1,		&j1939_db.pgn_65503_Proprietary_B_DF.pvedcls_reset_request},    // PVED-CLS Soft Reset request
    // 0x18FF99    ECU_0    PGN_65433_PROPB_99 (TEST MESSAGE DEBUG) TEST MESSAGE (8 datapoints)
        {0, 	8,		&j1939_db.pgn_65433_Proprietary_B_99.spn_901_Data_Field_1},
        {8, 	8,		&j1939_db.pgn_65433_Proprietary_B_99.spn_902_Data_Field_2},
        {16, 	8,		&j1939_db.pgn_65433_Proprietary_B_99.spn_903_Data_Field_3},
        {24, 	8,		&j1939_db.pgn_65433_Proprietary_B_99.spn_904_Data_Field_4},
        {32, 	8,		&j1939_db.pgn_65433_Proprietary_B_99.spn_905_Data_Field_5},
        {40, 	8,		&j1939_db.pgn_65433_Proprietary_B_99.spn_906_Data_Field_6},
        {48, 	8,		&j1939_db.pgn_65433_Proprietary_B_99.spn_907_Data_Field_7},
        {56, 	8,		&j1939_db.pgn_65433_Proprietary_B_99.spn_908_Data_Field_8},
        // 0x0CFE30   ECU_0     PGN_65072_AV0C
		{0, 	8,		&j1939_db.pgn_65072_Auxilary_Valve_0_Cmd.isobus_a2110_auxilary_valve_port_flow_cmd},
        {16, 	4,		&j1939_db.pgn_65072_Auxilary_Valve_0_Cmd.isobus_a2111_auxilary_valve_state_cmd},
        {22, 	2,		&j1939_db.pgn_65072_Auxilary_Valve_0_Cmd.isobus_a2112_auxilary_valve_fail_safe_mode_cmd},
    // 0x0CFE31   ECU_0     PGN_65072_AV1C
		{0, 	8,		&j1939_db.pgn_65072_Auxilary_Valve_1_Cmd.isobus_a2110_auxilary_valve_port_flow_cmd},
        {16, 	4,		&j1939_db.pgn_65072_Auxilary_Valve_1_Cmd.isobus_a2111_auxilary_valve_state_cmd},
        {22, 	2,		&j1939_db.pgn_65072_Auxilary_Valve_1_Cmd.isobus_a2112_auxilary_valve_fail_safe_mode_cmd},
    // 0x0CFE32   ECU_0     PGN_65072_AV2C
		{0, 	8,		&j1939_db.pgn_65072_Auxilary_Valve_2_Cmd.isobus_a2110_auxilary_valve_port_flow_cmd},
        {16, 	4,		&j1939_db.pgn_65072_Auxilary_Valve_2_Cmd.isobus_a2111_auxilary_valve_state_cmd},
        {22, 	2,		&j1939_db.pgn_65072_Auxilary_Valve_2_Cmd.isobus_a2112_auxilary_valve_fail_safe_mode_cmd},
    // 0x0CFE33   ECU_0     PGN_65072_AV3C
		{0, 	8,		&j1939_db.pgn_65072_Auxilary_Valve_3_Cmd.isobus_a2110_auxilary_valve_port_flow_cmd},
        {16, 	4,		&j1939_db.pgn_65072_Auxilary_Valve_3_Cmd.isobus_a2111_auxilary_valve_state_cmd},
        {22, 	2,		&j1939_db.pgn_65072_Auxilary_Valve_3_Cmd.isobus_a2112_auxilary_valve_fail_safe_mode_cmd},
  
};

const uint16_t bitmask_template[] = {
		0b0,
		0b1,
		0b11,
		0b111,
		0b1111,
		0b11111,
		0b111111,
		0b1111111,
		0b11111111	// 8
};

/* Generates Do Not Change bit mask by giving parameter start bit and lenght in bits.
 * E.g. gen_dnc_bitmask8(5, 2) gives 0b10011111 */
uint8_t gen_dnc_bitmask8(uint8_t pos_bit, uint8_t len_bit) {
	uint8_t bitmask = bitmask_template[len_bit] << pos_bit;
	bitmask = ~bitmask;
	return bitmask;
}

void Open_SAE_J1939_Transmit_DB(uint8_t ecu_sa, uint32_t current_timestamp) {
	for (i=0; i<LENGTH_OF(pgn_list); i++) {
		if (pgn_list[i].tx_enable && current_timestamp > pgn_list[i].next_tx_timestamp) {                           // Tikrina kada ar atėjo laikas siusti žinutė
			memset(&can_data_bytes, 0xFF, pgn_list[i].data_length_bytes);                                           // Užpildo visa žinute 0xFF
			for (j=pgn_list[i].spn_list_start; j<pgn_list[i].spn_list_start + pgn_list[i].spn_list_len; j++) {      // Eina per visus aprašytus spn parametrus
                uint8_t spn_pos_bit = spn_list[j].pos_bit;                                                          
                uint8_t spn_len_bit = spn_list[j].length_bit;                                                       // Nuskaito spn parametrus
				uint8_t spn_pos_byte = spn_pos_bit/8;                                                               
				if (spn_pos_byte == (spn_pos_bit + spn_len_bit - 1)/8) {	                                        // parameter fits in same byte  //dar nežinau kodel reikia žiuret ar telpa į viena byte
					uint8_t current_spn = *(uint8_t*) spn_list[j].variable << spn_pos_bit % 8;                      // set start bit within byte
					current_spn |= gen_dnc_bitmask8(spn_pos_bit % 8, spn_len_bit);
					can_data_bytes[spn_pos_byte] &= current_spn;
				} else if (spn_pos_byte + 1 == (spn_pos_bit + spn_len_bit - 1)/8) {	                                // parameter fits in two bytes
					uint16_t current_spn = *(uint16_t*) spn_list[j].variable << spn_pos_bit % 8;                    // Bitshift kad atitiktu start bito posicija
                    uint8_t current_spn_first_byte =  (uint8_t)((current_spn & 0xFF00) >> 8);                       //išskiriama į atskirus baitus kad butu galima atidėti start bita tik opirmam baitui
                    uint8_t current_spn_second_byte = (uint8_t)(current_spn & 0x00FF);
                    current_spn_second_byte |= gen_dnc_bitmask8(spn_pos_bit % 8, 8-spn_pos_bit % 8);                // Maskuojami tik naudojami bitai
                    current_spn_first_byte |= gen_dnc_bitmask8(0, spn_len_bit - 8 + spn_pos_bit % 8);               // Maskuojami tik naudojami bitai
                    current_spn = ((uint16_t)current_spn_second_byte << 8) | current_spn_first_byte;                // Sujungiami baitai akd butu intel byte order
					memcpy(&uint16_1, &can_data_bytes[spn_pos_byte], 2);
					current_spn &= uint16_1;
					memcpy(&can_data_bytes[spn_pos_byte], &current_spn, 2);
				}
			}
            os_can_send_msg(0, pgn_list[i].pgn << 8 | ecu_sa, 1, 8, can_data_bytes[0], can_data_bytes[1], can_data_bytes[2],
                            can_data_bytes[3], can_data_bytes[4], can_data_bytes[5], can_data_bytes[6], can_data_bytes[7]);
			pgn_list[i].next_tx_timestamp = current_timestamp + pgn_list[i].repetition_rate_ms;
		}
	}
}


void Auxilary_Section_Send_Ctrl_Cmd(PGN_65072_AVC *aux_valve_section, uint16_t value, bool *Flag,  uint8_t speed_Up,  uint8_t speed_Down) {
    if (value >=200 && value <=4800){
        if (value > 3700) {
            aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = 0;
            aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 3;  // FLOAT
            *Flag = TRUE;
        } else if (value > 2600) {
            aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = (value - 2640) * speed_Down / 40; // dalinta is 4 nes value * 2.5 / 10  where 2.5 is isobus constant and 10 is to scale precentige
            aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 1;  // EXTEND DOWN                  speed /10 = 0.x todel speed / 40         
            *Flag = TRUE;
        } else if (value < 2400 && value > 1300) {
            aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = (2330 - value) * speed_Up / 40;
            aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 2;  // RETRACT UP
            *Flag = TRUE;
        } else if ( value <= 1300) {
            aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = 25*speed_Up;
            aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 2;  // CONTINOUS RETRACT
            *Flag = TRUE;
        } else {
            if(*Flag){
                aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = 0;
                aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 0;  // BLOCK
            }
            *Flag = FALSE;
        }
    } else {
        aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = 254; //FE nes error value
        aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 0;  // block
    }
}
//Function for operating with AUXilary analog swiches
void Auxilary_Section_Send_Ctrl_Cmd_Joystick(PGN_65072_AVC *aux_valve_section, uint16_t value, bool Flag, uint8_t speed_Up, uint8_t speed_Down) {
    if (value == 1 && !Flag) {
        aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = 25 * speed_Down ; //25 = 10 * 2.5 where 2.5 is isobus constant and 10 is to scale precentige
        aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 1;  // EXTEND DOWN
    } else if (value == 2 && !Flag) {
        aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = 25 * speed_Up;
        aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 2;  // RETRACT UP
        
    } else if (value == 3 && !Flag) {
        aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = 0;
        aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 3;  // Float   
    }else if (!Flag ) {
        aux_valve_section->isobus_a2110_auxilary_valve_port_flow_cmd = 0;
        aux_valve_section->isobus_a2111_auxilary_valve_state_cmd = 0;  // BLOCK
    }

}

//Function for operating with programables joystick buttons
void Programable_Button_Action(uint8_t ButtonNr, uint8_t ButtonState, uint8_t *counter , uint8_t Cancounter){ 
    uint16_t speed = 4;
    uint16_t CANcount =  can_db_get_value(Cancounter);
    if(*counter < CANcount || ( *counter == 15 && CANcount== 0)){
         if ((ButtonNr == 1 || ButtonNr == 3 || ButtonNr == 5 || ButtonNr == 7) && ButtonState == 1){
                speed = 1;
            } else if((ButtonNr == 2 || ButtonNr == 4 || ButtonNr == 6 || ButtonNr == 8) && ButtonState == 1){ 
                speed = 2;
            } else {
                speed = 0;
            }
             *counter = CANcount;
        switch (ButtonNr){
            case 0:
                //Do nothing
                break;
            case 1:
            case 2:
                Auxilary_Section_Send_Ctrl_Cmd_Joystick(&j1939_db.pgn_65072_Auxilary_Valve_0_Cmd, speed, AUX1_flag,can_db_get_value(JOYSTIC_AUX1_SETSPEED_Up),can_db_get_value(JOYSTIC_AUX1_SETSPEED_Down));
                break;
            case 3:
            case 4:
                Auxilary_Section_Send_Ctrl_Cmd_Joystick(&j1939_db.pgn_65072_Auxilary_Valve_1_Cmd, speed, AUX2_flag,can_db_get_value(JOYSTIC_AUX2_SETSPEED_Up),can_db_get_value(JOYSTIC_AUX2_SETSPEED_Down));
                break;
            case 5:
            case 6:
                Auxilary_Section_Send_Ctrl_Cmd_Joystick(&j1939_db.pgn_65072_Auxilary_Valve_2_Cmd, speed, AUX3_flag,can_db_get_value(JOYSTIC_AUX3_SETSPEED_Up),can_db_get_value(JOYSTIC_AUX3_SETSPEED_Down));
                break;
            case 7:
            case 8:
                Auxilary_Section_Send_Ctrl_Cmd_Joystick(&j1939_db.pgn_65072_Auxilary_Valve_3_Cmd, speed, AUX4_flag,can_db_get_value(JOYSTIC_AUX4_SETSPEED_Up),can_db_get_value(JOYSTIC_AUX4_SETSPEED_Down));
                break;
            default:
                break;
        }
    }
}


void sendSoftwareInfo(void){
    int  major, minor, pach,year,month,day;
    (void)sscanf(SoftwareVersion, "V%d.%d.%d", &major, &minor, &pach);
    j1939_db.pgn_65284_Proprietary_B_04.spn_234_software_identification_1 = major; 
    j1939_db.pgn_65284_Proprietary_B_04.spn_235_software_identification_2 = minor;
    j1939_db.pgn_65284_Proprietary_B_04.spn_236_software_identification_3 = pach;
    (void)sscanf(SoftwareDate, "20%d.%d.%d", &year, &month, &day);
    j1939_db.pgn_65284_Proprietary_B_04.spn_237_software_identification_year = year; 
    j1939_db.pgn_65284_Proprietary_B_04.spn_238_software_identification_month = month;
    j1939_db.pgn_65284_Proprietary_B_04.spn_239_software_identification_day = day;
}
uint16_t calculateCRC(const uint8_t *data) {
    uint16_t crc = 0x0000; // Initial CRC value
    for (i = 0; i < 6; i++) {
        // Process high nibble
        uint8_t index = (crc >> 12) ^ (data[i] >> 4);
        crc = (crc << 4) ^ crc16_table[index];

        // Process low nibble
        index = (crc >> 12) ^ (data[i] & 0x0F);
        crc = (crc << 4) ^ crc16_table[index];
    }

    return crc;
}

/// \ingroup user
/// \brief           user-defined c-code INIT
//--------------------------------------------------------------------------
/// \return          None
//--------------------------------------------------------------------------
void usercode_init(void)
{
    // Set the SW-Version, maximal length=20
    (void)strcpy(EEPROM_SW_Version, SoftwareVersion);
    (void)strcpy(EEPROM_Modulname, "CabinController M1            ");
    sendSoftwareInfo();
    modulename_change = 1;
    os_digout(DOM_POWER_SENSOR_5V, 1);
    os_digout(D_REVERSE_BUZZER, 0);
    os_digout(D_REVERSE_LIGHT, 0);

    j1939_db.pgn_57434_Cabin_Message_1.spn_5630_park_brake_cmd = 0x3;               // NOT AVAILABLE FOR NOW
    j1939_db.pgn_65265_Cruise_Control_Vehicle_Speed_1.spn_69_two_speed_axle_sw = 0;
    j1939_db.pgn_65090_Hitch_And_PTO_Cmd.isobus_a1904_rear_hitch_pos_cmd = LENGTH_OF(pgn_list);
}



/// \ingroup user
/// \brief           user-defined c-code
//--------------------------------------------------------------------------
/// \return          None
//--------------------------------------------------------------------------
void usercode(void)
{
    if (os_digin(DI_KL15) == 0) {
        j1939_db.pgn_64980_Cabin_Message_3.spn_10145_operator_key_sw_ignition_power = 0;
        if (os_digin(DO_IGN_RELAY)) {
            j1939_db.pgn_64980_Cabin_Message_3.spn_10147_operator_key_sw_delayed_battery_off = 1;
        } else {
            j1939_db.pgn_64980_Cabin_Message_3.spn_10147_operator_key_sw_delayed_battery_off = 0;
        }
    } else {
        j1939_db.pgn_64980_Cabin_Message_3.spn_10145_operator_key_sw_ignition_power = 1;
        j1939_db.pgn_64980_Cabin_Message_3.spn_10147_operator_key_sw_delayed_battery_off = 0;
    }

    if (can_db_get_value(LVR_BTND_1)) {
        if (gearR_lastCnt != can_db_get_value(LVR_BTND_CNT)) {
            gearR_en = !gearR_en;
            gearR_lastCnt = can_db_get_value(LVR_BTND_CNT);
        }
    }

    if (os_algin_mv(A_PARKING_BRAKE_SW) < 2700) {
        current_gear = 0xFB;        // gear P
        gearR_en = FALSE;
    } else {
        if (os_algin_mv(A_TURTLE_RABBIT) > 3000) {
            if (gearR_en) {
                current_gear = 0x7B; // gear R2
            }   else{
                current_gear = 0x7F; // gear 2
            }     
        } else {
            if (gearR_en) {
                current_gear = 0x7C; // gear R1
            }   else{
                current_gear = 0x7E; // gear 1
            }           
        }
    }

    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2676_y_axis_neutral_position_status =  os_digin(D_JOYST_N_SW);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2671_y_axis_negative_position_status = os_digin(D_JOYST_RW_SW);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2666_y_axis_positive_position_status = os_digin(D_JOYST_FW_SW);
    //j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2661_y_axis_position = os_algin_mv(A_JOYST_POS1); 

    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2685_button_1_pressed = can_db_get_value(LVR_BR_UP_1);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2686_button_2_pressed = can_db_get_value(LVR_BR_UP_2);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2687_button_3_pressed = can_db_get_value(LVR_BR_DOWN_1);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2688_button_4_pressed = can_db_get_value(LVR_BR_DOWN_2);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2689_button_5_pressed = can_db_get_value(LVR_BR_LEFT_1);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2690_button_6_pressed = can_db_get_value(LVR_BR_RIGHT_1);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2691_button_7_pressed = can_db_get_value(LVR_SR_UP_1);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2692_button_8_pressed = can_db_get_value(LVR_SR_DOWN_1);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2693_button_9_pressed = can_db_get_value(LVR_SR_LEFT_1);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2694_button_10_pressed = can_db_get_value(LVR_SR_LEFT_2);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2695_button_11_pressed = can_db_get_value(LVR_SR_RIGHT_1);
    j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2696_button_12_pressed = can_db_get_value(LVR_SR_RIGHT_2);

    j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2726_button_5_pressed = can_db_get_value(LVR_BTNA_1);
    j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2727_button_6_pressed = can_db_get_value(LVR_BTNB_1);
    j1939_db.pgn_64984_Basic_Joystick_Message_2.spn_2728_button_7_pressed = can_db_get_value(LVR_BTNC_1);

    // j1939_db.auga_hmi_ctrl_1.auga_menu_selector_cnt = can_db_get_value(HMI_MENU_CNT);
    // j1939_db.auga_hmi_ctrl_1.auga_menu_selector_pos = can_db_get_value(HMI_MENU_POS);

    j1939_db.pgn_65280_Proprietary_B_00.spn_1504_operator_seat_sw = os_digin(D_SEAT_SW);

    j1939_db.pgn_65265_Cruise_Control_Vehicle_Speed_1.spn_70_parking_brake_sw = os_algin_mv(A_PARKING_BRAKE_SW) < 2700;
    j1939_db.pgn_65265_Cruise_Control_Vehicle_Speed_1.spn_597_brake_sw = os_digin(D_BRAKE_SW);

    // Menu BTNPNL retransmission:
    j1939_db.pgn_65464_Proprietary_B_B8.spn_401_Knob_1_Rotate = can_db_get_value(KNOB_1_ROT);
    j1939_db.pgn_65464_Proprietary_B_B8.spn_402_Knob_1_Push = can_db_get_value(KNOB_1_PRS);                 // Encoder upper right
    j1939_db.pgn_65464_Proprietary_B_B8.spn_403_Knob_1_Push_Counter = can_db_get_value(KNOB_1_CNT);
    j1939_db.pgn_65463_Proprietary_B_B7.spn_301_Button_2_Push = can_db_get_value(BTN_2_PRS);                // i Button
    j1939_db.pgn_65463_Proprietary_B_B7.spn_302_Button_2_Counter = can_db_get_value(BTN_2_CNT);
    j1939_db.pgn_65464_Proprietary_B_B8.spn_404_Knob_3_Rotate = can_db_get_value(KNOB_3_CNT);               // Turn knob right
    j1939_db.pgn_65463_Proprietary_B_B7.spn_303_Button_4_Push = can_db_get_value(BTN_4_PRS);                // tab Button
    j1939_db.pgn_65463_Proprietary_B_B7.spn_304_Button_4_Counter = can_db_get_value(BTN_4_CNT);
    j1939_db.pgn_65464_Proprietary_B_B8.spn_405_Knob_5_Rotate = can_db_get_value(KNOB_5_ROT);               // Turn knob left
    j1939_db.pgn_65464_Proprietary_B_B8.spn_406_Knob_6_Rotate = can_db_get_value(KNOB_6_ROT);
    j1939_db.pgn_65464_Proprietary_B_B8.spn_407_Knob_6_Push = can_db_get_value(KNOB_6_PRS);                 // Encoder upper left
    j1939_db.pgn_65464_Proprietary_B_B8.spn_408_Knob_6_Push_Counter = can_db_get_value(KNOB_6_CNT);
    j1939_db.pgn_65463_Proprietary_B_B7.spn_305_Button_7_Push = can_db_get_value(BTN_7_PRS);                // esc button
    j1939_db.pgn_65463_Proprietary_B_B7.spn_306_Button_7_Counter = can_db_get_value(BTN_7_CNT);
    
    //Auxilary section mV transmission
    sampleSumAux1 += os_algin_mv(A_AUX1_SIGN2);
    sampleSumAux2 += os_algin_mv(A_AUX2_SIGN2);
    sampleSumAux3 += os_algin_mv(A_AUX3_SIGN2);
    sampleSumAux4 += os_algin_mv(A_AUX4_SIGN2);
    sampleSumJoy += os_algin_mv(A_JOYST_POS1);
    sampleSumJoy2 += os_algin_mv(A_JOYST_POS2);
    sampleSumRef += os_algin_mv(AIM_SENSOR_5V);
    sampleCountAux++;
    
    if (os_time_past(LastSampleTimeAux, 100, OS_1ms)) {
        // Calculate the average
        j1939_db.pgn_2094_Auxiliary_Input_5.auxilaryI_OChannel3 = sampleSumAux1 / sampleCountAux;
        j1939_db.pgn_2094_Auxiliary_Input_5.auxilaryI_OChannel4 = sampleSumAux2 / sampleCountAux;
        j1939_db.pgn_2094_Auxiliary_Input_5.auxilaryI_OChannel5 = sampleSumAux3 / sampleCountAux;
        j1939_db.pgn_2094_Auxiliary_Input_5.auxilaryI_OChannel6 = sampleSumAux4 / sampleCountAux;
        j1939_db.pgn_65241_AUXIO1.auxiliaryI_OChannel_2 = sampleSumJoy / sampleCountAux;
        j1939_db.pgn_65280_Proprietary_B_00.joystick_Y_possition_2 = sampleSumJoy2 / sampleCountAux;
        j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2661_y_axis_position = sampleSumJoy / sampleCountAux; 
        //Refference voltage transmission
        j1939_db.pgn_65241_AUXIO1.auxiliaryI_OChannel_1 = sampleSumRef / sampleCountAux;
        // Reset the sum and count for the next second
        sampleSumAux1 = 0;
        sampleSumAux2 = 0;
        sampleSumAux3 = 0;
        sampleSumAux4 = 0;
        sampleSumJoy = 0;
        sampleSumJoy2 = 0;
        sampleSumRef = 0;
        sampleCountAux = 0;

        os_timestamp(&LastSampleTimeAux, OS_1ms);
    }

    //Joystick voltage transmission
    //j1939_db.pgn_65241_AUXIO1.auxiliaryI_OChannel_2 = os_algin_mv();

    //Calculating average value of PGN_64707_Gaseous_Fuel_Information_4.spn_5867_supply_gas_methane_percentage
    sample_sum += os_algin_mv(A_CNG_PRESSURE);
    sample_count++;
    sensor_outputV = os_algin_mv(AIM_SENSOR_5V) / 10; //10% sensor output voltage
    if (os_time_past(last_sample_time, 1000, OS_1ms)) {
        // Calculate the average
        average = sample_sum / sample_count;
        if (average >=  sensor_outputV){
            average = ((average-sensor_outputV)*100)/1538;  //From datasheet sensitivity 15.38 bar/mV
            j1939_db.pgn_65277_Alternate_Fuel_1.spn_159_engine_gaseous_fuel_supply_presure_1 = (uint16_t)average*2; // /2 becouse J1939 gain is 0,5
        } else {
            j1939_db.pgn_65277_Alternate_Fuel_1.spn_159_engine_gaseous_fuel_supply_presure_1 = 0; //Value below 10% of sensor's output voltage
        }
        // Reset the sum and count for the next second
        sample_sum = 0;
        sample_count = 0;
        // Update the last sample time
        os_timestamp(&last_sample_time, OS_1ms);
    }
    j1939_db.pgn_256_Transmission_Control_1_GBR.spn_525_transmission_requested_gear = current_gear;

    j1939_db.pgn_256_Transmission_Control_1_GBF.spn_525_transmission_requested_gear = current_gear;

    if (current_gear == 0x7F) { // Gear 2
        j1939_db.pgn_65265_Cruise_Control_Vehicle_Speed_1.spn_69_two_speed_axle_sw = 1; // High Speed Range
    } else if (current_gear == 0x7E || current_gear == 0x7C || current_gear == 0xFB) {
        j1939_db.pgn_65265_Cruise_Control_Vehicle_Speed_1.spn_69_two_speed_axle_sw = 0; // Low Speed Range
    } else {
        j1939_db.pgn_65265_Cruise_Control_Vehicle_Speed_1.spn_69_two_speed_axle_sw = 2; // Error
    }
    if( can_db_get_value(GBR_CURRENT_GEAR) == 0x7C ||  can_db_get_value(GBF_CURRENT_GEAR) == 0x7C || gearR_en ){ //Turn on buzzer and reverse light when reverse gear is selected on either of the gear boxes (front or rear)
        os_digout(D_REVERSE_LIGHT, 1);
        os_digout(D_REVERSE_BUZZER, 1);
    } else{
        os_digout(D_REVERSE_LIGHT, 0);
        os_digout(D_REVERSE_BUZZER, 0);
    }
    if (os_digin(D_AIR_CONDITIONER_SW)){ //When air conditioner pin is high send CAN message
        j1939_db.auga_cooling_1.cooling_on=1;
    }else{
        j1939_db.auga_cooling_1.cooling_on=0;
    }
    j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_1=os_digin(D_ROADTRAV_SW1);
    j1939_db.pgn_65241_AUXIO1.auxiliaryI_O_2=os_digin(D_ROADTRAV_SW2);
    //if engine coolant level is too low send CAN message
    if (os_algin_mv(A_ENGINE_COOLANT) > 5000) {
        //insert message here
        j1939_db.pgn_64773_DLCD_1.spn_5101_engine_coolant_level_low_lamp = 1;
    } else {
        j1939_db.pgn_64773_DLCD_1.spn_5101_engine_coolant_level_low_lamp = 0;
    }


    /*Auxilary_Section_Send_Ctrl_Cmd(&j1939_db.pgn_65072_Auxilary_Valve_0_Cmd, os_algin_mv(A_AUX1_SIGN2) , &AUX1_flag, can_db_get_value(JOYSTIC_AUX1_SETSPEED_Up),can_db_get_value(JOYSTIC_AUX1_SETSPEED_Down));
    Auxilary_Section_Send_Ctrl_Cmd(&j1939_db.pgn_65072_Auxilary_Valve_1_Cmd, os_algin_mv(A_AUX2_SIGN2) , &AUX2_flag, can_db_get_value(JOYSTIC_AUX2_SETSPEED_Up),can_db_get_value(JOYSTIC_AUX2_SETSPEED_Down));
    Auxilary_Section_Send_Ctrl_Cmd(&j1939_db.pgn_65072_Auxilary_Valve_2_Cmd, os_algin_mv(A_AUX3_SIGN2) , &AUX3_flag, can_db_get_value(JOYSTIC_AUX3_SETSPEED_Up),can_db_get_value(JOYSTIC_AUX3_SETSPEED_Down));
    Auxilary_Section_Send_Ctrl_Cmd(&j1939_db.pgn_65072_Auxilary_Valve_3_Cmd, os_algin_mv(A_AUX4_SIGN2) , &AUX4_flag, can_db_get_value(JOYSTIC_AUX4_SETSPEED_Up),can_db_get_value(JOYSTIC_AUX4_SETSPEED_Down));
    */
    //Programable joystick button commands: 

    Programable_Button_Action(can_db_get_value(JOYSTICK_SETUP_1),j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2693_button_9_pressed, &Left_cnt, LVR_SR_LEFT_CNT);
    Programable_Button_Action(can_db_get_value(JOYSTICK_SETUP_2),j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2691_button_7_pressed, &Up_cnt, LVR_SR_UP_CNT);
    Programable_Button_Action(can_db_get_value(JOYSTICK_SETUP_3),j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2695_button_11_pressed, &Right_cnt, LVR_SR_RIGHT_CNT);
    Programable_Button_Action(can_db_get_value(JOYSTICK_SETUP_4),j1939_db.pgn_64982_Basic_Joystick_Message_1.spn_2692_button_8_pressed, &Down_cnt, LVR_SR_DOWN_CNT);
    Programable_Button_Action(can_db_get_value(JOYSTICK_SETUP_5),can_db_get_value(LVR_BCK_UP), &BackUp_cnt, LVR_BCK_UP_CNT);
    Programable_Button_Action(can_db_get_value(JOYSTICK_SETUP_6),can_db_get_value(LVR_BCK_DOWN), &BackDown_cnt, LVR_BCK_DOWN_CNT);

    //j1939_db.pgn_65241_AUXIO1.spn_1083_auxiliary_IO_channel_1 = os_algin_mv(A_JOYST_POS1); //used for debuging analog values

    //danfos system messages MMI interface
    //-----------------------------------------------------------
    os_timestamp(&time_val, OS_1ms);
    time_diff =  time_val - last_time;
    if(time_diff >= 100){
        //send speed CAN message
        uint16_t crc_result;
        uint8_t direction = 0xFC | gearR_en;
        uint16_t speed = can_db_get_value(Wheel_Based_Vechcle_Speed);
        uint8_t can_message[8];
        can_message[0] = 0xFF;
        can_message[1] = direction;
        can_message[2] = (speed >> 8) & 0xFF; // High byte of speed
        can_message[3] = speed & 0xFF;        // Low byte of speed
        can_message[4] = 0xFF;
        can_message[5] = spd_counter;
        crc_result = calculateCRC(can_message);
        can_message[6] = crc_result & 0xFF; // Low byte
        can_message[7] = crc_result >> 8; // High byte
        os_can_send_msg(CAN_BUS_2, 0x0CFF40FB, 1, 8, can_message[0], can_message[1], can_message[2],can_message[3], can_message[4], can_message[5], can_message[6], can_message[7]);
        os_can_send_msg(CAN_BUS_2, 0x0CFF41FB, 1, 8, can_message[0], can_message[1], can_message[2],can_message[3], can_message[4], can_message[5], can_message[6], can_message[7]);

        spd_counter += 1;  
        if (spd_counter >= 256) {
            spd_counter = 240;
        }
        os_timestamp(&last_time, OS_1ms);
    }
    time_diff =  time_val - last_time1;
    if(time_diff >= 500){
        //send speed CAN message
        uint16_t crc_result;
        uint8_t offRoad = 0x00;
        uint8_t can_message1[8];
        if (os_digin(D_ROADTRAV_SW1))
            offRoad = 0x10;
        can_message1[0] = 0x00;
        can_message1[1] = offRoad;
        can_message1[2] = 0x7F;
        can_message1[3] = 0x30;
        can_message1[4] = 0xFF;
        can_message1[5] = rsw_counter;
        crc_result = calculateCRC(can_message1);
        can_message1[6] = crc_result & 0xFF; // Low byte
        can_message1[7] = crc_result >> 8; // High byte
        os_can_send_msg(CAN_BUS_2, 0x0CEF13FC, 1, 8, can_message1[0], can_message1[1], can_message1[2],can_message1[3], can_message1[4], can_message1[5], can_message1[6], can_message1[7]);
        can_message1[0] = 0x02;
        crc_result = calculateCRC(can_message1);
        can_message1[6] = crc_result & 0xFF; // Low byte
        can_message1[7] = crc_result >> 8; // High byte
        os_can_send_msg(CAN_BUS_2, 0x0CEF5AFC, 1, 8, can_message1[0], can_message1[1], can_message1[2],can_message1[3], can_message1[4], can_message1[5], can_message1[6], can_message1[7]);

        rsw_counter += 1;  
        if (rsw_counter >= 256) {
            rsw_counter = 240;
        }
        os_timestamp(&last_time1, OS_1ms);
    }

    // PVED-CLS Operation status message gateway:
    // Reikia persiųsti status iš CAN2 į CAN1 0x18FFDF28 adresą.
    j1939_db.pgn_65503_Proprietary_B_DF.pvedcls_state = can_db_get_value(PVED_CLS_Current_Op_State);

    // Jeigu CAN1 aptinkamas 0x18FFDF28 paskutinis bit'as 1 - siunčiama reset comanda į CAN2
    if (j1939_db.pgn_65503_Proprietary_B_DF.pvedcls_reset_request = 1) {
        os_can_send_msg(CAN_BUS_2, 0x0CEF00FC, 1, 8, 0x96, 0xA5, 0xA5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    }
    // MMI Messages END -----------------------------------------------------------

    os_timestamp(&time_val, OS_1ms);

    Open_SAE_J1939_Transmit_DB(0x31, time_val);
}
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    // 
    // Instructions for programming in Code Warrior  
    //
    // To start programming in Code Warrior first create a project in the Deverlopers Studio 
    //  1. Select the module you want 
    //  2. Compile the project 
    //  3. Copy the generated can_db-, os_io- and modulhardwarecode-files in the source-folder of your CW project
    
    //  If you want to change CAN baudrate or CAN filter / masks later, this is directly possible in can_db_tables.h
    //  Otherwise the changes can also be done in the DS project and then the files copied again into the CW project. 
    //
    //  The latest CodeWarrior Version can be found here:
    //  => http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=CW-HCS12X&nodeId=0152102726E4C7E4CB#
    //
    //  Downloadlink: http://cache.freescale.com/lgfiles/devsuites/HC12/CW_HC12_v5.1_SPECIAL.exe?fpsp=1
    //-----------------------------------------------------------
    //-----------------------------------------------------------


    /*

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // INPUTS AND OUTPUTS
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    // Procedure	uint8_t os_digin(uint16_t i);
    // Overview	    read a digital input
    // Input		    digital input, see #os_port_pin in os_io_tables.c
    // Output	      state of the pin
    //--------------------------------------------------------------------------

    // Example: ("D_IN0" = name of digital input, see #os_port_pin in os_io_tables.c):

    variable1 = os_digin(D_IN0);



    //--------------------------------------------------------------------------
    // Procedure	void os_digout(uint16_t i, uint8_t value);
    // Overview	    set a digital output
    // Input		    i       digital output, see #os_port_pin in os_io_tables.c
    //              value   value of the output
    // Output	      None
    //--------------------------------------------------------------------------

    // Example: ("OUT_HSD0" = name of digital ouput; "1" = output is set to 1):

    os_digout(OUT_HSD0, 1);



    //--------------------------------------------------------------------------
    // Procedure	uint16_t os_algin_mv(uint8_t i)
    // Overview	    read an analog input
    // Input		    i     analog input, see #os_analog_pin in os_io_tables.c
    // Output	      value of the pin in mV
    //--------------------------------------------------------------------------

    // Example: ("ANA0" = name of analog input, see #os_analog_pin in os_io_tables.c)

    variable1 =  os_algin_mv(ANA0);





    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // CAN-BUS
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    // Procedure	uint8_t os_can_send_msg(uint8_t hw_id, uint32_t id, uint8_t id_type,  uint8_t dlc,
    //                       uint8_t byte0 , uint8_t byte1 , uint8_t byte2 , uint8_t byte3 ,
    //                       uint8_t byte4 , uint8_t byte5 , uint8_t byte6 , uint8_t byte7);
    // Overview	    send an user-defined CAN message
    // Input		    hw_id     "0" CAN0, "1" CAN1, "2" CAN2
    //              id        CAN identifier
    //              id_type   "0" 11bit id, "1" 29bit id
    //              dlc       data length
    //              byte 0-7  data of the message
    // Output	      0 = Success, 1 = Error (TX-Buffer full, CAN not initialized, wrong parameters)
    //--------------------------------------------------------------------------

    // Example:

    ret = os_can_send_msg(0, 0x720, 0, 8, 0x66, 0x33, 0x11, 0, 0, 0, 0, 0);



    //--------------------------------------------------------------------------
    // Procedure	void user_can_message_receive(uint8_t hw_id, bios_can_msg_typ* msg);
    // Overview	    THE PROCEDURE IS AT THE END OF THIS FILE
    //              It's the simplest way to evaluate a received CAN message
    //              When a message is received it is stored in a buffer. Every
    //              cycle time the buffer is read, and when something is in
    //              the procedure is called
    // Input		    -
    // Output	      -
    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------
    // Procedure	void can_db_transmit_deactivate(uint8 state);
    // Overview     Stopps sending of all can Blocks.
    // Input		state uint8 value for all can dbs, 1 = stop, 0 = run.
    //
    // Output	    None
    //--------------------------------------------------------------------------

    // Example:

  	can_db_transmit_deactivate(1);



    //--------------------------------------------------------------------------
    // Procedure    void can_db_bus_transmit_stop(uint8_t state, uint8_t bus_id);
    // Overview     Stopps sending on selected CAN bus.
    // Input        state uint8 value,  deactivate / stop transmission = 1, activate / start transmission = 0
    //              bus_id uint8 value,  index of CAN-Bus
    //
    // Output	    None
    //--------------------------------------------------------------------------

    // Example: Stop transmission on CAN2

  	void can_db_bus_transmit_stop(1, CAN_BUS_2);



    //--------------------------------------------------------------------------
    // Procedure	uint32_t can_db_get_value(uint32_t name_signal);
    // Overview	    read a CAN message
    // Input		    bus_nr        "0"
    //              name_signal   name of datapoint, see #can1_datenpunkt_db_const
    //                            in can_db_tables.c
    // Output	      value of datapoint
    //--------------------------------------------------------------------------

    // Example: ("POINT1" = datapoint-name):

    variable1 =  can_db_get_value(POINT1);



    //--------------------------------------------------------------------------
    // Procedure	uint8_t can_db_test_dp_value(uint32_t id);
    // Overview	    see whether the CAN message with that datapoint is received
    // Input        id        name of datapoint, see #can1_datenpunkt_db_const
    //                        in can_db_tables.c
    // Output	      "0" no message received,
    //              "1" message received, datapoint has same value,
    //              "2" message received, datapoint has another value
    //--------------------------------------------------------------------------

    // Example: ("POINT1" = datapoint-name):

    variable1 =  can_db_test_dp_value(POINT1);

    //--------------------------------------------------------------------------
    // Procedure	void can_db_set_value(uint32_t name_signal, uint32_t wert_int);
    // Overview	    set the value of a datapoint (it's sent automatically)
    // Input        name_signal   name of datapoint, see #can1_datenpunkt_db_const
    //                            in can_db_tables.c
    //              wert_int      specify value of datapoint
    // Output	      None
    //--------------------------------------------------------------------------

    // Example: ("POINT1" = datapoint-name; "23" = datapoint is set to 23);

    can_db_set_value(POINT2, 23);



    //--------------------------------------------------------------------------
    // Procedure	void can_db_set_transmit_flag(uint32_t id );
    // Overview	    manually send a message defined in graphical programming
    // Input        id        name of datapoint, see #can1_datenpunkt_db_const
    //                        in can_db_tables.c
    // Output	      None
    //--------------------------------------------------------------------------

    // Example: ("POINT2" = datapoint-name)

    can_db_set_transmit_flag(POINT2);

    // If you want to send faster than 10ms you have to set

    can_cycle_time_transmit = 1;

    // Be aware that too fast sending may lead to high bus load and may disturb bootloader communication



    //--------------------------------------------------------------------------
    // Procedure    void os_can_set_baudrate (uint8_t bus_id, uint8_t can_baudrate);
    // Overview     Change baudrate during running time.
    //              Make sure that the resulting error frames are handled and the function is only called once.
    //              After restart, the baud rate is reset to the default value.
    // Input        bus_id uint8_t  CAN bus module number (CAN_BUS_0, CAN_BUS_1, CAN_BUS_2, CAN_BUS_3, CAN_BUS_4)
    //              can_baudrate uint8_t  CAN baudrate, e.g. CAN_BAUD_USER_125, CAN_BAUD_USER_250, CAN_BAUD_USER_500, see enum_USER_CAN_BAUDRADE in os_can.h
    //
    // Output	    None
    //--------------------------------------------------------------------------

    // Example: Set CAN0 to 250kBaud

  	void os_can_set_baudrate (CAN_BUS_0, CAN_BAUD_USER_250);



    //--------------------------------------------------------------------------
    // Procedure    void can_db_tables_set_tx_wait( uint16_t value )
    // Overview     Set the bios_can_tx_buffer_full_wait variable
    //              This variable controls how long the controller looks for a free transmit buffer.
    //              Note that this search is a CPU blocking operation (do-while loop).
    //              If problems like CAN transmit timing issues or "lost message" are experienced,
    //              the CAN transmit behavior can be "fine-tuned" by changing this value.
    //              If timing behavior is most important for your system, choose a lower value,e.g. 200
    //              If it's most important that no message is lost and all messages are sent in
    //              the order they came into the receive buffer, choose a higher value, e.g. 5000
    // Input        value uint16_t default value = 1000.
    //
    // Output	    None
    //--------------------------------------------------------------------------

    // Example:

  	can_db_tables_set_tx_wait(200);




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // TIME BEHAVIOR
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // check if a time of 500ms passed since last timestamp
    if( os_time_past(time_val, 500, OS_1ms) )
    {
        // if the time passed, set "time_val" to a new timestamp to start cycling
        os_timestamp(&time_val, OS_1ms);
    }



    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // USER VARIABLE FOR DATA EXCHANGE WITH GRAPHICAL PROGRAMMING
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // Index 0 - 9 as 32bit values, same index as in graphical programming
    variable1 = (uint16_t)user_variable[0];



    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // EEPROM SAVE AND READ DATA
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // The array eeprom_userdaten[] with Index 0 - 50 is read from EEPROM on startup
    // You can read and write to that array as often you want to
    variable1 = eeprom_userdaten[0];

    // If you want to save that array to the EEPROM call (maximal 100.000 times)
    os_eeprom_write_all(EEPROM_USER);



    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // EEPROM DIRECT ROUTINES
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // User-EEPROM-Range (Addr) is 0x000 - 0x9FF  (=2560 bytes)
    // With the following routines you can read and write to user-EEPROM range for
    // the specified number (Size) in bytes.

    // Read any EEPROM data:

    //--------------------------------------------------------------------------
    // Procedure	uint8_t os_eeprom_read_data(uint16_t Addr, uint8_t *Data, uint16_t Size);
    // Overview     Read EEPROM data
    // Input        Addr      Start address, range 0x000 - 0x9FF
    //              Size      Length in bytes
    // Output	      *Data     Pointer to uint8_t array
    // Return       0: ERR_OK, > 0: Error occurred (see PE_Error.h for error possibilities)
    //--------------------------------------------------------------------------

    e.g.
    ret = os_eeprom_read_data(addr, &data_arr[0], 8);


    // Write any EEPROM data:

    //--------------------------------------------------------------------------
    // Procedure	uint8_t os_eeprom_write_data(uint16_t Addr, uint8_t *Data, uint16_t Size);
    // Overview     Write EEPROM data
    // Input        Addr      Start address, range 0x000 - 0x9FF
    //              Size      Length in bytes
    // Output	      *Data     Pointer to uint8_t array
    // Return       0: ERR_OK, > 0: Error occurred (see PE_Error.h for error possibilities)
    //--------------------------------------------------------------------------

    e.g.
    ret = os_eeprom_write_data(addr, &data_arr[0], 8);




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // SETTING A PWM
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Procedure	void os_pwm_duty_cycle(uint8_t i, uint16_t pwm_promille, uint16_t pwm_frequency, uint16_t pwm_dither_promille, uint16_t pwm_dither_frequency)
    // Overview	    Set a pwm duty cycle and frequency at outputs OUT_HSD2-7 on CAN I/O
    // Input		    i               0: PWM0, 1: PWM1, 2: PWM2, ..., 7: PWM7
    //              duty_cycle      in 1/1000 (value range from 0 - 1000)
    //              pwm_frequency   20Hz - 1kHz (1digit / Hz), max frequency depended from high side driver
    // Output	      None
    //--------------------------------------------------------------------------

    // Example: (on OUT_HSD0 a pwm with 150Hz and 50% duty cycle)

    os_pwm_duty_cycle(0, 500, 150, 0, 0);





    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // SEND AND RECEIVE RS232 DATA WITH SERIAL COMMUNCATION INTERFACE (SCI)
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // - Go to user_code.h and specify the defines
  	// - Initialize SCI in usercode_init, call sci_init();
    //--------------------------------------------------------------------------
    // Procedure    void sci_buffer_send(void)
    // Overview     After you filled out the array, start sending
    //
    // Input        module      SCI module number SCI0 = 0, SCI1 = 1,... mGateway SCI0 is used
    //              value       byte to send
    // Output       none
    //--------------------------------------------------------------------------
    // Example1:
     * usercode_init
     * {
     * sci_init();
     * }
     *
    sci_tx_buffer.data[0] = 0x22;
    sci_tx_buffer.data[1] = 0x33;
    sci_tx_buffer.data[2] = 0x44;
    sci_tx_buffer.max = 3;
    sci_buffer_send();

    // To receive data goto to function user_int_rx_sci() at the end of this file




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // READ A PWM DUTY CYCLE WITH THE M2600
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // Set #define PWM_DUTYCYCLE_READ_ACTIVE in user_code.h
    // Call os_init_pwm_duty_read(); in void usercode_init(void)

    variable1 = os_pwm_duty_read[0].duty;
    variable2 = os_pwm_duty_read[0].frequency;

    // os_pwm_duty_read[i].duty;
    // os_pwm_duty_read[i].frequency;

    // --> i = Timer channel, see PWM_DUTYCYCLE_READ_CH in user_code.h




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // READ FREQUENCY
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    // Procedure	uint16_t os_frequency_calc(uint8_t channel_def)
    // Overview	Read frequency
    // Input		channel_def     D_ANA3
    // Output	    frequency value
    //--------------------------------------------------------------------------

    ret = os_frequency_calc(D_ANA3);
    
    
    
    //--------------------------------------------------------------------------
    // Procedure    void os_frequency_read(uint8_t channel_def, uint32_t* measure_period_timestamp, uint16_t* measured_frequency)
    // Overview     Read frequency
    // Input        channel_def                  name of digital input. see os_io_tables.c
    //              measure_period_timestamp     time variable
    // Output       measured_frequency           frequency at selected pin
    //--------------------------------------------------------------------------

    // Activate ECT_ACTIVE in user_code.h
    // to initialize call os_init_frequency_read in user_init.

    // Example: Read frequency at DI_RPM

    os_frequency_read(DI_RPM, &time_val, &variable1);




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // READ ENCODER SIGNALS
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    // Procedure	uint16_t os_incremental_encoder(uint8_t mode_type, uint8_t channel_def1, uint8_t channel_def2, uint16_t min, uint16_t max)
    // Overview	Read and calculate 2 encoder signals which have a 90� phase shift
    // Input		mode_type       PORT_INT_INCR_ENCODER
    //            channel_def1    D_ANA3
    //            channel_def2    D_ANA5
    //            min             0
    //            max             100 (if min=max=0, value won't be limited. To get signed value read with cast (int16_t).
    // Output	    actual counter value
    //--------------------------------------------------------------------------

    ret = os_incremental_encoder(PORT_INT_INCR_ENCODER, D_ANA3, D_ANA5, 0, 100);




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // USE LOOKUP TABLE FUNCTIONS
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    int16_t os_util_lookup1D(int16_t *table_x, int16_t *table_y, uint8_t count, int16_t val, uint8_t mode)

    * \param *table_x [in]             pointer x-array (1D)
    * \param *table_y [in]             pointer y-array (1D)
    * \param count [in]                counts of x-array elements
    * \param val [in]                  x-value
    * \param mode [in]                 LUT_MODE_EXTRAPOLATION (Standard): away from the array extrapolate
    *                                  LUT_MODE_LIMIT: away from the array not extrapolate
    *                                  LUT_MODE_KALIBRATION: y-value always >= 0, extrapolating upwards with difference (y_max - x_max)
    * \return int16_t                  y-value

    //--------------------------------------------------------------------------
    // for example

    int16_t arr_x[] = {136,200,311,444,666};
    int16_t arr_y[] = {-10,5,16,27,39};

    ret = os_util_lookup1D(arr_x, arr_y, 5, 250, LUT_MODE_EXTRAPOLATION);   // ret == 10
    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------
    int16_t os_util_lookup2D(int16_t *table_x, int16_t *table_y, uint8_t count_x, uint8_t count_y, int16_t *table_z, int16_t val_x, int16_t val_y)

    * \param *table_x [in]             pointer x-array (1D)
    * \param *table_y [in]             pointer y-array (1D)
    * \param count_x [in]              counts of x-array elements
    * \param count_y [in]              counts of y-array elements
    * \param *table_z [in]             pointer z-array (1D with (count_y * count_x) elements)
    * \param val_x [in]                x-value (away from the x-array => extrapolation)
    * \param val_y [in]                y-value (away from the y-array => extrapolation)
    * \return int16_t                  z-value

    //--------------------------------------------------------------------------
    // for example

    int16_t arr_x_2d[] = {10,20,30,40,50};
    int16_t arr_y_2d[] = {100,200,300};

    // y * x Matrix
    int16_t arr_z[] = {
        10,  20,  50, 90,150,
        25,  70, 150,250,400,
        50, 120, 240,400,860
    };

    ret = os_util_lookup2D(arr_x, arr_y, 5, 3, arr_z, 25, 270);   // ret == 159
    //--------------------------------------------------------------------------



    

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // USE LIN bus functionality
    //--------------------------------------------------------------------------
    // Actual LIN stack software version string in this macro --> MRS_LIN_STACK_SW_VERSION
    // In case of LIN communication the LIN transceiver EN Pin must be enabled on init (for example os_digout(DO_LIN_EN, 1);)
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    get the datapoint value from the LIN database
    * \details  
    *           
    * \pre      
    *           
    * \param    datapoint_id    [in] uint32_t   index of the LIN datapoint
    * \return   uint32_t                        the datapoint value
    //--------------------------------------------------------------------------
    uint32_t lin_db_get_value(uint32_t datapoint_id);
    
    value = lin_db_get_value(LIN0_RX_SIGNAL_NAME_15)
    
    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    set / write a datapoint value to the LIN database
    * \details  
    *           
    * \pre      
    *           
    * \param    datapoint_id    [in] uint32_t   index of the LIN datapoint
    * \param    set_value       [in] uint32_t   value to set / write
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_db_set_value(uint32_t datapoint_id, uint32_t set_value);
    
    lin_db_set_value(LIN0_TX_SIGNAL_NAME_1, 0x0F);
    
    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Check if a frame from the selected lin_module was received,\n
    *           without knowing about request or response frame on master or slave.
    * \details  
    *           
    * \pre      
    *           
    * \param    lin_module  [in] uint8_t    LIN index number --> enum_lin_bus_id
    * \param    frame_index [in] uint8_t    frame index number or name from enum_lin_frame_id
    * \param    reset       [in] uint8_t    TRUE (1) clear the flag to detect a new receive, FALSE (0) don't clear the flag to reset it manual
    * \return   uint8_t                     TRUE (1) received, FALSE (0) nothing received
    //--------------------------------------------------------------------------
    uint8_t lin_check_for_received_frame(uint8_t lin_module, uint8_t frame_index, uint8_t reset);

    if( lin_check_for_received_frame(LIN_BUS_0, LIN0_FRM_RX_INDEX_NAME_01, TRUE) == TRUE )
    {
        // frame was received
    }

    //--------------------------------------------------------------------------
     * \ingroup  lin
     * \brief    Check if a header form the selected lin_module was received.
     *
     * \details  After received a header you must clear the flag with the command
     *           lin_clear_response_frame_flag(frame_index);
     *           If you don�t clear the frame you will receive just one frame.
     * \pre
     *
     * \param    frame_index	[in] uint8_t	frame index number or name from enum_lin_frame_id
  	//--------------------------------------------------------------------------

	  if (lin_check_for_received_response_frame(uint8_t frame_index))
  	{
  	lin_clear_response_frame_flag( uint8_t frame_index );
  	}

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Clear the response frame flag to transmit the slave response data field.
    * \details  On Slave mode to transmit the slave response data field./n
    *           On Master mode to tell the stack that we have read all received data./n
    *           It's not functional necessary for Master mode.
    * \pre      
    *           
    * \param    frame_index [in] uint8_t frame index number or name from enum_lin_frame_id
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_clear_response_frame_flag(uint8_t frame_index);

    lin_clear_response_frame_flag(LIN0_FRM_RESP_INDEX_NAME_01);

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Clear the request frame flag to detect a new receive.
    * \details  Only on Slave mode to detect a master request frame correctly.
    *
    * \pre      
    *           
    * \param    frame_index [in] uint8_t frame index number or name from enum_lin_frame_id
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_clear_request_frame_flag(uint8_t frame_index);

    lin_clear_request_frame_flag(LIN0_FRM_REQ_INDEX_NAME_01);

    //--------------------------------------------------------------------------
    // LIN Master only functionality
    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Schedule table handling and bus idle check with automatic go to sleep command if bus was idle for 4s.
    * \details  --> lin_schedule_handling() must be called for each bus schedule table\n
    *           With lin_st_active parameter is it possible to hold and resume a schedule table.
    * \pre      
    *           
    * \param    lin_schedule_table_index [in] uint8_t
    * \param    lin_st_active            [in] uint8_t
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_schedule_handling(uint8_t lin_schedule_table_index, uint8_t lin_st_active);

    lin_schedule_handling(LIN0_SCHEDULE_TABLE_NR1, TRUE);

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    help function to reset a started schedule table, reset the schedule line index
    * \details  call this function only one time to reset the schedule line index,\n
    *           other you send only the first frame at the first schedule line
    * \pre      
    *           
    * \param    lin_schedule_table_index [in] uint8_t
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_schedule_handling_reset(uint8_t lin_schedule_table_index);

    lin_schedule_handling_reset(LIN0_SCHEDULE_TABLE_NR1);

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Check if response timeout was received, only on Master mode.
    * \details  
    *           
    * \pre      
    *           
    * \param    frame_index [in] uint8_t    frame index number or name from enum_lin_frame_id
    * \return   uint8_t                     TRUE (1) = Slave not responding (timeout), FALSE (0) = not timeout reached
    //--------------------------------------------------------------------------
    uint8_t lin_ma_check_frame_resp_timeout(uint8_t frame_index);

    if( lin_ma_check_frame_resp_timeout(LIN0_FRM_INDEX_NAME_01) == TRUE )
    {
        // timeout reached - Slave not responding
    }





    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // Code EXAMPLE:
    //  - Read analog pin ANA1 and send it on CAN bus on ID 0x18FA3000 byte 0+1
    //  - Switch the output OUT_HSD2 when bit8 of byte3 of 0x1FF004AB is "1"
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    variable1 = os_algin(ANA1);

    can_db_set_value(DATA2, variable1); // DATA must be defined in #can1_datenpunkt_db_const


    variable2 = (uint8_t)can_db_get_value(DATA3); // DATA3 must be defined in #can1_datenpunkt_db_const

    os_digout(OUT_HSD3, variable2);

    */

/// \ingroup user
/// \brief           Manually interpret the CAN messages
//--------------------------------------------------------------------------
///                  When a CAN message is received this function will be
///                  automatically called in main-loop, while reading from
///                  the buffered messages. This routine is not called in
///                  CAN-interrupt.
///
/// \param *msg      Pointer to the message with its struct bios_can_msg_typ
/// \return          None
//--------------------------------------------------------------------------
void user_can_message_receive(uint8_t hw_id, bios_can_msg_typ* msg)
{
    // --------------------------------------------------------------------------------
    // in the case of no using to get no compiler INFO set all as (void)xy;
    // --------------------------------------------------------------------------------
    (void)hw_id;
    (void)msg;

    /*
    // When 0x400 is received, send an acknowledge
    if( (msg->id == 0x400) && (msg->id_ext == 0) )
    {
        if( msg->data[0] == 0x33 )
        {
            os_can_send_msg(0, 0x720, 0, 8, 0x66, 0x33, 0x11, 0, 0, 0, 0, msg->data[7]);
        }
        else
        {
            // do nothing
        }
    }
    else
    {
        // do nothing
    }

    // Switch the output OUT_HSD2 when bit8 of byte3 of 0x1FF004AB is "1"
    if( (msg->id == 0x1FF004AB) && (msg->id_ext == 1) )
    {
        os_digout( OUT_HSD2, (msg->data[3] & 0x80) );
    }
    else
    {
        // do nothing
    }
    */

}


/// \ingroup user
/// \brief           Receive interrupt of serial communication interface (RS232)
//--------------------------------------------------------------------------
///                  When a message on SCI0 (RS232) is received, this function is
///                  called directly in the interrupt
///
/// \param data      The received byte
/// \return          None
//--------------------------------------------------------------------------
void user_int_rx_sci(uint8_t module, uint8_t data)
{
    // --------------------------------------------------------------------------------
    // in the case of no using to get no compiler INFO set all as (void)xy;
    // --------------------------------------------------------------------------------
    (void)module;
    (void)data;

}


/// \ingroup user
/// \brief           Transmit interrupt of serial communication interface (RS232)
//--------------------------------------------------------------------------
///                  When a message on SCI0 (RS232) is successful transmitted, this function is
///                  called directly in the interrupt
///
/// \return          None
//--------------------------------------------------------------------------
void user_int_tx_sci(uint8_t module)
{
    // --------------------------------------------------------------------------------
    // in the case of no using to get no compiler INFO set all as (void)xy;
    // --------------------------------------------------------------------------------
    (void)module;
}


/// \ingroup user
/// \brief           Timer PID0 interrupt every 1ms
//--------------------------------------------------------------------------
///                  Take care not to put much code in here
///
/// \return          None
//--------------------------------------------------------------------------
void user_int_timer_1ms(void)
{

}



/// \ingroup user
/// \brief           User-defined Timer PID2 interrupt
//--------------------------------------------------------------------------
///                  Take care not to put much code in here
///                  Activate it by setting of PID2_USER_DEFINED_CYCLE in user_code.h
///
/// \return          None
//--------------------------------------------------------------------------
void user_defined_int_timer(void)
{

}


/// \ingroup user
/// \brief           Interrupt of Enhanced Capture Timer (ECT)
//--------------------------------------------------------------------------
///                  If ECT is successful initialized to rising, falling or
///                  both edges, this routine is called in interrupt
///                  To initialize the function call bios_ect_init() in usercode_init(). 
///
/// \param ch        channel of ECT on which interrupt raised
/// \return          None
//--------------------------------------------------------------------------
void user_int_ect(uint8_t ch)
{
    // --------------------------------------------------------------------------------
    // in the case of no using to get no compiler INFO set all as (void)xy;
    // --------------------------------------------------------------------------------
    (void)ch;

/*
uint16_t counter_value;

    // read actual counter value from register (16bit value)
    switch( ch )
    {
        case 0: counter_value = ECT_TC0; break;
        case 1: counter_value = ECT_TC1; break;
        case 2: counter_value = ECT_TC2; break;
        case 3: counter_value = ECT_TC3; break;
        case 4: counter_value = ECT_TC4; break;
        case 5: counter_value = ECT_TC5; break;
        case 6: counter_value = ECT_TC6; break;
        case 7: counter_value = ECT_TC7; break;

        default: break;
    }

    // as wished, it is possible to change the edge mode
    //bios_ect_channels_init(ch, PWM_READ_FALLING);
*/

}

