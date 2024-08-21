/*--------------------------------------------------------------------------*/
/** \file     can_db_tables.c
*   \brief    CAN bus database
*
*   \date     2024-08-21    \author   DS/
*
*   \platform HCS08DZ / HCS12XE / HCS12XD / HCS12P
* --------------------------------------------------------------------------*/


#include "can_db_tables.h" 
#include "can_db_tables_data.h"



/** \ingroup can_tables
*   \brief           Table of all CAN datapoints
* --------------------------------------------------------------------------*
*                    Every line defines a datapoint within a CAN block.
*                    Every datapoint is assigned to a CAN block ID
*                    #can_block_id, the order of the IDs has to be the same as
*                    the CAN block table #can_block_db_const.
*                    The table contains bit position, bit length and datatype.
*                    The datapoints are used with #can_db_set_value() or #can_db_get_value().
*                    As parameter use the name from the list #can_dp_id verwendet,
*                    the order has to be the same as in this table.
* --------------------------------------------------------------------------*/
 volatile const can_datenpunkt_db_const_typ can_datenpunkt_db_const[CAN_DP_MAX+1] = { // Array can1_....  with the dp of one CAN interface
 //CAN block index        Pos bit0             Length,     Data type   Data format           LineNr: datapoint-ID  
 //-----------------------------------------------------------------------------------------------------------------
 //(16 Bit)              (0-63)                (1-32)                  (0=Intel, 1=Motorola)
 //-----------------------------------------------------------------------------------------------------------------
{  CCVS                  ,8                    ,16         ,UINT       ,0          },  //Wheel_Based_Vehicle_Speed
{  EAMS1                 ,0                    ,2          ,UBYTE      ,0          },  //EA_Motor_EN_Status    
{  EAMS1                 ,2                    ,4          ,UBYTE      ,0          },  //EA_Motor_Controller_Status_Reason
{  EAMS1                 ,6                    ,2          ,UBYTE      ,0          },  //EA_Motor_Controller_COM
{  EAMS1                 ,8                    ,16         ,UINT       ,0          },  //EA_Motor_Speed        
{  EAMS1                 ,24                   ,16         ,UINT       ,0          },  //EA_Motor_Medium_Temperature
{  EAMS1                 ,40                   ,16         ,UINT       ,0          },  //EA_Motor_Power        
{  EAMS1                 ,56                   ,2          ,UBYTE      ,0          },  //EA_Motor_Service_Indicator
{  EAMS1                 ,58                   ,2          ,UBYTE      ,0          },  //EA_Motor_Operating_Status
{  EAMS1                 ,60                   ,4          ,UBYTE      ,0          },  //EA_Motor_Instance_of_Status_Data
{  ETC2_F                ,24                   ,8          ,UBYTE      ,0          },  //GBF_CURRENT_GEAR      
{  ETC2_R                ,24                   ,8          ,UBYTE      ,0          },  //GBR_CURRENT_GEAR      
{  LEVER_BACK            ,8                    ,4          ,UBYTE      ,0          },  //LVR_BCK_UP_CNT        
{  LEVER_BACK            ,12                   ,1          ,UBYTE      ,0          },  //LVR_BCK_UP            
{  LEVER_BACK            ,24                   ,4          ,UBYTE      ,0          },  //LVR_BCK_DOWN_CNT      
{  LEVER_BACK            ,28                   ,1          ,UBYTE      ,0          },  //LVR_BCK_DOWN          
{  LEVER_BRLR            ,8                    ,4          ,UBYTE      ,0          },  //LVR_BR_LEFT_CNT       
{  LEVER_BRLR            ,12                   ,1          ,UBYTE      ,0          },  //LVR_BR_LEFT_1         
{  LEVER_BRLR            ,24                   ,4          ,UBYTE      ,0          },  //LVR_BR_RIGHT_CNT      
{  LEVER_BRLR            ,28                   ,1          ,UBYTE      ,0          },  //LVR_BR_RIGHT_1        
{  LEVER_BRUD            ,8                    ,4          ,UBYTE      ,0          },  //LVR_BR_DOWN_CNT       
{  LEVER_BRUD            ,12                   ,1          ,UBYTE      ,0          },  //LVR_BR_DOWN_1         
{  LEVER_BRUD            ,13                   ,1          ,UBYTE      ,0          },  //LVR_BR_DOWN_2         
{  LEVER_BRUD            ,24                   ,4          ,UBYTE      ,0          },  //LVR_BR_UP_CNT         
{  LEVER_BRUD            ,28                   ,1          ,UBYTE      ,0          },  //LVR_BR_UP_1           
{  LEVER_BRUD            ,29                   ,1          ,UBYTE      ,0          },  //LVR_BR_UP_2           
{  LEVER_BTNA            ,8                    ,4          ,UBYTE      ,0          },  //LVR_BTNA_CNT          
{  LEVER_BTNA            ,12                   ,1          ,UBYTE      ,0          },  //LVR_BTNA_1            
{  LEVER_BTNBC           ,8                    ,4          ,UBYTE      ,0          },  //LVR_BTNB_CNT          
{  LEVER_BTNBC           ,12                   ,1          ,UBYTE      ,0          },  //LVR_BTNB_1            
{  LEVER_BTNBC           ,24                   ,4          ,UBYTE      ,0          },  //LVR_BTNC_CNT          
{  LEVER_BTNBC           ,28                   ,1          ,UBYTE      ,0          },  //LVR_BTNC_1            
{  LEVER_BTNDE           ,8                    ,4          ,UBYTE      ,0          },  //LVR_BTNE_CNT          
{  LEVER_BTNDE           ,12                   ,1          ,UBYTE      ,0          },  //LVR_BTNE_1            
{  LEVER_BTNDE           ,24                   ,4          ,UBYTE      ,0          },  //LVR_BTND_CNT          
{  LEVER_BTNDE           ,28                   ,1          ,UBYTE      ,0          },  //LVR_BTND_1            
{  LEVER_SRLR            ,8                    ,4          ,UBYTE      ,0          },  //LVR_SR_LEFT_CNT       
{  LEVER_SRLR            ,12                   ,1          ,UBYTE      ,0          },  //LVR_SR_LEFT_1         
{  LEVER_SRLR            ,13                   ,1          ,UBYTE      ,0          },  //LVR_SR_LEFT_2         
{  LEVER_SRLR            ,24                   ,4          ,UBYTE      ,0          },  //LVR_SR_RIGHT_CNT      
{  LEVER_SRLR            ,28                   ,1          ,UBYTE      ,0          },  //LVR_SR_RIGHT_1        
{  LEVER_SRLR            ,29                   ,1          ,UBYTE      ,0          },  //LVR_SR_RIGHT_2        
{  LEVER_SRUD            ,8                    ,4          ,UBYTE      ,0          },  //LVR_SR_UP_CNT         
{  LEVER_SRUD            ,12                   ,1          ,UBYTE      ,0          },  //LVR_SR_UP_1           
{  LEVER_SRUD            ,24                   ,4          ,UBYTE      ,0          },  //LVR_SR_DOWN_CNT       
{  LEVER_SRUD            ,28                   ,1          ,UBYTE      ,0          },  //LVR_SR_DOWN_1         
{  PNL_4E                ,8                    ,8          ,UBYTE      ,0          },  //KNOB_5_ROT            
{  PNL_4E                ,24                   ,4          ,UBYTE      ,0          },  //KNOB_6_CNT            
{  PNL_4E                ,28                   ,4          ,UBYTE      ,0          },  //KNOB_6_PRS            
{  PNL_4F                ,8                    ,4          ,UBYTE      ,0          },  //BTN_7_CNT             
{  PNL_4F                ,12                   ,1          ,UBYTE      ,0          },  //BTN_7_PRS             
{  PNL_4F                ,24                   ,4          ,UBYTE      ,0          },  //BTN_2_CNT             
{  PNL_4F                ,28                   ,1          ,UBYTE      ,0          },  //BTN_2_PRS             
{  PNL_50                ,8                    ,4          ,UBYTE      ,0          },  //BTN_4_CNT             
{  PNL_50                ,12                   ,1          ,UBYTE      ,0          },  //BTN_4_PRS             
{  PNL_53                ,8                    ,8          ,UBYTE      ,0          },  //KNOB_3_CNT            
{  PNL_53                ,24                   ,4          ,UBYTE      ,0          },  //KNOB_1_CNT            
{  PNL_53                ,28                   ,1          ,UBYTE      ,0          },  //KNOB_1_PRS            
{  PNL_B1                ,8                    ,8          ,UBYTE      ,0          },  //KNOB_6_ROT            
{  PNL_B2                ,8                    ,8          ,UBYTE      ,0          },  //KNOB_1_ROT            
{  PROPB_02              ,0                    ,4          ,UBYTE      ,0          },  //JOYSTICK_SETUP_1      
{  PROPB_02              ,4                    ,4          ,UBYTE      ,0          },  //JOYSTICK_SETUP_2      
{  PROPB_02              ,8                    ,4          ,UBYTE      ,0          },  //JOYSTICK_SETUP_3      
{  PROPB_02              ,12                   ,4          ,UBYTE      ,0          },  //JOYSTICK_SETUP_4      
{  PROPB_02              ,16                   ,4          ,UBYTE      ,0          },  //JOYSTICK_SETUP_5      
{  PROPB_02              ,20                   ,4          ,UBYTE      ,0          },  //JOYSTICK_SETUP_6      
{  PROPB_02              ,24                   ,4          ,UBYTE      ,0          },  //JOYSTIC_AUX1_SETSPEED_Up
{  PROPB_02              ,28                   ,4          ,UBYTE      ,0          },  //JOYSTIC_AUX1_SETSPEED_Down
{  PROPB_02              ,32                   ,4          ,UBYTE      ,0          },  //JOYSTIC_AUX2_SETSPEED_Up
{  PROPB_02              ,36                   ,4          ,UBYTE      ,0          },  //JOYSTIC_AUX2_SETSPEED_Down
{  PROPB_02              ,40                   ,4          ,UBYTE      ,0          },  //JOYSTIC_AUX3_SETSPEED_Up
{  PROPB_02              ,44                   ,4          ,UBYTE      ,0          },  //JOYSTIC_AUX3_SETSPEED_Down
{  PROPB_02              ,48                   ,4          ,UBYTE      ,0          },  //JOYSTIC_AUX4_SETSPEED_Up
{  PROPB_02              ,52                   ,4          ,UBYTE      ,0          },  //JOYSTIC_AUX4_SETSPEED_Down
{  PVED_RST_RQST         ,63                   ,1          ,UBYTE      ,0          },  //PVED_Reset_request    
{  PVED_STATUS           ,0                    ,8          ,UBYTE      ,0          },  //PVED_CLS_Current_Op_State
};



/** \ingroup can_tables
*   \brief           Table of all CAN blocks
* --------------------------------------------------------------------------*
*                    Every line defines a CAN block and sets the CAN-ID,
*                    the minimal and maximal time between to messages for sending,
*                    the length of a message and the direction (0=Rx, 1=Tx).
*                    The CAN-IDs must be defined in can_db.h.
*                    Every CAN block is assigned to a ID, the order has to be
*                    the same as in this table.
* --------------------------------------------------------------------------*/
 volatile const can_block_db_const_typ can_block_db_const[CAN_BLOCK_MAX+1] = { // Array can1_....  with the CAN blocks of one CAN interface
 //CAN bus index         CAN-ID                EXT-ID                Max,                  Min                   Msg-Len DLC           TX Flag               CAN bus gw            CAN-ID-Mask           Mux start             Mux length            Mux val               
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 //(8 Bit)               (32 Bit)              (0,1)                 (32 Bit)              (32 Bit)              (0-8)                 (0,1)                 (NONE,CAN_BUS_x)      (29bit)               0-62                  0-8                   0-255                 
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
{  CAN_BUS_0             ,0x18F00503           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //ETC2_R                
{  CAN_BUS_0             ,0x18F00504           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //ETC2_F                
{  CAN_BUS_1             ,0x10314D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //LEVER_BRUD            
{  CAN_BUS_1             ,0x10584D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //LEVER_BRLR            
{  CAN_BUS_1             ,0x10594D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //LEVER_SRUD            
{  CAN_BUS_1             ,0x105A4D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //LEVER_SRLR            
{  CAN_BUS_1             ,0x10374D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //LEVER_BTNA            
{  CAN_BUS_1             ,0x105F4D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //LEVER_BTNBC           
{  CAN_BUS_1             ,0x105C4D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //LEVER_BTNDE           
{  CAN_BUS_0             ,0x18FF0228           ,1                    ,1000                 ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //PROPB_02              
{  CAN_BUS_1             ,0x105B4D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //LEVER_BACK            
{  CAN_BUS_1             ,0x104E4D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //PNL_4E                
{  CAN_BUS_1             ,0x104F4D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //PNL_4F                
{  CAN_BUS_1             ,0x10504D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //PNL_50                
{  CAN_BUS_1             ,0x10534D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //PNL_53                
{  CAN_BUS_1             ,0x10B14D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //PNL_B1                
{  CAN_BUS_1             ,0x10B24D4C           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //PNL_B2                
{  CAN_BUS_0             ,0x18FEF15A           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //CCVS                  
{  CAN_BUS_2             ,0x18FF2013           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //PVED_STATUS           
{  CAN_BUS_0             ,0x0CFFDF28           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //PVED_RST_RQST         
{  CAN_BUS_0             ,0x18FC012E           ,1                    ,100                  ,50                   ,8                    ,0                    ,NONE                 ,0                    ,0                    ,0                    ,0                    } ,  //EAMS1                 
};


 volatile const can_bus_db_const_typ can_bus_db_const[CAN_BUS_MAX+1] = 
{
 //                                                                             Manual setting of segment / jumper-with register
 //HW CAN-Modul          HW CAN-Modul active   CAN Baudrate          Gateway Input         {   Name Str.         SJ                    Prescaler             T_SEG1                T_SEG2                CLK_SRC   }           
 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 //(8 Bit)               (8 Bit)               (8 Bit)               (8 Bit)               {   (10 Byte)         (8 Bit)               (8 Bit)               (8 Bit)               (8 Bit)               (8 Bit)               
 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
{  CAN_BUS_0             ,TRUE                 ,BIOS_CAN_EXT_BAUDRATE,IS_NO_GW_INPUT       ,{   "250 kBit"       ,2                    ,2                    ,13                   ,2                    ,0  }  },             
{  CAN_BUS_1             ,TRUE                 ,BIOS_CAN_EXT_BAUDRATE,IS_NO_GW_INPUT       ,{   "250 kBit"       ,2                    ,2                    ,13                   ,2                    ,0  }  },             
{  CAN_BUS_2             ,TRUE                 ,BIOS_CAN_EXT_BAUDRATE,IS_NO_GW_INPUT       ,{   "250 kBit"       ,2                    ,2                    ,13                   ,2                    ,0  }  },             
{  CAN_BUS_3             ,FALSE                ,BIOS_CAN_EXT_BAUDRATE,IS_NO_GW_INPUT       ,{   "125 kBit"       ,2                    ,4                    ,13                   ,2                    ,0  }  },             
{  CAN_BUS_4             ,FALSE                ,BIOS_CAN_EXT_BAUDRATE,IS_NO_GW_INPUT       ,{   "125 kBit"       ,2                    ,4                    ,13                   ,2                    ,0  }  },             
};



 volatile const can_gateway_db_const_typ can_gateway_db_const[CAN_GATEWAY_DB_MAX+1]; 




