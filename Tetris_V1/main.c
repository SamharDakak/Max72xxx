/*
 * Tetris_V1.c
 *
 * Created: 06/04/2021 19:41:36
 * Author : dakak
 */ 


#include "Tetris.h"

#define NUM_DEVICES    2	 // Number of cascaded max7219's, or just 1
void initSPI(void)
{
	DDRB |= (1 << PB2);		// Set SS output
	PORTB |= (1 << PB2);      // Begin high (unselected)

	DDRB |= (1 << PB3);       // Output on MOSI
	DDRB |= (1 << PB5);       // Output on SCK

	SPCR |= (1 << MSTR);            // Clockmaster
	SPCR |= (1 << SPE);       // Enable SPI
	
}
//**********************************************************************************************************************************************************
// Send byte through SPI
void writeByte(uint8_t byte)
{
	SPDR = byte;                      // SPI starts sending immediately
	while(!(SPSR & (1 << SPIF)));     // Loop until complete bit set
}



//**********************************************************************************************************************************************************
// Sends word through SPI
void writeWord(uint8_t address, uint8_t data)
{
	writeByte(address);	// Write first byte
	writeByte(data);      // Write Second byte
}


//**********************************************************************************************************************************************************
// Initializes all cascaded devices

void initMatrix()
{
	uint8_t i;	// Var used in for loops

	// Set display brighness
	SLAVE_SELECT;
	for(i = 0; i < NUM_DEVICES; i++)	// Loop through number of cascaded devices
	{
		writeByte(0x0E); // Select Intensity register
		writeByte(0x07); // Set brightness
	}
	SLAVE_DESELECT;

	
	// Set display refresh
	SLAVE_SELECT;
	for(i = 0; i < NUM_DEVICES; i++)
	{
		writeByte(0x0B); // Select Scan-Limit register
		writeByte(0x07); // Select columns 0-7
	}
	SLAVE_DESELECT;

	
	// Turn on the display
	SLAVE_SELECT;
	for(i = 0; i < NUM_DEVICES; i++)
	{
		writeByte(0x0C); // Select Shutdown register
		writeByte(0x01); // Select Normal Operation mode
	}
	SLAVE_DESELECT;

	
	// Disable Display-Test
	SLAVE_SELECT;
	for(i = 0; i < NUM_DEVICES; i++)
	{
		writeByte(0x0F); // Select Display-Test register
		writeByte(0x00); // Disable Display-Test
	}
	SLAVE_DESELECT;
}
//**********************************************************************************************************************************************************
void clearMatrix(void)
{
	for(uint8_t x = 1; x < 9; x++) // for all columns
	{
		SLAVE_SELECT;
		for(uint8_t i = 0; i < NUM_DEVICES; i++)
		{
			writeByte(x);    // Select column x
			writeByte(0x00); // Set column to 0
		}
		SLAVE_DESELECT;
	}
}



int main(void)
{
	initSPI();
	initMatrix();
	clearMatrix();
	

	
    /* Replace with your application code */
    while (1) 
    {
    }
}

