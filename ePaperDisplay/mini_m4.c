#include "mini_m4.h"

void LEDs_Init ( void )
{
	/* Onboard leds are connected to:
	 * ORANGE = PC12
	 * RED = PC13
	 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC , ENABLE );

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init( GPIOC , &GPIO_InitStruct );
}

void ToggleLed ( LED led )
{
	uint16_t pin = 0x01 << ( (uint8_t)led + 0x0C );

	GPIO_ToggleBits( GPIOC , pin );
}

void ChangeLedState( LED led , BitAction state )
{
	uint16_t pin = 0x01 << ( (uint8_t)led + 0x0C );

	if (state == Bit_SET)
		GPIO_SetBits( GPIOC , pin );
	else
		GPIO_ResetBits( GPIOC , pin );
}

void GPIOs_Init( void )
{
	/* Initialize misc GPIO for screen :
	 * BUSY   : B13 <- INPUT
	 * BORDER : B14
	 * POWER  : B1
	 * DSCHG  : C10
	 * RESET  : C11
	 */
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB , ENABLE );
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC , ENABLE );

	// POWER & BORDER
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_14 ;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init( GPIOB , &GPIO_InitStruct );

	// BUSY
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init( GPIOB , &GPIO_InitStruct );

	// DISCHARGE & RESET
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init( GPIOC , &GPIO_InitStruct );
}

void ChangePowerState( BitAction state )
{
	GPIO_WriteBit( GPIOB, GPIO_Pin_1 , state );
}

void ChangeResetState( BitAction state )
{
	GPIO_WriteBit( GPIOC, GPIO_Pin_11 , state );
}

void ChangeDischargeState( BitAction state )
{
	GPIO_WriteBit( GPIOC, GPIO_Pin_10 , state );
}

void ChangeBorderState( BitAction state )
{
	GPIO_WriteBit( GPIOB, GPIO_Pin_14 , state );
}

uint8_t IsBusy ( void )
{
	return GPIO_ReadInputDataBit( GPIOB , GPIO_Pin_13 );
}

void SPI1_Init ( void )
{
	/* Initialize GPIO for SPI
	 * A5 : SCK
	 * A6 : MISO
	 * A7 : MOSI
	 * A4 : CS
	 */
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE );

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init( GPIOA , &GPIO_InitStruct );

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);


	// CS Pin A4
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init( GPIOA , &GPIO_InitStruct );

	GPIO_SetBits( GPIOA , GPIO_Pin_4 );

	/* Initialize SPI
	 * 5.25 Mbits/s (APB2_Clock / 16)
	 * Mode 0 ( CPHA on first edge , CPOL is Low )
	 * Data size = 8 bits
	 * Full duplex
	 * MSB first
	 * Mode master
	 * NSS ?
	 */
	SPI_InitTypeDef SPI_InitStruct;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 , ENABLE );

	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSSInternalSoft_Set;

	SPI_Init( SPI1 , &SPI_InitStruct );

	SPI_Cmd(SPI1, ENABLE);
}

/* This funtion is used to transmit and receive data
 * with SPI1
 * *data --> pointer to data to be transmitted
 * The received value will be returned in it.
 */
void SPI1_send(uint8_t * data)
{
	// write data to be transmitted to the SPI data register
	SPI1->DR = *data;

	// wait until transmit complete
	while( !(SPI1->SR & SPI_I2S_FLAG_TXE) );
	// wait until receive complete
	while( !(SPI1->SR & SPI_I2S_FLAG_RXNE) );
	// wait until SPI is not busy anymore
	while( SPI1->SR & SPI_I2S_FLAG_BSY );
	*data =  SPI1->DR; // return received data from SPI data register
}
