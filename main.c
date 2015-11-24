#include "stm32f4xx.h"
#include "mini_m4.h"
#include "ePaperDisplay.h"
#include "delay.h"
#include "Hello_World.h"

int main ( void )
{
	SysTick_Init();
	LEDs_Init();
	GPIOs_Init();
	SPI1_Init();

	EPD_Init();
	EPD_PowerOff();
	EPD_Init();
	DisplayImage( IMAGE );
	EPD_PowerOff();

    while(1)
    {
    	delay_nms( 2000 );
    }
}

void SysTick_Handler ( void )
{
	// Called every microsecond
	TimeTick_Decrement();
}
