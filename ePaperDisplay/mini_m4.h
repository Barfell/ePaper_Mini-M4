#ifndef __MINI_M4_H
#define __MINI_M4_H

#include "stm32f4xx.h"

typedef enum
{
	ORANGE = 0x00,
	RED = 0x01
} LED;

void LEDs_Init ( void );
void ToggleLed ( LED led );
void ChangeLedState( LED led , BitAction state );
void GPIOs_Init( void );
void ChangePowerState( BitAction state );
void ChangeResetState( BitAction state );
void ChangeDischargeState( BitAction state );
void ChangeBorderState( BitAction state );
uint8_t IsBusy ( void );
void SPI1_Init ( void );
void SPI1_send(uint8_t * data);

#endif // __MINI_M4_H
