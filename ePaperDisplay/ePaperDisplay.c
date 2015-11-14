#include "ePaperDisplay.h"
#include "Hello_World.h"
/*
 * https://github.com/embeddedartists/gratis/blob/master/Sketches/libraries/EPD2/EPD2.cpp
 * http://www.pervasivedisplays.com/_literature_138408/E-paper_Display_COG_Driver_Interface_Timing_V110
 */

void EPD_Init ( void )
{
	EPD_PowerOn();

	while ( IsBusy() )
		delay_nus( 10 );

	// Get ID from COG (0x11 = COG1, 0x12 = COG2).
	if ( GetCOGid() != 0x11 )
	{
		// return;
	}

	DisableOE();

	if ( !CheckBreakage() )
	{
		ChangeLedState( RED , Bit_SET );
		// return;
	}

	EnterPowerSavingMode();
	ChannelSelect();
	HighPowerMode();
	PowerSettings1();
	SetVComLevel();
	PowerSettings2();
	DriverLatchOn();
	DriverLatchOff();

	delay_nms( 10 );

	ChargePump();
}

void EPD_PowerOn ( void )
{
	uint8_t dummy = 0;

	ChangeResetState( 		Bit_RESET );
	ChangePowerState( 		Bit_RESET );
	ChangeDischargeState( 	Bit_RESET );
	ChangeBorderState( 		Bit_RESET );
	ChangeCSState(			Bit_RESET );

	dummy = SPI1_Send( 0x00 );
	dummy = SPI1_Send( 0x00 );

	delay_nms( 5 );

	ChangePowerState( 		Bit_SET );

	delay_nms( 10 );

	ChangeCSState(			Bit_SET );
	ChangeResetState( 		Bit_SET );
	ChangeBorderState( 		Bit_SET );

	delay_nms( 10 );

	ChangeResetState( 		Bit_RESET );
	delay_nms( 10 );

	ChangeResetState( 		Bit_SET );
	delay_nms( 10 );
}

uint16_t GetCOGid ( void )
{
	// Read 0x71,0x00 twice
	uint8_t data[2] = { GET_ID };
	SPI1_Read( data , 2 );
	return SPI1_Read( data , 2 );
}

void DisableOE ( void )
{
	uint16_t dummy = 0;
	uint8_t disableOE[4] = { CMD_HDR , 0x02 , DATA_W_HDR , 0x40 };

	// Disable OE. Send 0x70 0x02 0x72 0x40
	dummy = SPI1_Read( disableOE , 2 );
	dummy = SPI1_Read( disableOE + 2 , 2 );
}

uint8_t CheckBreakage( void )
{
	uint16_t dummy = 0;
	uint8_t readBreakage[4] = { CMD_HDR , 0x0F , DATA_R_HDR , 0x00 };

	// Check breakage. Read 0x70 0x0F 0x73 0x00
	dummy = SPI1_Read( readBreakage , 2 );
	return ( (SPI1_Read( readBreakage + 2 , 2 ) & 0x80) == 0x80 );
}

void EnterPowerSavingMode ( void )
{
	uint16_t dummy = 0;
	uint8_t enterPwrSav[4] = { CMD_HDR , 0x0B , DATA_W_HDR , 0x02 };

	// Enter power saving mode. Send 0x70 0x0B 0x72 0x02
	dummy = SPI1_Read( enterPwrSav , 2 );
	dummy = SPI1_Read( enterPwrSav + 2 , 4 );
}

void ChannelSelect ( void )
{
	uint16_t dummy = 0;
	uint8_t channelSelect[11] = {	CMD_HDR , 0x01 , DATA_W_HDR , 0x00,
									0x00 , 0x00 , 0x7F , 0xFF,
									0xFE , 0x00 , 0x00 };

	// Select channel. Send 0x70 0x01 0x72 0x00 0x00 0x00 0x7F 0xFF 0xFE 0x00 0x00
	dummy = SPI1_Read( channelSelect , 2 );
	dummy = SPI1_Read( channelSelect + 2, 11 );
}

void HighPowerMode ( void )
{
	uint16_t dummy = 0;
	uint8_t highPowerMode[4] = { CMD_HDR , 0x07 , DATA_W_HDR , 0xD1 };

	// Enter high power mode. Send 0x70 0x07 0x72 0xD1
	dummy = SPI1_Read( highPowerMode , 2 );
	dummy = SPI1_Read( highPowerMode + 2 , 4 );
}

void PowerSettings1 ( void )
{
	uint16_t dummy = 0;
	uint8_t powerSettings[4] = { CMD_HDR , 0x08 , DATA_W_HDR , 0x02 };

	// Power settings 1. Send 0x70 0x08 0x72 0x02
	dummy = SPI1_Read( powerSettings , 2 );
	dummy = SPI1_Read( powerSettings + 2 , 2 );
}

void SetVComLevel ( void )
{
	uint16_t dummy = 0;
	uint8_t vcomLevel[4] = { CMD_HDR , 0x09 , DATA_W_HDR , 0xC2 };

	// Set VCom level. Send 0x70 0x09 0x72 0xc2
	dummy = SPI1_Read( vcomLevel , 2 );
	dummy = SPI1_Read( vcomLevel + 2 , 2 );
}

void PowerSettings2 ( void )
{
	uint16_t dummy = 0;
	uint8_t powerSettings[4] = { CMD_HDR , 0x04 , DATA_W_HDR , 0x03 };

	// Power settings 2. Send 0x70 0x04 0x72 0x03
	dummy = SPI1_Read( powerSettings , 2 );
	dummy = SPI1_Read( powerSettings + 2 , 2 );
}

void DriverLatchOn ( void )
{
	uint16_t dummy = 0;
	uint8_t latchOn[4] = { CMD_HDR , 0x03 , DATA_W_HDR , 0x01 };

	// Driver latch on. Send 0x70 0x03 0x72 0x01
	dummy = SPI1_Read( latchOn , 2 );
	dummy = SPI1_Read( latchOn + 2 , 2 );
}

void DriverLatchOff( void )
{
	uint16_t dummy = 0;
	uint8_t latchOff[4] = { CMD_HDR , 0x03 , DATA_W_HDR , 0x00 };

	// Driver latch off. Send 0x70  0x03 0x72 0x00
	dummy = SPI1_Read( latchOff , 2 );
	dummy = SPI1_Read( latchOff + 2 , 2 );
}

void ChargePump( void )
{
	uint8_t i = 0;
	uint16_t dummy = 0;
	uint8_t setChrgPmpPos[4] = { CMD_HDR , 0x05 , DATA_W_HDR , 0x01 };
	uint8_t setChrgPmpNeg[4] = { CMD_HDR , 0x05 , DATA_W_HDR , 0x03 };
	uint8_t setChrgPmpVcom[4] = { CMD_HDR , 0x05 , DATA_W_HDR , 0x0F };

	for ( i = 0 ; i < 4 ; i++ )
	{
		dummy = SPI1_Read( setChrgPmpPos , 2 );
		dummy = SPI1_Read( setChrgPmpPos + 2 , 2 );
		delay_nms( 200 );
		dummy = SPI1_Read( setChrgPmpNeg , 2 );
		dummy = SPI1_Read( setChrgPmpNeg + 2 , 2 );
		delay_nms( 100 );
		dummy = SPI1_Read( setChrgPmpVcom , 2 );
		dummy = SPI1_Read( setChrgPmpVcom + 2 , 2 );
		delay_nms( 40 );

		if ( CheckDCDC() )
		{
			OutputEnaToDisa();
			break;
		}
	}
	if (i == 4)
	{
		ChangeLedState( RED , Bit_SET );
	}
}

uint8_t CheckDCDC ( void )
{
	uint16_t dummy = 0;
	uint8_t checkDCDC[4] = { CMD_HDR , 0x0F , DATA_R_HDR , 0x00 };

	// Check DCDC Send 0x70 0x0F 0x73 0x00
	dummy = SPI1_Read( checkDCDC , 2 );
	return ( (SPI1_Read( checkDCDC + 2 , 2 ) & 0x40) == 0x40 );
}

void OutputEnaToDisa ( void )
{
	uint16_t dummy = 0;
	uint8_t outputEnableToDisable[4] = { CMD_HDR , 0x02 , DATA_W_HDR , 0x06 };

	// Output Enable to Disable Send 0x70 0x02 0x72 0x06
	dummy = SPI1_Read( outputEnableToDisable , 2 );
	dummy = SPI1_Read( outputEnableToDisable + 2 , 2 );
}

void TurnOnOE ( void )
{
	uint16_t dummy = 0;
	uint8_t turnOnOE[4] = { CMD_HDR , 0x02 , DATA_W_HDR , 0x07 };

	// Turn on OE Send 0x70 0x02 0x72 0x07
	dummy = SPI1_Read( turnOnOE , 2 );
	dummy = SPI1_Read( turnOnOE + 2 , 2 );
}

void DisplayImage ( uint8_t * image )
{
	uint16_t i = 0;

	DisplayInverse( image );
	DisplayBW( image );

	for ( i = 0 ; i < 176 ; i++)
	{
		DisplayLine( image + (264 * i) );
	}
}

void DisplayInverse ( uint8_t * image )
{
	uint8_t j = 0;

	for ( j = 0 ; j < 176 ; j++)
	{
		DisplayLine( image + (264 * i) , INVERSE );
	}
}

void DisplayBW ( uint8_t * image )
{

}

void DisplayLine ( uint8_t * line , STAGE stage )
{
	uint8_t i = 0;
	uint8_t data_scan[ 44 ] = { 0x00 };

	uint8_t data_odd[ 33 ] = { 0x00 };
	uint8_t data_even[ 33 ] = { 0x00 };
	for ( i = 33 ; i > 0 ; i-- )
	{
		data_odd[ 33 - i ] = line[i] & ODD_MASK;
		data_odd[ 33 - i ] >>= 1;
		data_odd[ 33 - i ] |= BW_MASK;

		data_even[ i - 1 ] = line[i] & EVEN_MASK;
		data_even[ i - 1 ] |= BW_MASK;

		if ( stage == INVERSE )
		{
			data_odd[ 33 - i ] ^= INVERSE_MASK;
			data_even[ i - 1 ] ^= INVERSE_MASK;
		}
	}

	uint8_t data[ 112 ] = { 0x00 };
	data[ 1 ] = DATA_W_HDR;

	for ( i = 0 ; i < 33 ; i++ )
		data[ i + 2 ] = data_odd[ i ];
	for ( i = 0 ; i < 44 ; i++ )
		data[ i + 35 ] = data_scan[ i ];
	for ( i = 0 ; i < 33 ; i++ )
		data[ i + 79 ] = data_even[ i ];

	uint8_t header[ 2 ] = { CMD_HDR , START_FRAME };

	uint8_t dummy = 0;
	dummy = SPI1_Read( header , 2 );
	dummy = SPI1_Read( data , 112 );

	TurnOnOE();
}
