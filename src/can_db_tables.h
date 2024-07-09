#ifndef _CAN_DB_TABLES_H_
#define _CAN_DB_TABLES_H_


/*--------------------------------------------------------------------------*/
/** \file     can_db_tables.h
*   \brief    CAN bus database
*
*   \date     2024-07-09    \author   DS/
*
*   \platform HCS08DZ / HCS12XE / HCS12XD / HCS12P
* --------------------------------------------------------------------------
*   \defgroup Name      can_tables
* --------------------------------------------------------------------------*/



// Change the Baudrate either to extended mode or a fix rate from the list below:
#define CAN_BAUDRATE BIOS_CAN_EXT_BAUDRATE
// einstellbare BAUDRATEN
// BIOS_CAN_EXT_BAUDRATE
// BIOS_CAN_1MBIT
// BIOS_CAN_800KBIT
// BIOS_CAN_500KBIT
// BIOS_CAN_250KBIT
// BIOS_CAN_125KBIT
// BIOS_CAN_100KBIT
// BIOS_CAN_83_33KBIT
// BIOS_CAN_50KBIT
// BIOS_CAN_33_33KBIT
// BIOS_CAN_20KBIT
// BIOS_CAN_10KBIT


//SETTING FOR BIOS_CAN_EXT_BAUDRATE 
#define  CAN_BAUDRATE_def_BEZEICHNUNG  250 kBit
#define  CAN_BAUDRATE_def_sj           2
#define  CAN_BAUDRATE_def_pre          2
#define  CAN_BAUDRATE_def_t_seg1       13
#define  CAN_BAUDRATE_def_t_seg2       2
#define  CAN_BAUDRATE_def_source       0



// Activate the user-defined filter settings:
#define USER_CAN_FILTER_ACTIVE

#ifdef USER_CAN_FILTER_ACTIVE

    #define CAN0_FILTER_A 0x18F00504
    #define CAN0_FILTER_B 0x18FF0228
    #define CAN0_MASK_A   0x00000007
    #define CAN0_MASK_B   0x00000000
    #define CAN0_MASK_DIVISION BIOS_CAN_ID_EXT_A_AND_B

    #define CAN1_FILTER_A 0x104E4D4C
    #define CAN1_FILTER_B 0x18FEF15A
    #define CAN1_MASK_A   0x00FF0000
    #define CAN1_MASK_B   0x00000000
    #define CAN1_MASK_DIVISION BIOS_CAN_ID_EXT_A_AND_B

    #define CAN2_FILTER_A 0x18FF205A
    #define CAN2_FILTER_B 0x18FF205A
    #define CAN2_MASK_A   0x00000000
    #define CAN2_MASK_B   0x00000000
    #define CAN2_MASK_DIVISION BIOS_CAN_ID_EXT_A_AND_B

    #define CAN3_FILTER_A 0x000
    #define CAN3_FILTER_B 0x000
    #define CAN3_MASK_A   0x000
    #define CAN3_MASK_B   0x000
    #define CAN3_MASK_DIVISION BIOS_CAN_ID_ALL_CLOSED

    #define CAN4_FILTER_A 0x000
    #define CAN4_FILTER_B 0x000
    #define CAN4_MASK_A   0x000
    #define CAN4_MASK_B   0x000
    #define CAN4_MASK_DIVISION BIOS_CAN_ID_ALL_CLOSED

#endif

// Datentypen
#define FLOAT   0x24
//#define W_FLOAT 0x24
#define ULONG   0x04
//#define W_ULONG 0x04
#define SLONG   0x14
#define UINT    0x02
//#define W_UINT  0x02
#define SINT    0x12
//#define M_SINT  0x12   // als MEMO-Feld in der Datenbank
//#define W_SINT  0x12
#define UBYTE   0x01
//#define W_UBYTE 0x01
#define SBYTE   0x11
//#define TEXT    0x01
//#define M_TEXT  0x01



#define IS_GW_INPUT      1
#define IS_NO_GW_INPUT   0


// Defines for initialization of CAN datapoints by EEPROM
#define CAN_NO_EEPROM_INIT 255
#define CAN_INIT_EEPROM_SIZE 20


typedef enum
{
    CAN_GATEWAY_DB_MAX    =   0
}can_gateway_id;

/// Define CAN busses: All the possible CAN busses. Only use these busses in table #can_block_db_const
typedef enum
{
    CAN_BUS_0 = 0,
    CAN_BUS_1    ,
    CAN_BUS_2    ,
    CAN_BUS_3    ,
    CAN_BUS_4    ,
    CAN_BUS_MAX
} can_bus_id;



/// Define datapoints: Order must be the same as in table#can_datenpunkt_db_const
typedef enum
{
    Wheel_Based_Vehicle_Speed  = 0,
    GBF_CURRENT_GEAR           ,
    GBR_CURRENT_GEAR           ,
    LVR_BCK_UP_CNT             ,
    LVR_BCK_UP                 ,
    LVR_BCK_DOWN_CNT           ,
    LVR_BCK_DOWN               ,
    LVR_BR_LEFT_CNT            ,
    LVR_BR_LEFT_1              ,
    LVR_BR_RIGHT_CNT           ,
    LVR_BR_RIGHT_1             ,
    LVR_BR_DOWN_CNT            ,
    LVR_BR_DOWN_1              ,
    LVR_BR_DOWN_2              ,
    LVR_BR_UP_CNT              ,
    LVR_BR_UP_1                ,
    LVR_BR_UP_2                ,
    LVR_BTNA_CNT               ,
    LVR_BTNA_1                 ,
    LVR_BTNB_CNT               ,
    LVR_BTNB_1                 ,
    LVR_BTNC_CNT               ,
    LVR_BTNC_1                 ,
    LVR_BTNE_CNT               ,
    LVR_BTNE_1                 ,
    LVR_BTND_CNT               ,
    LVR_BTND_1                 ,
    LVR_SR_LEFT_CNT            ,
    LVR_SR_LEFT_1              ,
    LVR_SR_LEFT_2              ,
    LVR_SR_RIGHT_CNT           ,
    LVR_SR_RIGHT_1             ,
    LVR_SR_RIGHT_2             ,
    LVR_SR_UP_CNT              ,
    LVR_SR_UP_1                ,
    LVR_SR_DOWN_CNT            ,
    LVR_SR_DOWN_1              ,
    KNOB_5_ROT                 ,
    KNOB_6_CNT                 ,
    KNOB_6_PRS                 ,
    BTN_7_CNT                  ,
    BTN_7_PRS                  ,
    BTN_2_CNT                  ,
    BTN_2_PRS                  ,
    BTN_4_CNT                  ,
    BTN_4_PRS                  ,
    KNOB_3_CNT                 ,
    KNOB_1_CNT                 ,
    KNOB_1_PRS                 ,
    KNOB_6_ROT                 ,
    KNOB_1_ROT                 ,
    JOYSTICK_SETUP_1           ,
    JOYSTICK_SETUP_2           ,
    JOYSTICK_SETUP_3           ,
    JOYSTICK_SETUP_4           ,
    JOYSTICK_SETUP_5           ,
    JOYSTICK_SETUP_6           ,
    JOYSTIC_AUX1_SETSPEED_Up   ,
    JOYSTIC_AUX1_SETSPEED_Down ,
    JOYSTIC_AUX2_SETSPEED_Up   ,
    JOYSTIC_AUX2_SETSPEED_Down ,
    JOYSTIC_AUX3_SETSPEED_Up   ,
    JOYSTIC_AUX3_SETSPEED_Down ,
    JOYSTIC_AUX4_SETSPEED_Up   ,
    JOYSTIC_AUX4_SETSPEED_Down ,
    PVED_CLS_Current_Op_State  ,
    CAN_DP_MAX            
}can_dp_id;




/// Define CAN blocks: Order must be the same as in table #can_block_db_const
typedef enum
{
    ETC2_R                  = 0,
    ETC2_F                  = 1,
    LEVER_BRUD              = 2,
    LEVER_BRLR              = 3,
    LEVER_SRUD              = 4,
    LEVER_SRLR              = 5,
    LEVER_BTNA              = 6,
    LEVER_BTNBC             = 7,
    LEVER_BTNDE             = 8,
    PROPB_02                = 9,
    LEVER_BACK              = 10,
    PNL_4E                  = 11,
    PNL_4F                  = 12,
    PNL_50                  = 13,
    PNL_53                  = 14,
    PNL_B1                  = 15,
    PNL_B2                  = 16,
    CCVS                    = 17,
    PVED_STATUS             = 18,
    CAN_BLOCK_MAX           = 19
} can_block_id;





/// Activate direct routing of CAN messages
//#define CAN_GW_MODE_ROUTE

#endif


