#include "stm32f4xx.h"
#include "mini_m4.h"
#include "ePaperDisplay.h"
#include "delay.h"

int main(void)
{
	LEDs_Init();
	SPI1_Init();
	SysTick_Init();

	//ChangeLedState( ORANGE , Bit_SET );
	//ChangeLedState( RED , Bit_RESET );
	GPIO_WriteBit( GPIOC , GPIO_Pin_11 , Bit_RESET );

    while(1)
    {
    	//ToggleLed( ORANGE );
    	//ToggleLed( RED );
    	//GPIO_ToggleBits( GPIOC , GPIO_Pin_11 );
    	delay_1ms();
    }
}

void SysTick_Handler( void )
{
	// Called every microsecond
	TimeTick_Decrement();
}
