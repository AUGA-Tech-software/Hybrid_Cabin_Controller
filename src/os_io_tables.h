#ifndef _OS_IO_TABLES_H_
#define _OS_IO_TABLES_H_


/*--------------------------------------------------------------------------*/
/** \file     os_io_tables.h
*   \brief    Tables for definitions of I/O ports and their properties
*
*   \date     2024-08-21    \author   DS/
*
*   \platform HCS08DZ / HCS12XE / HCS12XD / HCS12P
* --------------------------------------------------------------------------
*   \defgroup Name      io
* --------------------------------------------------------------------------*/


#include "bios_base.h"
#include "os_io.h"

#define OSC_CLK_4MHZ 0
#define OSC_CLK_8MHZ 1


/** \ingroup io
*   \brief           List of used pin names
* --------------------------------------------------------------------------*
*                    All the used pin names are inserted in this table.
*                    IMPORTANT: The order of the pins has to be the same as
*                    in #os_port_pin!
* --------------------------------------------------------------------------*/
enum {
DI_CAN_ERR0_N         =0,
DI_CAN_ERR1_N         ,
DI_CAN_ERR2_N         ,
DI_KL15               ,
DI_KL15_GSM           ,
DI_RPM                ,
DI_RPM_2              ,
D_JOYST_N_SW          ,
D_SEAT_SW             ,
D_ROADTRAV_SW1        ,
D_ROADTRAV_SW2        ,
D_AIR_CONDITIONER_SW  ,
D_JOYST_RW_SW         ,
D_JOYST_FW_SW         ,
D_BRAKE_SW            ,
DIM_I1                ,
DIM_I2                ,
DIM_I3                ,
DIM_I4                ,
DIM_I5                ,
DIM_I6                ,
DIM_I7                ,
DIM_I8                ,
DI_DIX_IO_1_8         ,
DI_DIX_I_1_8          ,
OS_PORT_INPUT_PIN_MAX ,
CAN_EN0               ,
CAN_EN1               ,
CAN_EN2               ,
CAN_STB0_N            ,
CAN_STB1_N            ,
CAN_STB2_N            ,
CAN_WAKE0             ,
CAN_WAKE1             ,
CAN_WAKE2             ,
CS_A_AI               ,
CS_B_AI               ,
CS_C_AI               ,
CS_A_IO               ,
CS_B_IO               ,
CS_C_IO               ,
CS_A_OUT              ,
CS_B_OUT              ,
CS_C_OUT              ,
CS_A_TEMP             ,
CS_B_TEMP             ,
CS_C_TEMP             ,
CS_SPI_0_N            ,
CS_SPI_1_N            ,
CS_SPI_2_N            ,
CS_SPI_3_N            ,
CS_SPI_RST_N          ,
DO_CS_DIS0            ,
DO_CS_DIS1            ,
DO_CS_DIS2            ,
DO_CS_DIS3            ,
DO_CS_DE_RS485        ,
DO_NOX                ,
DO_POWER              ,
DO_PWM_O5             ,
DO_PWM_O6             ,
DO_PWM_O7             ,
DO_PWM_O8             ,
DO_SHIFT_MR_N         ,
DO_SHIFT_OE_N         ,
DO_SHIFT_SH_CP        ,
DO_SHIFT_ST_CP        ,
DO_SPI_HOLD_N         ,
DO_SPI_WP_N           ,
DOM_IO1               ,
DOM_IO2               ,
DOM_IO3               ,
DOM_IO4               ,
DOM_IO5               ,
DOM_IO6               ,
DOM_IO7               ,
DOM_IO8               ,
DOM_POWER_SENSOR_5V   ,
DOM_PANEL_ON          ,
DOM_OUT_CB_2          ,
DOM_OUT_CB_1          ,
DOM_O4                ,
D_REVERSE_BUZZER      ,
D_REVERSE_LIGHT       ,
DO_IGN_RELAY          ,
DOM_DUMMY_1           ,
DOX_SR                ,
WD                    ,
OS_PORT_PIN_MAX       
};




typedef struct {
  uint16_t name;
  uint16_t port;
  uint8_t bit;
  uint8_t direction;
  uint8_t pullup;
  uint8_t pull_enable;
  uint8_t int_edge;
  uint8_t int_enable;
  uint8_t value;
  uint8_t hi_entprell_cnt;
  uint8_t lo_entprell_cnt;
  uint8_t multiplex;
  uint8_t inverted;
}os_port_pin_def;

extern const os_port_pin_def os_port_pin[];




/** \ingroup io
*   \brief           List of analog pins
* --------------------------------------------------------------------------*
*                    All the used analog pins are inserted in this table.
*                    IMPORTANT: The order of the pins has to be the same as
*                    in #os_analog_pin!
* --------------------------------------------------------------------------*/
enum {
AI_CUR_CB_1           =0,
AI_CUR_CB_2           ,
AI_NOX                ,
AI_OUT_CB_1           ,
AI_OUT_CB_2           ,
AI_SENS_PWM_O5        ,
AI_SENS_PWM_O6        ,
AI_SENS_PWM_O7        ,
AI_SENS_PWM_O8        ,
AI_T_REF              ,
AI_VCC_PANEL          ,
A_JOYST_POS1          ,
A_JOYST_POS2          ,
A_PARKING_BRAKE_SW    ,
A_AUX1_SIGN2          ,
AIM_CJC               ,
AIM_DUMMY_1           ,
AIM_DUMMY_2           ,
AIM_DUMMY_3           ,
AIM_KL30_2            ,
AIM_KL30_1            ,
AIM_DUMMY_4           ,
AIM_SENSOR_5V         ,
AIM_PRESSURE          ,
A_CNG_PRESSURE        ,
A_ENGINE_COOLANT      ,
AIM_DUMMY_5           ,
AIM_PT200_1P          ,
AIM_PT200_2P          ,
AIM_PT200_3P          ,
AIM_PT200_4P          ,
AIM_DUMMY_6           ,
AIM_DUMMY_7           ,
AIM_DUMMY_8           ,
AIM_DUMMY_9           ,
AIM_IO5               ,
AIM_IO7               ,
AIM_IO8               ,
AIM_IO6               ,
AIM_IO2               ,
AIM_IO4               ,
AIM_IO1               ,
AIM_IO3               ,
A_AUX3_SIGN2          ,
A_AUX2_SIGN2          ,
A_AUX1_SIGN1          ,
A_AUX4_SIGN2          ,
A_AUX4_SIGN1          ,
A_AUX3_SIGN1          ,
A_TURTLE_RABBIT       ,
A_AUX2_SIGN1          ,
AIM_CUR_IO1           ,
AIM_CUR_IO2           ,
AIM_CUR_IO3           ,
AIM_CUR_IO4           ,
AIM_CUR_IO5           ,
AIM_CUR_IO6           ,
AIM_CUR_IO7           ,
AIM_CUR_IO8           ,
AIM_CUR_O1            ,
AIM_CUR_O2            ,
AIM_CUR_O3            ,
AIM_CUR_O4            ,
AIM_CUR_O5            ,
AIM_CUR_O6            ,
AIM_CUR_O7            ,
AIM_CUR_O8            ,
AIX_AI                ,
AIX_I_1_8             ,
AIX_IO_1_8            ,
AIX_KTYPE             ,
AIX_OUT               ,
AIX_PT200             ,
OS_ANALOG_PIN_MAX     
};




typedef struct {
  uint16_t name;
  uint8_t  adc_channel;
  uint8_t  direction;
  uint16_t value;
  uint16_t min;
  uint16_t max;
  uint8_t  error_filter;
  uint8_t  entprell_cnt;
  uint16_t adc_max;
  uint8_t  multiplex;
  uint8_t  multiplex_type;
  uint8_t  parameter1;   // e.g. HSD number to calculate calibration value of current flow
  uint8_t  samples;   
}os_analog_pin_def;

extern const os_analog_pin_def os_analog_pin[];


/** \ingroup io
*   \brief           List of PWM pins
* --------------------------------------------------------------------------*
*                    All the used PWM pins are inserted in this table.
*                    IMPORTANT: The order of the pins has to be the same as
*                    in #os_pwm_pin!
* --------------------------------------------------------------------------*/
enum {
PWM_O5                =0,
PWM_O6                ,
PWM_O7                ,
PWM_O8                ,
OS_PWM_PIN_MAX        
};



typedef struct {
  uint8_t  name;
  uint8_t  timer_channel;
  uint8_t  pulse_type;
  uint8_t  init_type;
  uint8_t  adc_active;
  uint8_t  adc_channel;
  uint16_t adc_max_volt;
  uint8_t  adc_filter;
  uint8_t  adc_startbed;
  uint8_t  digout_vorw;
  uint8_t  digout_rueckw;
  uint16_t current_max;
  uint8_t  digout_index;
}os_pwm_pin_def;

extern const os_pwm_pin_def os_pwm_pin[];



/** \ingroup io
*   \brief           List of EEPROM_INDEX
* --------------------------------------------------------------------------*
*                    Enum for EEPROM_INDEX instead of numbers
* --------------------------------------------------------------------------*/

// Nothing defined


/** \ingroup io
*   \brief           List of USERVAR_INDEX
* --------------------------------------------------------------------------*
*                    Enum for USERVAR_INDEX instead of numbers
* --------------------------------------------------------------------------*/

// Nothing defined


extern const uint8_t bios_osc_clk;

extern const uint8_t modul_geraetetyp_nr;

#define MACRO_MODUL_GERAETETYP_NR 251

#define HSDDATENSATZ_SIZE_DYNAMIC 15

//#define USER_VARIABLE_SIZE_DYNAMIC 10

#define IO_DEF_FILENAME "M3600_CAN_SPS"

#endif

