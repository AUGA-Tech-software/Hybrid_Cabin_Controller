//--------------------------------------------------------------------------
/// \file     modulhardwarecode.c
/// \brief    functions for modulhardware
/// \author   JHT
/// \date     01.12.15
/// \version  1.00
//--------------------------------------------------------------------------
#include "bios.h"
#include "os.h"

// --------------------------------------------------------------------------------
// ----------------------------------- defines ------------------------------------
// --------------------------------------------------------------------------------
#define PIN_SHIFT_DS                                  PORTC_PC5
#define PIN_SHIFT_SH_CP                               PORTC_PC4
#define SPI_NO_0                                      0
#define SPI_NO_1                                      1
#define ACTIVE_HIGH                                   1
#define M25P128_ID_MAX                                3
#define M25P128_CHIP_MAX                              3
#define T_MAX                                         4
#define T_MEAS_MAX                                    20
#define PRESS_BUF_SIZE                                20
#define PRESS_DIGITS_LIMIT_LOW                        455UL
#define PRESS_MBAR_MAX                                600UL
#define RPM_PRESCALER                                 4
#define K_TYPE_OC_VAL                                 9999
#define K_TYPE_OC_LIMIT                               1500
#define CAPTURE_CHANNEL_0                             0
#define CAPTURE_CHANNEL_6                             6


// --------------------------------------------------------------------------------
// ------------------------------- typedef structs --------------------------------
// --------------------------------------------------------------------------------
typedef struct
{
    uint8_t idx[T_MAX];
    int16_t T[T_MAX];
    int32_t U_diff[T_MAX];
    int32_t U_T[T_MAX];
    int32_t U_ref;

}k_type_def;


// --------------------------------------------------------------------------------
// ----------------------------------- globals ------------------------------------
// --------------------------------------------------------------------------------
static k_type_def k_type;

// --------------------------------------------------------------------------------
// ------------------------------- external globals -------------------------------
// --------------------------------------------------------------------------------
extern uint8 os_io_vals[];

// --------------------------------------------------------------------------------
// ---------------------------- functional prototypes -----------------------------
// --------------------------------------------------------------------------------
uint16_t get_curr_BTS(uint8_t channel, uint16_t gain);
uint16_t get_curr_INA194AID(uint8_t channel);
uint16_t get_press_val(void);
uint16_t get_CjC(void);
void get_k_type_temp (uint8_t ch, uint16_t* T);
uint16_t get_freq(void);
uint16_t get_rpm(uint16_t factor);
uint16_t get_freq_sinus_to_square_converter(void);


/*------------------------------------------------------------------------------
* function: uint16_t get_curr_BTS(uint8_t channel, uint16_t gain)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \test         STATUS: *** UNTESTED ***
* \brief       function to get load current of BTS high side driver types
*              BTS6143 & BTS650
*              regarding calculation refer to corresponding datasheets:
*              I_load = (U_sense[digits] * 5V * k_ilis) / (4095 * R_sense)
*              k_ilis(BTS650) = 14000
*              k_ilis(BTS6143) = 10000
*
*/
/*----------------------------------------------------------------------------*/
/**
* \param channel [in]           analogue input channel
* \return uint16_t              current of BTS  [mA]
*/
/*----------------------------------------------------------------------------*/
/**
* \date 10.02.2015      \author J. Harter
* \date 10.02.2015      \editor none
* \date 10.02.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
uint16_t get_curr_BTS(uint8_t channel, uint16_t gain)
{
    return(uint16_t)(((uint32_t)os_algin(channel) * gain) / 1000UL);
}

/*------------------------------------------------------------------------------
* Funktion: uint16_t get_curr_INA194AID(uint8_t channel)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \test         STATUS: *** UNTESTED ***
* \brief        function to get calculated current value
*               measured over INA194AID
*               the factor calculation for this chip is as follows:
*               Im = (5V / (4095 * Rm * A)) * U_digit
*               with Rm = 20mOhm, A = 50
*/
/*----------------------------------------------------------------------------*/
/**
* \param channel [in]           current analogue channel
* \return uint16_t              calculated curren [mA]
*/
/*----------------------------------------------------------------------------*/
/**
* \date 10.02.2015      \author J.Harter
* \date 10.02.2015      \editor none
* \date 10.02.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
uint16_t get_curr_INA194AID(uint8_t channel)
{
    // --------------------------------------------------------------------------------
    // --------------------------- calc current sense value ---------------------------
    // --------------------------------------------------------------------------------
    return ((uint16_t)((5UL * 1000UL * (uint32_t)os_algin(channel)) / ((4095UL * 20UL * 50UL)/1000UL)));
}

/*------------------------------------------------------------------------------
* function: uint16_t get_press_val(void)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \test         STATUS: *** UNTESTED ***
* \brief       function which caculates pressure value of Huba Control Typ
*              528
*              pressure range 0 - 600mbar
*              control range 0,5 - 4,5V
*/
/*----------------------------------------------------------------------------*/
/**
* \return uint16_t              pressure value
*/
/*----------------------------------------------------------------------------*/
/**
* \date 10.02.2015      \author J. Harter
* \date 10.02.2015      \editor none
* \date 10.02.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
uint16_t get_press_val(void)
{
    //local variables
    static int16_t press_buf[PRESS_BUF_SIZE];
    static uint8_t cnt = 0;
    static uint16_t res = 0;
    volatile uint16_t press_data = 0;
    volatile uint32_t press_data_sum = 0;
    volatile uint8_t i = 0;

    //get pressure row value
    press_data = os_algin_mv(AIM_PRESSURE);


    //under 0,5V = 455digits
    if (press_data <= PRESS_DIGITS_LIMIT_LOW)
    {
        //open circuit -> no value
        press_buf[cnt] = 0;
    }

    else
    {
        //take over
        press_buf[cnt] = press_data;
    }

    //increment
    cnt++;

    if (cnt >=  PRESS_BUF_SIZE)
    {
        //clear
        press_data_sum = 0;
        cnt = 0;

        //get sum of all saved data
        for (i = 0; i < PRESS_BUF_SIZE; i++)
        {
            //get sum
            press_data_sum += (uint32_t)press_buf[i];

        }

        if (press_data_sum != 0)
        {
            //calc average
            press_data_sum /= PRESS_BUF_SIZE;

            //calc digits -> mbar
            res = (uint16_t)((PRESS_MBAR_MAX * press_data_sum) / 4095UL);
        }

        else
        {
            res = 0;
        }
    }

    return(res);
}

/*------------------------------------------------------------------------------
* function: uint16_t get_CjC(void)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \test         STATUS: *** UNTESTED ***
* \brief        function to calc CJC with the following calculation
*               LM50CIM3 used:
*               V_out[mV] = 10 [mV]/[°C] * T_CjC[°C]) + 500mV
*               -> T_CjC[°C] = V_out[mV] - 500 [mV] / 10 [mV]/[°C]
*               with  V_out[mV] = V_out[digits] * 5000mV / 4095 digits
*
*               -> T_CjC[°C] = (((V_out [digits] * 5000 [mV]) / 4095 [digits]) - 500 [mV]) / 10 [mV] / [°C]
*/
/*----------------------------------------------------------------------------*/
/**
* \return uint16_t              CjC
*/
/*----------------------------------------------------------------------------*/
/**
* \date 10.02.2015      \author J. Harter
* \date 10.02.2015      \editor none
* \date 10.02.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
uint16_t get_CjC(void)
{
    return(uint16_t)(((((uint32_t)os_algin(AIM_CJC) * 5000UL) / 4095UL) - 500UL) / 10UL);

}

/*------------------------------------------------------------------------------
* function: void get_k_type_temp (uint8_t ch)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \test         STATUS: *** UNTESTED ***
* \brief        function to calculate k type temperature
*/
/*----------------------------------------------------------------------------*/
/**
* \param ch [in]                channel
*/
/*----------------------------------------------------------------------------*/
/**
* \date 03.03.2015      \author J. Harter
* \date 03.03.2015      \editor none
* \date 03.03.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
void get_k_type_temp (uint8_t ch, uint16_t* T)
{
    // //increment counter
    // k_type.idx[ch - AIM_T1]++;

    // //get U_T in mV
    // k_type.U_T[ch - AIM_T1] += os_algin_mv(ch);

    // //get U_ref dig -> mV
    // k_type.U_ref = ((os_algin(AI_T_REF) * 1000UL / 4095UL) * 5000UL) / 1000UL;

    // //20 measurements reached ?
    // if (k_type.idx[ch - AIM_T1] >= T_MEAS_MAX)
    // {
        // //calc average
        // k_type.U_T[ch - AIM_T1] /= T_MEAS_MAX;

        // //check if k type has open circuit
        // if (k_type.U_T[ch - AIM_T1] > K_TYPE_OC_LIMIT)
        // {
            // //calculate U_diff
            // if (k_type.U_T[ch - AIM_T1] >= k_type.U_ref)
            // {
                // //U_T has to be always higher than U_ref
                // k_type.U_diff[ch - AIM_T1] = k_type.U_T[ch - AIM_T1] - k_type.U_ref;
            // }
            // else
            // {
                // //otherwise U_diff is zero -> zero point adjustment is needed
                // k_type.U_diff[ch - AIM_T1] = 0;
            // }

            // //*** calculate temperature ***
            // //Udiff = U_T - U_ref = 40uV/K
            // //Ua = V * U_diff with V = (180K/1,37K + 1) = 132,38
            // //Ua = 132,38 * 40uV/K = 5,3mV/K
            // k_type.T[ch - AIM_T1] = (int16_t)(((k_type.U_diff[ch - AIM_T1] * 10000UL / 53) + (int32_t)(get_CjC() * 1000UL)) / 1000UL);
        // }

        // else
        // {
            // k_type.T[ch - AIM_T1] = K_TYPE_OC_VAL;
        // }

        // //take over
        // *T = k_type.T[ch - AIM_T1];

        // //reset values
        // k_type.idx[ch - AIM_T1] = 0;
        // k_type.U_diff[ch - AIM_T1] = 0;
        // k_type.U_T[ch - AIM_T1] = 0;
    // }
}

/*------------------------------------------------------------------------------
* function: void get_rpm(uint16_t factor)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \test         STATUS: *** UNTESTED ***
* \brief        function calculate rpm 1/min
*               rpm = f * 4(prescaler) = 1/s -> f * 60 * 4(prescaler) 1/min  * factor
*               factor: 123 -> 12,3!
*/
/*----------------------------------------------------------------------------*/
/**
* \date 10.02.2015      \author J. Harter
* \date 10.02.2015      \editor none
* \date 10.02.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
uint16_t get_rpm(uint16_t factor)
{
    //local variables
    uint32_t rpm = 0;

    //calc
    rpm = (uint32_t)get_freq() * 60UL * 10UL;

    //check factor
    if (factor > 0)
    {
        //scale down
        rpm = rpm /= factor;
    }

    else
    {
        rpm = 0;
    }

    return ((uint16_t) rpm);
}

/*------------------------------------------------------------------------------
* function: uint16_t get_freq(void)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \test         STATUS: *** UNTESTED ***
* \brief        function to get frequency of DI_RPM
*/
/*----------------------------------------------------------------------------*/
/**
* \return uint16_t      frequency value  [Hz]
*/
/*----------------------------------------------------------------------------*/
/**
* \date 10.02.2015      \author J. Harter
* \date 10.02.2015      \editor none
* \date 10.02.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
uint16_t get_freq(void)
{
    return ((uint16_t) os_pwm_duty_read[CAPTURE_CHANNEL_0].frequency * RPM_PRESCALER);
}

/*------------------------------------------------------------------------------
* function: uint16_t get_freq_sinus_to_square_converter(void)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \test         STATUS: *** UNTESTED ***
* \brief        function to convert sinus frequency to squarewave frequency
*/
/*----------------------------------------------------------------------------*/
/**
* \return uint16_t              frequency value of square wave  [Hz]
*/
/*----------------------------------------------------------------------------*/
/**
* \date 01.12.2015      \author J. Harter
* \date 01.12.2015      \editor none
* \date 01.12.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
uint16_t get_freq_sinus_to_square_converter(void)
{
    return ((uint16_t) os_pwm_duty_read[CAPTURE_CHANNEL_6].frequency);
}

/*------------------------------------------------------------------------------
* Funktion: void modulhardwarecode (void)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \test         STATUS: *** UNTESTED ***
* \brief        device specific functionality
*/
/*----------------------------------------------------------------------------*/
/**
* \date 10.02.2015      \author J. Harter
* \date 10.02.2015      \editor none
* \date 10.02.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
void modulhardwarecode (void)
{
    //local variables
    volatile uint8 i;
    volatile uint8 curr;

    os_pin_write(DO_SHIFT_MR_N, 1);

    bios_disable_interrupts();

    for (i = DOM_DUMMY_1; i >= DOM_IO1; i--)
    {
        PIN_SHIFT_SH_CP = 0;    // SCL to low

        curr = ((os_io_vals[i>>3] & bios_bitmask[i&7]) != 0);

        PIN_SHIFT_DS = curr; // SDA high or low

        PIN_SHIFT_SH_CP = 1;    // SCL to high again
    }

    PIN_SHIFT_SH_CP = 0;

    os_pin_write(DO_SHIFT_ST_CP, 1);
    os_pin_write(DO_SHIFT_ST_CP, 0);

    os_pin_write(DO_SHIFT_OE_N, 0);

    bios_enable_interrupts();
}


/*------------------------------------------------------------------------------
* Funktion: void modulhardwarecode_adc_multiplex(uint8 multiplex_nr)
*-----------------------------------------------------------------------------*/
/**
* \ingroup
* \brief       device specific function for multiplexing actions
*              (defined within os_io_tables.c file)
*/
/*----------------------------------------------------------------------------*/
/**
* \param multiplex_nr [in]      mux nr
*/
/*----------------------------------------------------------------------------*/
/**
* \date 01.12.2015      \author J. Harter
* \date 01.12.2015      \editor none
* \date 01.12.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
void modulhardwarecode_adc_multiplex(uint8 multiplex_nr)
{
    //multiplex all analogue inputs
    os_pin_write(CS_A_AI, os_adc_vals.multiplex & 0x01);
    os_pin_write(CS_B_AI, os_adc_vals.multiplex & 0x02);
    os_pin_write(CS_C_AI, os_adc_vals.multiplex & 0x04);

    //multiplex all temperature sensors
    os_pin_write(CS_A_TEMP, os_adc_vals.multiplex & 0x01);
    os_pin_write(CS_B_TEMP, os_adc_vals.multiplex & 0x02);
    os_pin_write(CS_C_TEMP, os_adc_vals.multiplex & 0x04);

    //multiplex all IO´s
    os_pin_write(CS_A_IO, os_adc_vals.multiplex & 0x01);
    os_pin_write(CS_B_IO, os_adc_vals.multiplex & 0x02);
    os_pin_write(CS_C_IO, os_adc_vals.multiplex & 0x04);

    //digital values mux
    switch (os_adc_vals.multiplex)
    {
        case 0:
				os_digout(D_AIR_CONDITIONER_SW, os_pin_read(DI_DIX_IO_1_8));
                os_digout(DIM_I2, os_pin_read(DI_DIX_I_1_8 ));
                break;

        case 1:
                os_digout(D_JOYST_FW_SW, os_pin_read(DI_DIX_IO_1_8));
                os_digout(DIM_I3, os_pin_read(DI_DIX_I_1_8 ));
                break;
        case 2:
                os_digout(D_BRAKE_SW, os_pin_read(DI_DIX_IO_1_8));
                os_digout(DIM_I4, os_pin_read(DI_DIX_I_1_8 ));
                break;
        case 3:
                os_digout(D_JOYST_RW_SW, os_pin_read(DI_DIX_IO_1_8));
                os_digout(DIM_I1, os_pin_read(DI_DIX_I_1_8 ));
                break;
        case 4:
                os_digout(D_SEAT_SW, os_pin_read(DI_DIX_IO_1_8));
                os_digout(DIM_I6, os_pin_read(DI_DIX_I_1_8 ));
                break;
        case 5:
                os_digout(D_ROADTRAV_SW2, os_pin_read(DI_DIX_IO_1_8));
                os_digout(DIM_I7, os_pin_read(DI_DIX_I_1_8 ));
                break;
        case 6:
                os_digout(D_JOYST_N_SW, os_pin_read(DI_DIX_IO_1_8));
                os_digout(DIM_I5, os_pin_read(DI_DIX_I_1_8 ));
                break;
        case 7:
                os_digout(D_ROADTRAV_SW1, os_pin_read(DI_DIX_IO_1_8));
                os_digout(DIM_I8, os_pin_read(DI_DIX_I_1_8 ));
                break;

        default:
            //do nothing
                break;
    }

    //HSD curr mux
    os_pin_write(DO_CS_DIS0, (os_adc_vals.multiplex >> 2) != 0);
    os_pin_write(DO_CS_DIS3, (os_adc_vals.multiplex >> 2) != 1);
    os_pin_write(DO_CS_DIS1, (os_adc_vals.multiplex >> 2) != 2);
    os_pin_write(DO_CS_DIS2, (os_adc_vals.multiplex >> 2) != 3);

    os_pin_write(CS_A_OUT, os_adc_vals.multiplex & 0x01);
    os_pin_write(CS_B_OUT, os_adc_vals.multiplex & 0x02);
    os_pin_write(CS_C_OUT, 0);
}


void modulhardwarecode_timer(void)
{
    // empty
}


/*------------------------------------------------------------------------------
* function: void modulhardwarecode_sleep(void)
*-----------------------------------------------------------------------------*/
/** 
* \ingroup      
* \test         STATUS: *** UNTESTED ***
* \brief        sleep function
*/
/*----------------------------------------------------------------------------*/
/** 
* \date 01.12.2015      \author J. Harter
* \date 01.12.2015      \editor none
* \date 01.12.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
void modulhardwarecode_sleep(void) 
{       
    DisableInterrupts;
    
    os_pin_write(CAN_STB0_N, 0);
    os_pin_write(CAN_STB1_N, 0); 
    os_pin_write(CAN_STB2_N, 0);
                
    (void)Start_wait(30); // 1ms warten
    
    os_pin_write(CAN_EN0, 0); 
    os_pin_write(CAN_EN1, 0); 
    os_pin_write(CAN_EN2, 0);       
    
    os_pin_write(DO_POWER, 0);
    
    (void)Start_wait(70000); 

    // Notstrategie
    os_pin_write(DO_POWER, 1);
    
    os_pin_write(CAN_STB0_N, 1);
    os_pin_write(CAN_STB1_N, 1); 
    os_pin_write(CAN_STB2_N, 1);
               
    os_pin_write(CAN_EN0, 1); 
    os_pin_write(CAN_EN1, 1); 
    os_pin_write(CAN_EN2, 1);           
    
    EnableInterrupts;
}


/*------------------------------------------------------------------------------
* Funktion: void modulhardwarecode_init(void)
*-----------------------------------------------------------------------------*/
/** 
* \ingroup      
* \test         STATUS: *** UNTESTED ***
* \brief        device specific init functions
*/
/*----------------------------------------------------------------------------*/
/** 
* \date 01.12.2015      \author J. Harter
* \date 01.12.2015      \editor none
* \date 01.12.2015      \tester none
*/
/*----------------------------------------------------------------------------*/
void modulhardwarecode_init(void) 
{    
    /* ECLKCTL: NECLK=1,NCLKX2=1,DIV16=0,EDIV4=0,EDIV3=0,EDIV2=0,EDIV1=0,EDIV0=0 */
    ECLKCTL = 0xC0;    
    
    //changes states initial
    os_pin_write(CAN_WAKE0, 1);
	  os_pin_write(CAN_WAKE0, 0);
	  os_pin_write(CAN_WAKE1, 1);
	  os_pin_write(CAN_WAKE1, 0);
    os_pin_write(CAN_WAKE2, 1);
	  os_pin_write(CAN_WAKE2, 0);

	//init sci
    bios_sci_module_init(4, BIOS_SCI_19200_BAUD, 0, BIOS_SCI_8N1);
   
}
