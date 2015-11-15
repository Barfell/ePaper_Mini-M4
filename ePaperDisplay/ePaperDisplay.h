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

#define STAGE_TIME 2
#define REPEAT_CYCLE 2


typedef enum {
	OSCILLATOR_OFF = 0x01,
	HIGH_PWR = 0xD1
}eOSCILLATOR_MODE;

typedef enum {
	INVERSE,
	WHITE,
	BLACK,
	NEW,
	NOTHING,
	DUMMY
}eSTAGE;

typedef enum {
	POSITIVE_OFF = 0x00,
	NEGATIVE_OFF = 0x01,
	POSITIVE_ON = 0x01,
	VCOM_OFF = 0x03,
	NEGATIVE_ON = 0x03,
	VCOM_ON = 0x0F
}eVOLTAGE_SETTINGS;

void EPD_Init ( void );
void EPD_PowerOn ( void );
void EPD_PowerOff ( void );

uint16_t GetCOGid ( void );
void DisableOE ( void );
uint8_t CheckBreakage ( void );
void EnterPowerSavingMode ( void );
void ChannelSelect ( void );
void OscillatorPowerMode ( eOSCILLATOR_MODE mode );
void PowerSettings1 ( void );
void SetVComLevel ( void );
void PowerSettings2 ( void );
void PowerSettings3 ( void );
void DriverLatchOn ( void );
void DriverLatchOff ( void );

void ChargePump ( void );
void ChargePumpVoltageSettings( eVOLTAGE_SETTINGS settings );
uint8_t CheckDCDC ( void );
void OutputEnaToDisa ( void );
void TurnOnOE ( void );
void DischargeInternal( void );
void RefreshBorder ( uint8_t delay );

void DisplayImage ( uint8_t * image );
void DisplayInverse ( uint8_t * image );
void DisplayBW ( uint8_t * image );
void DisplayNew ( uint8_t * image );
void DisplayNothing ();
void DisplayLine ( uint8_t * image , uint8_t line , eSTAGE stage );

#endif // __E_PAPER_DISPLAY_H
