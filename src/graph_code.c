/*--------------------------------------------------------------------------*/
/** \file     graph_code.c.c
*   \brief    Generated code from FUP
*             Project name: M3600_CAN_SPS_D_CabinControllerM1.sch
*
*   \date     2/7/2023 11:50 AM    \author   dr/
*
*   \platform HCS08DZ / HCS12XE / HCS12XD
* --------------------------------------------------------------------------*/

#include "graph_includes.h"

// MEMBITARRAY_INDICES  = 4     // index 1 - 3
// MEM16ARRAY_INDICES   = 0     // index none
// MEM32ARRAY_INDICES   = 2     // index 4 - 5
// CONST32ARRAY_INDICES = 2     // index 6 - 7

uint8_t membitarray[MEMBITARRAY_SIZE];            // size: 1
uint16_t mem16array[MEM16ARRAY_SIZE];             // size: 1
GRAPH_LONG mem32array[MEM32ARRAY_SIZE];         // size: 2
uint8_t i2c_array[I2C_ARRAY_SIZE][I2C_BYTE_SIZE]; // size: 0
const GRAPH_LONG const32array[CONST32ARRAY_SIZE] = {10000,12000}; // size: 2

uint16_t graph_cycle_time = 10;

/* --------------------------------------------------------------------------
*  Function:			    void graphcode_init(void)
* -------------------------------------------------------------------------*/
/** \ingroup graph_func
*   \brief          		Generated graph-init-code from FUP
* --------------------------------------------------------------------------
*   \date     				2/7/2023 11:50 AM \author      dr/
* ------------------------------------------------------------------------*/
void graphcode_init(void)
{
}


/* --------------------------------------------------------------------------
*  Function:			    void graphcode(void)
* -------------------------------------------------------------------------*/
/** \ingroup graph_func
*   \brief          		Generated graph-code from FUP
* --------------------------------------------------------------------------
*   \date     				2/7/2023 11:50 AM \author      dr/
* ------------------------------------------------------------------------*/
void graphcode(void)
{

//  DIGIN1
//  DIGIN:1:Defined:1:Bit1A:1
//  1=N$1:1[0.1]
    DIGIN(DI_KL15, 1);

//  VALUE#1
//  VALUE:38:Konst:1:Bit32A:1
//  6=D$KONST(10000):C[0], 6=D$KONST(10000):C[0]
//  VALUE(6, 6);

//  VALUE#2
//  VALUE:38:Konst:1:Bit32A:1
//  7=D$KONST(12000):C[1], 7=D$KONST(12000):C[1]
//  VALUE(7, 7);

//  B1
//  DELAY_OFF_RTR:105:Merker32:1:Bit1E:1:Bit32E:1:Bit1A:1
//  4=D$Merker32:32[0], 1=N$1:1[0.1], 6=D$KONST(10000):C[0], 2=N$4:1[0.2]
    DELAY_OFF_RTR(4, 1, 6, 2);

//  B2
//  DELAY_OFF_RTR:105:Merker32:1:Bit1E:1:Bit32E:1:Bit1A:1
//  5=D$Merker32:32[1], 1=N$1:1[0.1], 7=D$KONST(12000):C[1], 3=N$5:1[0.3]
    DELAY_OFF_RTR(5, 1, 7, 3);

//  DIGOUT1
//  DIGOUT:224:Defined:1:Bit1E:1
//  2=N$4:1[0.2]
    DIGOUT(DO_IGN_RELAY, 2);

//  DIGOUT2
//  DIGOUT:224:Defined:1:Bit1E:1
//  3=N$5:1[0.3]
    DIGOUT(DO_POWER, 3);
}
