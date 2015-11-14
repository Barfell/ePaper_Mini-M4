#include "stm32f4xx.h"
#include "mini_m4.h"
#include "ePaperDisplay.h"
#include "delay.h"
#include "Hello_World.h"

int main(void)
{
	SysTick_Init();
	LEDs_Init();
	SPI1_Init();
	EPD_Init();


    while(1)
    {
    	DisplayImage( Hello_World_bits );
    	delay_nms(2000);
    }
}

void SysTick_Handler( void )
{
	// Called every microsecond
	TimeTick_Decrement();
}
