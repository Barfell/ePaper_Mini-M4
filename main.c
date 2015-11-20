#include "stm32f4xx.h"
#include "mini_m4.h"
#include "ePaperDisplay.h"
#include "delay.h"
#include "Hello_World.h"

int main ( void )
{
	SysTick_Init();
	LEDs_Init();
	ChangeLedState( ORANGE , Bit_SET );
	SPI1_Init();
	EPD_Init();
	ChangeLedState( ORANGE , Bit_RESET );

	ChangeLedState( RED , Bit_SET );
	DisplayImage( IMAGE );
	EPD_PowerOff();
	ChangeLedState( RED , Bit_RESET );

    while(1)
    {
    	ToggleLed( ORANGE );
    	delay_nms( 500 );
    }
}

void SysTick_Handler ( void )
{
	// Called every microsecond
	TimeTick_Decrement();
}
