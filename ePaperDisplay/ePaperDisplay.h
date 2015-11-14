#ifndef __E_PAPER_DISPLAY_H
#define __E_PAPER_DISPLAY_H

#include "stm32f4xx.h"
#include "mini_m4.h"
#include "delay.h"
 /*
  * http://www.pervasivedisplays.com/_literature_138408/E-paper_Display_COG_Driver_Interface_Timing_V110
  */

#define GET_ID 0x71,0x00
#define CMD_HDR 0x70
#define DATA_W_HDR 0x72
#define DATA_R_HDR 0x73
#define START_FRAME 0x0A
#define ODD_MASK 0b10101010
#define EVEN_MASK 0b01010101
#define BW_MASK 0b10101010
#define INVERSE_MASK 0b01010101

void EPD_Init ( void );
void EPD_PowerOn ( void );
uint16_t GetCOGid ( void );
void DisableOE ( void );
uint8_t CheckBreakage ( void );
void EnterPowerSavingMode ( void );
void ChannelSelect ( void );
void HighPowerMode ( void );
void PowerSettings1 ( void );
void SetVComLevel ( void );
void PowerSettings2 ( void );
void DriverLatchOn ( void );
void DriverLatchOff ( void );
void ChargePump ( void );
uint8_t CheckDCDC ( void );
void OutputEnaToDisa ( void );
void TurnOnOE ( void );

typedef enum { INVERSE , WB , NEW }STAGE;

void DisplayImage ( uint8_t * image );
void DisplayInverse ( uint8_t * image );
void DisplayBW ( uint8_t * image );
void DisplayLine ( uint8_t * line , STAGE stage );

#endif // __E_PAPER_DISPLAY_H
