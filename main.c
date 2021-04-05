// MAX 7219 8x8 RED LED Display
// Using AVR MCU (ATmega328P), displays a message on a number of cascaded 8x8 matrices.
// Message and array for LED character representations are stored in flash.
// SPI is used to send data to the matrix driver chips.
// Last Modified: April 11th 2016
// Author: Joey Conenna



#include "max7219_8x8.h"




#define NUM_DEVICES     8	 // Number of cascaded max7219's, or just 1
#define DEL 		140000    // Delay for scrolling speed in microseconds


// Buffer array of bytes to store current display data for each column in each cascaded device
uint8_t buffer [NUM_DEVICES*8];


// Initializes SPI
void initSPI(void)
{
	DDRB |= (1 << PB2);		// Set SS output
	PORTB |= (1 << PB2);      // Begin high (unselected)

	DDRB |= (1 << PB3);       // Output on MOSI
	DDRB |= (1 << PB5);       // Output on SCK

	SPCR |= (1 << MSTR);            // Clockmaster
	SPCR |= (1 << SPE);       // Enable SPI
	
}

void Uart_init (){
	
	UBRR0L= UBRR0_Value;
	UCSR0B=(1<<TXEN0) | (1<<RXEN0);
	UCSR0C= (1<< UCSZ00) | (1<<UCSZ01);
}
void uart_outputchar (char ch )
{
	while (!(UCSR0A &(1<<UDRE0)));
	UDR0=ch ;
}

void uart_output (char *calledstring )
{
	
	for (int i =0 ; i<255; i++){
		if (calledstring[i]!=0)
		{
			
			uart_outputchar(calledstring[i]);
			
			if (calledstring[i]>=32)
			{
				
			
		pushCharacter(calledstring[i]-32);
							            // Display &
		
			}
			
		}
		
		
		else
		{
		
						 
			break;
		}
	}
	
}


char uart_getnew () {
	
	while (!(UCSR0A&(1<<RXC0)));
	
	return UDR0;
	
}

void  Uart_readnew (char *calledstring )
{
	char ch ;
	int counter = 0 ; 
	
	
	while (1)
	{
		ch = uart_getnew();
		
		if (ch==13)// new Line 
		{
			calledstring[counter]=0;
			return ; 
			
		}
		else {
			calledstring[counter]=ch;
			counter ++;
		}
	}
	
	
}




// Send byte through SPI
void writeByte(uint8_t byte)
{
	SPDR = byte;                      // SPI starts sending immediately
	while(!(SPSR & (1 << SPIF)));     // Loop until complete bit set
}




// Sends word through SPI
void writeWord(uint8_t address, uint8_t data)
{
	writeByte(address);	// Write first byte
	writeByte(data);      // Write Second byte
}



// Initializes all cascaded devices
void initMatrix()
{
	uint8_t i;	// Var used in for loops

	// Set display brighness
	SLAVE_SELECT;
	for(i = 0; i < NUM_DEVICES; i++)	// Loop through number of cascaded devices
	{
		writeByte(0x0A); // Select Intensity register
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


// Clears all columns on all devices
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




/*Initializes buffer empty*/
void initBuffer(void)
{
	
	
	
	for(uint8_t i = 0; i < NUM_DEVICES*8; i++)
	buffer[i] = 0x00;
	
	
}



/*Moves each byte forward in the buffer and adds next byte in at the end*/
void pushBuffer(uint8_t x)
{
	
	for(uint8_t i = 0; i < NUM_DEVICES*8 - 1; i++)
	buffer[i] = buffer[i+1];
	
	buffer[NUM_DEVICES*8 - 1]=x ;
	
}
void pushBuffer_hindrnace(uint8_t x)
{
	
	for(uint8_t i = 0; i < NUM_DEVICES*8 - 1; i++)
	buffer[i] = buffer[i+1];
	
	buffer[(NUM_DEVICES)*8 - 1]=x ;
	
}


// Pushes in 5 characters columns into the buffer.
void pushCharacter(uint8_t c)
{
	
	for(uint8_t i = 0; i < 8; i++)						// For 5 bytes representing each character
	{
		
		pushBuffer(pgm_read_byte(&Alphabet[c][i]));   // Push the byte from the characters array to the display buffer
		_delay_us(DEL);
		// and delay
	}
	displayBuffer();								// Display the current buffer on the devices
	
}

void pushCharacter_Hindrance(uint8_t c)
{
	
	for(uint8_t i = 0; i < 8; i++)						// For 5 bytes representing each character
	{
		
		pushBuffer_hindrnace(pgm_read_byte(&Hindrance[c][i]));   // Push the byte from the characters array to the display buffer
		_delay_us(DEL);
		// and delay
		
	}
			displayBuffer();				// Display the current buffer on the devices
		
}

void pushCharacter_DISTANCE()
{
	

		pushBuffer_hindrnace(0);   // Push the byte from the characters array to the display buffer
		_delay_us(DEL);
		// and delay
		

	displayBuffer();				// Display the current buffer on the devices
	
}
//
// // Takes a pointer to the beginning of a char array holding message, and array size, scrolls message.
void displayMessage(const char *arrayPointer, uint16_t arraySize)
{
	for(uint16_t i = 0; i < arraySize; i++)
	{
		pushCharacter(pgm_read_byte(arrayPointer + i) -32);	// Send converted ASCII value of character in message to index in characters array (-32 sends corrent index to characters array)
		/*pushBuffer(0x00);*/								            // Add empty column after character for letter spacing
		displayBuffer();							            // Display &
		// Delay
	}
	
	/*_delay_us(DEL);*/
	
}

// Displays current buffer on the cascaded devices
void displayBuffer()
{
	for(uint8_t i = 0; i < NUM_DEVICES; i++) // For each cascaded device
	{
		for(uint8_t j = 1; j < 9; j++) // For each column
		{
			SLAVE_SELECT;
			
			for(uint8_t k = 0; k < i; k++) // Write Pre No-Op code
			writeWord(0x00, 0x00);
			
			writeWord(j, buffer[j + i*8 - 1]); // Write column data from buffer
			
			
			for(uint8_t k = NUM_DEVICES-1; k > i; k--) // Write Post No-Op code
			writeWord(0x00, 0x00);
			
			SLAVE_DESELECT;
		}
		
	}
}
void displaycar()
{
	for(uint8_t i = 0; i < NUM_DEVICES; i++) // For each cascaded device
	{
		for(uint8_t j = 1; j < 9; j++) // For each column
		{
			SLAVE_SELECT;
			
			// 			for(uint8_t k = 0; k < i; k++) // Write Pre No-Op code
			// 			writeWord(0x00, 0x00);
			
			writeWord(j, buffer[j + i*8 - 1]); // Write column data from buffer
			
			
			// 			for(uint8_t k = NUM_DEVICES-1; k > i; k--) // Write Post No-Op code
			// 			writeWord(0x00, 0x00);
			
			SLAVE_DESELECT;
		}
		
	}
}
// void display_road()
// {
// 	SLAVE_SELECT;
// 	
// 	writeWord(8,0);
// 	writeWord(8,pgm_read_byte(&Road[0][7]));
// 	writeWord(8,pgm_read_byte(&Road[0][7]));
// 	writeWord(8,pgm_read_byte(&Road[0][7]));
// 	writeWord(8,pgm_read_byte(&Road[0][7]));
// 	writeWord(8,pgm_read_byte(&Road[0][7]));
// 	writeWord(8,pgm_read_byte(&Road[0][7]));
// 	writeWord(8,pgm_read_byte(&Road[0][7]));
// 	
// 
//  SLAVE_DESELECT;
// 	
// }
void display_Hindrance()
{
	SLAVE_SELECT;
	writeWord(8,0);
	writeWord(8,0);
	writeWord(8,0);
	writeWord(8,0);
	writeWord(8,0);
	writeWord(6,pgm_read_byte(&Hindrance[0][5]));
	

SLAVE_DESELECT;

SLAVE_SELECT;
	writeWord(8,0);
	writeWord(8,0);
	writeWord(8,0);
	writeWord(8,0);
	writeWord(8,0);
	writeWord(7,pgm_read_byte(&Hindrance[0][6]));


SLAVE_DESELECT;
SLAVE_SELECT;
writeWord(8,0);
writeWord(8,0);
writeWord(8,0);
writeWord(8,0);
writeWord(8,0);
writeWord(8,pgm_read_byte(&Hindrance[0][7]));


SLAVE_DESELECT;
	
	
}


int sekunden_erzeuger( int sec ){
	
	
	for(int s = 0; s < 6; s++){
		
		for(int j = 0; j < 10; j++){
			sec++;
			for(uint8_t i = 0; i < NUM_DEVICES; i++){
				
				
				SLAVE_SELECT;
				
				writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
				writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
				writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
				writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
				writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
				writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
				writeWord(i+1,pgm_read_byte(&Alphabet[16+s][i]));
				writeWord(i+1,pgm_read_byte(&Alphabet[16+j][i]));
					_delay_us(DEL);
				SLAVE_DESELECT;
			}
			
			
		}
		
		
	}
	return (sec);
}
int minuten_erzeuger(int sec ,int min){
	if (sec == 60 )
	{
		sec=0;
		
		for(uint8_t m = 0; m < 6; m++){
			
			for(uint8_t mm = 0; mm < 10; mm++){
				min++;
				for(uint8_t s = 1; s < 6; s++){
					for(uint8_t j = 0; j < 10; j++){
						sec++;
						for(uint8_t i = 0; i < NUM_DEVICES; i++){
							
							
							SLAVE_SELECT;
							
							writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
							writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
							writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
							writeWord(i+1,pgm_read_byte(&Alphabet[16+m][i]));
							writeWord(i+1,pgm_read_byte(&Alphabet[16+mm][i]));
							writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
							writeWord(i+1,pgm_read_byte(&Alphabet[16+s][i]));
							writeWord(i+1,pgm_read_byte(&Alphabet[16+j][i]));
								_delay_us(DEL);
							SLAVE_DESELECT;
						}
					}
				}
				
			}
			
		}
	}
	return (min);
}
int stunden_erzeuger (int sec , int min , int stu ){
	if (min==60){
		
		min=0;
		
		
			for(uint8_t h = 0; h < 3; h++){
		
		for(uint8_t hh = 0; hh < 10; hh++)
		{
			stu++;
			
			for(uint8_t m = 0; m < 6; m++){
				for(uint8_t mm = 0; mm < 10;mm++){
					min++;
					for(uint8_t s = 0; s < 6; s++){
						for(uint8_t j = 0; j < 10; j++){
							sec++;
							for(uint8_t i = 0; i < NUM_DEVICES; i++){
								
								
								SLAVE_SELECT;
								
								writeWord(i+1,pgm_read_byte(&Alphabet[16+h][i]));
								writeWord(i+1,pgm_read_byte(&Alphabet[16+hh][i]));
								writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
								writeWord(i+1,pgm_read_byte(&Alphabet[16+m][i]));
								writeWord(i+1,pgm_read_byte(&Alphabet[16+mm][i]));
								writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
								writeWord(i+1,pgm_read_byte(&Alphabet[16+s][i]));
								writeWord(i+1,pgm_read_byte(&Alphabet[16+j][i]));
									_delay_us(DEL);
								SLAVE_DESELECT;
							}
						}
					}
				}
			}
			
		}
		
	}
	
	if (stu == 23)
	{
		
	
	
	for(uint8_t h = 0; h < 3; h++){
		
			
			for(uint8_t hh = 0; hh < 4; hh++)
			{
				stu++;
				for(uint8_t m = 0; m < 6; m++){
					for(uint8_t mm = 0; mm < 10;mm++){
						min++;
						for(uint8_t s = 0; s < 6; s++){
							for(uint8_t j = 0; j < 10; j++){
								sec++;
								for(uint8_t i = 0; i < NUM_DEVICES; i++){
									
									
									SLAVE_SELECT;
									
									writeWord(i+1,pgm_read_byte(&Alphabet[16+h][i]));
									writeWord(i+1,pgm_read_byte(&Alphabet[16+hh][i]));
									writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
									writeWord(i+1,pgm_read_byte(&Alphabet[16+m][i]));
									writeWord(i+1,pgm_read_byte(&Alphabet[16+mm][i]));
									writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
									writeWord(i+1,pgm_read_byte(&Alphabet[16+s][i]));
									writeWord(i+1,pgm_read_byte(&Alphabet[16+j][i]));
									
									SLAVE_DESELECT;
									
									
								}
							}
						}
					}
				}
			}
		}
		
	}
}

	
	

return(stu);
}
void Max (int stu  ){
	
	if (stu==24){
		stu=00;
		
		for(uint8_t i = 0; i < NUM_DEVICES; i++){
			
			SLAVE_SELECT;
			
			

			writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
			writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
			writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
			writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
			writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
			writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
			writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
			writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));


			SLAVE_DESELECT;
			/*_delay_us(DEL);*/
		}
		
		
		

	}
}
void setstopwatch ()

{
	
	
	for(uint8_t i = 0; i < NUM_DEVICES; i++){
		
		SLAVE_SELECT;
		
		

		writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
		writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
		writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
		writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
		writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
		writeWord(i+1,pgm_read_byte(&Alphabet[26][i]));
		writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
		writeWord(i+1,pgm_read_byte(&Alphabet[16][i]));
         

		SLAVE_DESELECT;
		

		
	}
	_delay_us(DEL);
	
	int sec = 0;
	int min = 0 ;
	int stu = 0;
	
	int V= sekunden_erzeuger( sec );
	

	
	int M = minuten_erzeuger(V , min);
	
	
	
	int S= stunden_erzeuger(sec,M,stu);
	
	Max(S);
	
	

	

}


void show_car ()
{

	for(uint8_t i = 0; i < 8; i++)						// For 5 bytes representing each character
	{
		
		pushBuffer(pgm_read_byte(&car[0][i]));   // Push the byte from the characters array to the display buffer
		
		// and delay
	}
	displaycar();
}
void car_car ()
{
	for (int i = 0; i<8;i++)
	{	
	SLAVE_SELECT;
	writeWord(i+1,pgm_read_byte(&car[0][i]));
	SLAVE_DESELECT;
	}
}
void show_car_up ()
{
	for(uint8_t i = 0; i < 8; i++)						// For 5 bytes representing each character
	{
		
		pushBuffer(pgm_read_byte(&car_up[0][i]));   // Push the byte from the characters array to the display buffer
		
		// and delay
	}
	displaycar();
}
void show_road ()
{
	for(uint8_t i = 5; i < 8; i++)						// For 5 bytes representing each character
	{
		
		pushBuffer(pgm_read_byte(&Hindrance[0][i]));   // Push the byte from the characters array to the display buffer
		
		// and delay
	}
	displayBuffer();
}





// Main Loop
int main(void)
{
	// Inits
	
	
	initSPI();
	initMatrix();
	clearMatrix();
	initBuffer();
     Uart_init();
	/*char input [255];*/
	// // 	Pointer to beginning of message
/*	const char *messagePointer = &message[0];*/
	
			 //Size of message matrix
	/*uint16_t messageSize = sizeof(message);*/
	
	// Event loop
	






// display_Hindrance();
// _delay_us(DEL);
// show_car();


// _delay_us(DEL);



	while (1)
	{
/*setstopwatch();*/

		/*displayMessage(messagePointer, messageSize);*/	// Display the message*/
car_car();
pushCharacter_DISTANCE();
pushCharacter_Hindrance(0);


// Uart_readnew(input);
// uart_output(input);
// uart_output("\r\n");


		/*writeAlphabet();*/



	}
	return (0);
}

