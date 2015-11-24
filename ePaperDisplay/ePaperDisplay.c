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

	uint8_t ID = GetCOGid();
	// Get ID from COG (0x11 = COG1, 0x12 = COG2).
	if ( !(ID == 0x11 || ID == 0x12) )
	{
		ChangeLedState( ORANGE , Bit_SET );
		//return;
	}

	DisableOE();

	if ( !CheckBreakage() )
	{
		ChangeLedState( RED , Bit_SET );
		// return;
	}

	EnterPowerSavingMode();
	ChannelSelect();
	OscillatorPowerMode( HIGH_PWR );
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

	dummy = SPI1_Send( 0x00 );

	ChangeResetState( Bit_RESET );
	ChangePowerState( Bit_RESET );
	ChangeDischargeState( Bit_RESET );
	ChangeBorderState( Bit_RESET );
	ChangeCSState( Bit_RESET );

	delay_nms( 5 );

	ChangePowerState( Bit_SET );

	delay_nms( 10 );


	ChangeResetState( Bit_SET );
	ChangeBorderState( Bit_SET );
	ChangeCSState( Bit_SET );

	delay_nms( 10 );

	ChangeResetState( 		Bit_RESET );
	delay_nms( 10 );

	ChangeResetState( 		Bit_SET );
	delay_nms( 10 );
}

void EPD_PowerOff ( void )
{
	uint8_t * dummy = (uint8_t *) 0x00 ;

	// Empty buffer
	DisplayNothing();
	DisplayLine( dummy , 0 , NOTHING );
	delay_nms( 20 );

	RefreshBorder( 200 );
	PowerSettings3();
	DriverLatchOn();

	ChargePumpVoltageSettings( VCOM_OFF );
	ChargePumpVoltageSettings( NEGATIVE_OFF );
	delay_nms( 350 );

	DischargeInternal();
	ChargePumpVoltageSettings( POSITIVE_OFF );
	OscillatorPowerMode( OSCILLATOR_OFF );
	delay_nms( 75 );

	ChangeBorderState( Bit_RESET );
	ChangePowerState( Bit_RESET );
	delay_nms( 20 );

	ChangeResetState( Bit_RESET );
	ChangeCSState( Bit_RESET );
	ChangeDischargeState( Bit_SET );
	delay_nms( 200 );

	ChangeDischargeState( Bit_SET );
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

void OscillatorPowerMode ( eOSCILLATOR_MODE mode )
{
	uint16_t dummy = 0;
	uint8_t highPowerMode[4] = { CMD_HDR , 0x07 , DATA_W_HDR , (uint8_t) mode };

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

void PowerSettings3 ( void )
{
	uint16_t dummy = 0;
	uint8_t powerSettings[4] = { CMD_HDR , 0x0B , DATA_W_HDR , 0x00 };

	// Turn on OE Send 0x70 0x02 0x72 0x07
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

	for ( i = 0 ; i < 4 ; i++ )
	{
		ChargePumpVoltageSettings( POSITIVE_ON );
		delay_nms( 200 );
		ChargePumpVoltageSettings( NEGATIVE_ON );
		delay_nms( 100 );
		ChargePumpVoltageSettings( VCOM_ON );
		delay_nms( 40 );

		if ( CheckDCDC() )
		{
			OutputEnaToDisa();
			break;
		}
	}
	if (i == 4)
	{
		// ChangeLedState( RED , Bit_SET );
		// return;
	}
}

void ChargePumpVoltageSettings( eVOLTAGE_SETTINGS settings )
{
	uint16_t dummy = 0;
	uint8_t pumpSettings[4] = { CMD_HDR , 0x05 , DATA_W_HDR , (uint8_t)settings };

	dummy = SPI1_Read( pumpSettings , 2 );
	dummy = SPI1_Read( pumpSettings + 2 , 2 );
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

void DischargeInternal( void )
{
	uint16_t dummy = 0;
	uint8_t dischargeInternal[4] = { CMD_HDR , 0x04 , DATA_W_HDR , 0x80 };

	// Discharge internal Send 0x70 0x04 0x72 0x80
	dummy = SPI1_Read( dischargeInternal , 2 );
	dummy = SPI1_Read( dischargeInternal + 2 , 2 );
}

void DisplayImage ( uint8_t * image )
{
	DisplayInverse( image );
	DisplayBW( image );
	DisplayNew( image );
	RefreshBorder( 50 );
}

void DisplayBW ( uint8_t * image )
{
	uint8_t i = 0;
	uint8_t repeat = 0;
	uint8_t stageTime = 0;

	for ( repeat = 0 ; repeat < REPEAT_CYCLE ; repeat++ )
	{
		for( stageTime = 0 ; stageTime < STAGE_TIME ; stageTime++ )
		{
			for ( i = 0 ; i < 176 ; i++)
			{
				DisplayUnicolorLine( i , BLACK );
			}
			for ( i = 0 ; i < 176 ; i++)
			{
				DisplayUnicolorLine( i , WHITE );
			}
		}
	}
}

void DisplayInverse ( uint8_t * image )
{
	uint16_t i = 0;

	for ( i = 0 ; i < 176 ; i++)
	{
		DisplayLine( image + ( 33 * i ) , i , INVERSE );
	}
}

void DisplayNew ( uint8_t * image )
{
	uint16_t i = 0;

	for ( i = 0 ; i < 176 ; i++)
	{
		DisplayLine( image + ( 33 * i ) , i , NEW);
	}
}

void DisplayNothing ( void )
{
	uint8_t i = 0;
	uint8_t * dummy = (uint8_t *) 0x00 ;

	for ( i = 0 ; i < 176 ; i++)
	{
		DisplayDummyLine();
	}
}

void DisplayUnicolorLine ( uint8_t line , eSTAGE stage )
{
	uint8_t data[ 112 ] = { 0x00 };
	data[ 0 ] = DATA_W_HDR;
	data[ 1 ] = 0x00;

	uint8_t i = 0;
	for ( i = 0 ; i < 33 ; i++ )
	{
		switch (stage)
		{
			case BLACK:
				data[ i + 2 ] = data[ i + 79 ] = 0xFF;
				break;

			case WHITE:
				data[ i + 2 ] = data[ i + 79 ] = 0xAA;
				break;
			default:
				break;
		}
	}

	uint8_t scan_index = 35 + 43 - (line / 4);
	data[ scan_index ] = 0b00000011 << ( 2 * ( line % 4 ) );

	uint8_t header[ 2 ] = { CMD_HDR , START_FRAME };

	uint8_t dummy = 0;
	dummy = SPI1_Read( header , 2 );
	dummy = SPI1_Read( data , 112 );

	TurnOnOE();
}

void DisplayDummyLine ( void )
{
	uint8_t dummy = 0;

	uint8_t header[ 2 ] = { CMD_HDR , START_FRAME };
	dummy = SPI1_Read( header , 2 );

	uint8_t data[ 112 ] = { 0x00 };
	data[ 0 ] = DATA_W_HDR;
	dummy = SPI1_Read( data , 112 );

	TurnOnOE();
}

void DisplayLine ( uint8_t * image , uint8_t line , eSTAGE stage )
{
	uint8_t i = 0;

	uint8_t data_scan[ 44 ] = { 0x00 };
	data_scan[ 43 - ( line / 4 ) ] = 0b00000011 << ( 2 * ( line % 4 ) );

	uint8_t data_odd[ 33 ]  = { 0x00 };
	uint8_t data_even[ 33 ] = { 0x00 };

	if ( stage != NOTHING )
	{
		for ( i = 0 ; i < 33 ; i++ )
		{
			uint8_t oddIndex = 32 - i;

			data_odd[ oddIndex ] = image[i] & ODD_MASK;
			REVERSE( data_odd[ oddIndex] );
			data_odd[ oddIndex ] |= BW_MASK;

			data_even[ i ] = image[i] & EVEN_MASK;
			data_even[ i ] |= BW_MASK;

			switch (stage)
			{
				case INVERSE:
					data_odd[ oddIndex ] ^= INVERSE_MASK;
					data_even[ i ] ^= INVERSE_MASK;
					break;
				default:
					break;
			}
		}
	}

	uint8_t data[ 112 ] = { 0x00 };
	data[ 0 ] = DATA_W_HDR;
	data[ 1 ] = 0x00;

	for ( i = 0 ; i < 33 ; i++ )
	{
		data[ i + 2 ] = data_odd[ i ];
		data[ i + 79 ] = data_even[ i ];
	}
	for ( i = 0 ; i < 44 ; i++ )
		data[ i + 35 ] = data_scan[ i ];

	uint8_t header[ 2 ] = { CMD_HDR , START_FRAME };

	uint8_t dummy = 0;
	dummy = SPI1_Read( header , 2 );
	dummy = SPI1_Read( data , 112 );

	TurnOnOE();
}

void RefreshBorder ( uint8_t delay )
{
	ChangeBorderState( Bit_RESET );
	delay_nms( delay );
	ChangeBorderState( Bit_SET );
}
