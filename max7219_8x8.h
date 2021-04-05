
#ifndef F_CPU                   // optional definieren
#define F_CPU 12288000UL        // ATmega 328 mit 12,288 MHz Quarz
#endif

#include <avr/io.h>

#include <util/delay.h>
#include <avr/pgmspace.h>



// Macros to set SS line LOW (selected) or HIGH (deselected)
#define SLAVE_SELECT    PORTB &= ~(1 << PB2)
#define SLAVE_DESELECT  PORTB |= (1 << PB2)
#define Baud  9600UL 
#define UBRR0_Value  ((F_CPU/(16*Baud))-1)

// Array holding arrays of 5 Bytes for each representation of an ASCII character, stored in flash
const char characters[96][5] PROGMEM = {
	{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000}, // space
	{0b00000000,0b00000000,0b01001111,0b00000000,0b00000000}, // !
	{0b00000000,0b00000111,0b00000000,0b00000111,0b00000000}, // "
	{0b00010100,0b01111111,0b00010100,0b01111111,0b00010100}, // #
	{0b00100100,0b00101010,0b01111111,0b00101010,0b00010010}, // $
	{0b00100011,0b00010011,0b00001000,0b01100100,0b01100010}, // %
	{0b00110110,0b01001001,0b01010101,0b00100010,0b01010000}, // &
	{0b00000000,0b00000101,0b00000011,0b00000000,0b00000000}, // '
	{0b00000000,0b00011100,0b00100010,0b01000001,0b00000000}, // (
	{0b00000000,0b01000001,0b00100010,0b00011100,0b00000000}, // )
	{0b00010100,0b00001000,0b00111110,0b00001000,0b00010100}, // *
	{0b00001000,0b00001000,0b00111110,0b00001000,0b00001000}, // +
	{0b00000000,0b01010000,0b00110000,0b00000000,0b00000000}, // ,
	{0b00001000,0b00001000,0b00001000,0b00001000,0b00001000}, // -
	{0b00000000,0b01100000,0b01100000,0b00000000,0b00000000}, // .
	{0b00100000,0b00010000,0b00001000,0b00000100,0b00000010}, // /
	{0b00111110,0b01010001,0b01001001,0b01000101,0b00111110}, // 0
	{0b00000000,0b01000010,0b01111111,0b01000000,0b00000000}, // 1
	{0b01000010,0b01100001,0b01010001,0b01001001,0b01000110}, // 2
	{0b00100001,0b01000001,0b01000101,0b01001011,0b00110001}, // 3
	{0b00011000,0b00010100,0b00010010,0b01111111,0b00010000}, // 4
	{0b00100111,0b01000101,0b01000101,0b01000101,0b00111001}, // 5
	{0b00111100,0b01001010,0b01001001,0b01001001,0b00110000}, // 6
	{0b00000011,0b01110001,0b00001001,0b00000101,0b00000011}, // 7
	{0b00110110,0b01001001,0b01001001,0b01001001,0b00110110}, // 8
	{0b00000110,0b01001001,0b01001001,0b00101001,0b00011110}, // 9
	{0b00000000,0b01101100,0b01101100,0b00000000,0b00000000}, // :
	{0b00000000,0b01010110,0b00110110,0b00000000,0b00000000}, // ;
	{0b00001000,0b00010100,0b00100010,0b01000001,0b00000000}, // <
	{0b00010100,0b00010100,0b00010100,0b00010100,0b00010100}, // =
	{0b00000000,0b01000001,0b00100010,0b00010100,0b00001000}, // >
	{0b00000010,0b00000001,0b01010001,0b00001001,0b00000110}, // ?
	{0b00110010,0b01001001,0b01111001,0b01000001,0b00111110}, // @
	{0b01111110,0b00010001,0b00010001,0b00010001,0b01111110}, // A
	{0b01111111,0b01001001,0b01001001,0b01001001,0b00111110}, // B
	{0b00111110,0b01000001,0b01000001,0b01000001,0b00100010}, // C
	{0b01111111,0b01000001,0b01000001,0b01000001,0b00111110}, // D
	{0b01111111,0b01001001,0b01001001,0b01001001,0b01001001}, // E
	{0b01111111,0b00001001,0b00001001,0b00001001,0b00000001}, // F
	{0b00111110,0b01000001,0b01001001,0b01001001,0b00111010}, // G
	{0b01111111,0b00001000,0b00001000,0b00001000,0b01111111}, // H
	{0b01000001,0b01000001,0b01111111,0b01000001,0b01000001}, // I
	{0b00110000,0b01000001,0b01000001,0b00111111,0b00000001}, // J
	{0b01111111,0b00001000,0b00010100,0b00100010,0b01000001}, // K
	{0b01111111,0b01000000,0b01000000,0b01000000,0b01000000}, // L
	{0b01111111,0b00000010,0b00001100,0b00000010,0b01111111}, // M
	{0b01111111,0b00000100,0b00001000,0b00010000,0b01111111}, // N
	{0b00111110,0b01000001,0b01000001,0b01000001,0b00111110}, // O
	{0b01111111,0b00001001,0b00001001,0b00001001,0b00000110}, // P
	{0b00111110,0b01000001,0b01010001,0b00100001,0b01011110}, // Q
	{0b01111111,0b00001001,0b00001001,0b00011001,0b01100110}, // R
	{0b01000110,0b01001001,0b01001001,0b01001001,0b00110001}, // S
	{0b00000001,0b00000001,0b01111111,0b00000001,0b00000001}, // T
	{0b00111111,0b01000000,0b01000000,0b01000000,0b00111111}, // U
	{0b00001111,0b00110000,0b01000000,0b00110000,0b00001111}, // V
	{0b00111111,0b01000000,0b00111000,0b01000000,0b00111111}, // W
	{0b01100011,0b00010100,0b00001000,0b00010100,0b01100011}, // X
	{0b00000011,0b00000100,0b01111000,0b00000100,0b00000011}, // Y
	{0b01100001,0b01010001,0b01001001,0b01000101,0b01000011}, // Z
	{0b01111111,0b01000001,0b01000001,0b00000000,0b00000000}, // [
	{0b00000010,0b00000100,0b00001000,0b00010000,0b00100000}, // '\'
	{0b00000000,0b00000000,0b01000001,0b01000001,0b01111111}, // ]
	{0b00000100,0b00000010,0b00000001,0b00000010,0b00000100}, // ^
	{0b01000000,0b01000000,0b01000000,0b01000000,0b01000000}, // _
	{0b00000000,0b00000001,0b00000010,0b00000100,0b00000000}, // `
	{0b00100000,0b01010100,0b01010100,0b01010100,0b01111000}, // a
	{0b01111111,0b01001000,0b01000100,0b01000100,0b00111000}, // 0b
	{0b00111000,0b01000100,0b01000100,0b01000100,0b00100000}, // c
	{0b00111000,0b01000100,0b01000100,0b01001000,0b01111111}, // d
	{0b00111000,0b01010100,0b01010100,0b01010100,0b00011000}, // e
	{0b00001000,0b01111110,0b00001001,0b00000001,0b00000010}, // f
	{0b00001100,0b01010010,0b01010010,0b01010010,0b00111110}, // g
	{0b01111111,0b00001000,0b00000100,0b00000100,0b01111000}, // h
	{0b00000000,0b01000100,0b01111101,0b01000000,0b00000000}, // i
	{0b00100000,0b01000000,0b01000100,0b00111101,0b00000000}, // j
	{0b01111111,0b00010000,0b00101000,0b01000100,0b00000000}, // k
	{0b00000000,0b01000001,0b01111111,0b01000000,0b00000000}, // l
	{0b01111000,0b00000100,0b00001000,0b00000100,0b01111000}, // m
	{0b01111100,0b00001000,0b00000100,0b00000100,0b01111000}, // n
	{0b00111000,0b01000100,0b01000100,0b01000100,0b00111000}, // o
	{0b01111100,0b00010100,0b00010100,0b00010100,0b00001000}, // p
	{0b00001000,0b00010100,0b00010100,0b01111100,0b00000000}, // q
	{0b01111100,0b00001000,0b00000100,0b00000100,0b00001000}, // r
	{0b01001000,0b01010100,0b01010100,0b01010100,0b00100000}, // s
	{0b00000100,0b00111111,0b01000100,0b01000000,0b00100000}, // t
	{0b00111100,0b01000000,0b01000000,0b00100000,0b01111100}, // u
	{0b00011100,0b00100000,0b01000000,0b00100000,0b00011100}, // v
	{0b00111100,0b01000000,0b00110000,0b01000000,0b00111100}, // w
	{0b01000100,0b00101000,0b00010000,0b00101000,0b01000100}, // x
	{0b00001100,0b01010000,0b01010000,0b01010000,0b00111100}, // y
	{0b01000100,0b01100100,0b01010100,0b01001100,0b01000100}, // z
	{0b00000000,0b00001000,0b00110110,0b01000001,0b00000000}, // {
	{0b00000000,0b00000000,0b01111111,0b00000000,0b00000000}, // |
	{0b00000000,0b01000001,0b00110110,0b00001000,0b00000000}, // }
	{0b00001000,0b00000100,0b00000100,0b00001000,0b00000100} // ~
}; // characters[95]
const char Alpha[6][9] PROGMEM={{0B00000000,0B00011100,0B00100010,0B00100000,0B00101110,0B00100010,0B00011100,0B00000000},
{0B00000000,0B01111100,0B01000100,0B01000100,0B01111100,0B01010000,0B01001000,0B01000100},
{0B00000000,0B01000010,0B01000010,0B01000010,0B01000010,0B01000010,0B00111100,0B00000000},
{0B00000000,0B00111100,0B00100010,0B00100010,0B00111100,0B00100000,0B00100000,0B00100000},
{0B00000000,0B00111100,0B00100010,0B00100010,0B00111100,0B00100000,0B00100000,0B00100000},
{0B00000000,0B01111110,0B01000000,0B01000000,0B01111110,0B01000000,0B01000000,0B01111110}};

const char car [][8] PROGMEM={{0B00000000,
	                           0B00000000,
	                           0B00000000,
	                           0B00111100,
	                           0B00100100,
	                           0B11100111,
	                           0B11111111,
                               0B01000010}};
							   
const char car_up [][8] PROGMEM  = {{0B00111100,
	                                 0B00100100,
	                                 0B11100111,
	                                 0B11111111,
	                                 0B01000010,
	                                 0B00000000,
	                                 0B00000000,
                                     0B00000000}};
									 
	const char 	Hindrance [][8] PROGMEM= {{0B00000000,
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000,
		0B00111100,
		0B01000010,
	0B10000001}};
			
	
		   
		   const char Road [][8] PROGMEM={{0B00000000,
			  0B00000000,
			  0B00000000,
			  0B00000000,
			  0B00000000,
			  0B00000000,
			  0B00000000,
		      0B11111111}};
	
const char Alphabet[96][8] PROGMEM={
	{
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000,    // Space
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000
	},
	
	
	{
		0B00011000,
		0B00111100,
		0B00111100,
		0B00011000, // !
		0B00011000,
		0B00000000,
		0B00011000,
		0B00000000
	},
	{
		0B00011000,
		0B00011000,
		0B00011000,
		0B00000000,
		0B00000000, // "
		0B00000000,
		0B00000000,
		0B00000000
	},
	{
		0B01101100,
		0B01101100,
		0B11111110,
		0B01101100,    // #
		0B11111110,
		0B01101100,
		0B01101100,
		0B00000000
	},
	{
		0B00011000,
		0B00011000,
		0B00011000,
		0B00000000,
		0B00000000,
		0B00000000, // '
		0B00000000,
		0B00000000
	},
	{
		0B00011000,
		0B00110000,
		0B01100000,
		0B01100000,
		0B01100000,
		0B00110000, // (
		0B00011000,
		0B00000000
	},
	{
		0B01100000,
		0B00110000,
		0B00011000,
		0B00011000,  // )
		0B00011000,
		0B00110000,
		0B01100000,
		0B00000000
	},
	{
		0B00000000,
		0B01100110,
		0B00111100,
		0B11111111,
		0B00111100, //*
		0B01100110,
		0B00000000,
		0B00000000
	},
	{
		0B00000000,
		0B00110000,
		0B00110000,
		0B11111100, // +
		0B00110000,
		0B00110000,
		0B00000000,
		0B00000000
	},
	
	{
		0B00110000,
		0B01111100,
		0B11000000,
		0B01111000,
		0B00001100, // $
		0B11111000,
		0B00110000,
		0B00000000
	},
	{
		0B00000000,
		0B11000110,
		0B11001100,
		0B00011000,
		0B00110000,  // %
		0B01100110,
		0B11000110,
		0B00000000
	},
	{
		0B01111100,
		0B11000110,
		0B11011110,
		0B11011110,
		0B11011110, // &
		0B11000000,
		0B01111000,
		0B00000000
	},
	{
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000,
		0B00110000, //,
		0B00110000,
		0B01100000
	},
	{
		0B00000000,
		0B00000000,
		0B00000000,
		0B11111100,
		0B00000000, // -
		0B00000000,
		0B00000000,
		0B00000000
	},
	{
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000, // .
		0B00000000,
		0B00110000,
		0B00110000,
		0B00000000
	},
	{
		0B00000110,
		0B00001100,
		0B00011000,
		0B00110000,// /
		0B01100000,
		0B11000000,
		0B10000000,
		0B00000000
	},
	{
		0B01111100,
		0B11000110,
		0B11001110,
		0B11011110, // 0
		0B11110110,
		0B11100110,
		0B01111100,
		0B00000000
	},
	{
		0B00110000,
		0B01110000,
		0B00110000,
		0B00110000,
		0B00110000, // 1
		0B00110000,
		0B11111100,
		0B00000000
		},{
		0B01111000,
		0B11001100,
		0B00001100,
		0B00111000,
		0B01100000,
		0B11001100,
		0B11111100,
		0B00000000
		},{
		0B01111000,
		0B11001100,
		0B00001100,
		0B00111000,
		0B00001100,
		0B11001100,
		0B01111000,
		0B00000000
		},{
		0B00011100,
		0B00111100,
		0B01101100,
		0B11001100,
		0B11111110,
		0B00001100,
		0B00011110,
		0B00000000
		},{
		0B11111100,
		0B11000000,
		0B11111000,
		0B00001100,
		0B00001100,
		0B11001100,
		0B01111000,
		0B00000000
		},{
		0B00111000,
		0B01100000,
		0B11000000,
		0B11111000,
		0B11001100,
		0B11001100,
		0B01111000,
		0B00000000
		},{
		0B11111100,
		0B11001100,
		0B00001100,
		0B00011000,
		0B00110000,
		0B00110000,
		0B00110000,
		0B00000000
		},{
		0B01111000,
		0B11001100,
		0B11001100,
		0B01111000,
		0B11001100,
		0B11001100,
		0B01111000,
		0B00000000
		},{
		0B01111000,
		0B11001100,
		0B11001100,
		0B01111100,
		0B00001100,
		0B00011000,
		0B01110000,
		0B00000000
	},
	{
		0B00000000,
		0B00110000,
		0B00110000,
		0B00000000,
		0B00000000,
		0B00110000, //:
		0B00110000,
		0B00000000
	},
	{
		0B00000000,
		0B00110000,
		0B00110000,
		0B00000000,
		0B00000000, //;
		0B00110000,
		0B00110000,
		0B01100000
	},
	{
		0B00011000,
		0B00110000,
		0B01100000,
		0B11000000, //<
		0B01100000,
		0B00110000,
		0B00011000,
		0B00000000
	},
	{
		0B00000000,
		0B00000000,
		0B11111100,
		0B00000000, //=
		0B00000000,
		0B11111100,
		0B00000000,
		0B00000000
	},
	{
		0B01100000,
		0B00110000,
		0B00011000,
		0B00001100, //>
		0B00011000,
		0B00110000,
		0B01100000,
		0B00000000
	},
	{
		0B01111000,
		0B11001100,
		0B00001100,
		0B00011000,
		0B00110000, // ?
		0B00000000,
		0B00110000,
		0B00000000
	},
	{
		0B01111100,
		0B11000110,
		0B11011110,
		0B11011110, // @
		0B11011110,
		0B11000000,
		0B01111000,
		0B00000000
	},
	
	{
		
	0B00110000,
	0B01111000,
	0B11001100,
	0B11001100,       
	0B11111100, //A
	0B11001100,
	0B11001100,
	0B00000000
	},{
	0B11111100,
	0B01100110,
	0B01100110,
	0B01111100, // B
	0B01100110,
	0B01100110,
	0B11111100,
	0B00000000
	},{
	0B00111100,
	0B01100110,
	0B11000000,
	0B11000000, // C
	0B11000000,
	0B01100110,
	0B00111100,
	0B00000000
	},{
	0B11111000,
	0B01101100,
	0B01100110,
	0B01100110,
	0B01100110, // D
	0B01101100,
	0B11111000,
	0B00000000
	},{
	0B11111110,
	0B01100010,
	0B01101000,
	0B01111000,
	0B01101000, // E
	0B01100010,
	0B11111110,
	0B00000000
	},{
	0B11111110,
	0B01100010,
	0B01101000,
	0B01111000, //F
	0B01101000,
	0B01100000,
	0B11110000,
	0B00000000
	},{
	0B00111100,
	0B01100110,
	0B11000000,
	0B11000000, //G
	0B11001110,
	0B01100110,
	0B00111110,
	0B00000000
	},{
	0B11001100,
	0B11001100,
	0B11001100,
	0B11111100, // H
	0B11001100,
	0B11001100,
	0B11001100,
	0B00000000
	},{
	0B01111000,
	0B00110000,
	0B00110000,
	0B00110000,  //I
	0B00110000,
	0B00110000,
	0B01111000,
	0B00000000
	},{
	0B00011110,
	0B00001100,
	0B00001100,
	0B00001100, // J
	0B11001100,
	0B11001100,
	0B01111000,
	0B00000000
	},{
	0B11100110,
	0B01100110,
	0B01101100,
	0B01111000, // K
	0B01101100,
	0B01100110,
	0B11100110,
	0B00000000
	},{
	0B11110000,
	0B01100000,
	0B01100000,
	0B01100000, // L
	0B01100010,
	0B01100110,
	0B11111110,
	0B00000000
	},{
	0B11000110,
	0B11101110,
	0B11111110, // M
	0B11111110,
	0B11010110,
	0B11000110,
	0B11000110,
	0B00000000
	},{
	0B11000110,
	0B11100110,
	0B11110110,
	0B11011110, // N
	0B11001110,
	0B11000110,
	0B11000110,
	0B00000000
	},{
	0B00111000,
	0B01101100,
	0B11000110,
	0B11000110,
	0B11000110, // O
	0B01101100,
	0B00111000,
	0B00000000
	},{
	0B11111100,
	0B01100110,
	0B01100110,
	0B01111100, // P
	0B01100000,
	0B01100000,
	0B11110000,
	0B00000000
	},{
	0B01111000,
	0B11001100,
	0B11001100,
	0B11001100, // Q
	0B11011100,
	0B01111000,
	0B00011100,
	0B00000000
	},{
	0B11111100,
	0B01100110,
	0B01100110, // R
	0B01111100,
	0B01101100,
	0B01100110,
	0B11100110,
	0B00000000
	},{
	0B01111000,
	0B11001100,
	0B11100000,
	0B01110000,
	0B00011100, // S
	0B11001100,
	0B01111000,
	0B00000000
	},{
	0B11111100,
	0B10110100,
	0B00110000,
	0B00110000,
	0B00110000, //T
	0B00110000,
	0B01111000,
	0B00000000
	},{
	0B11001100,
	0B11001100,
	0B11001100,
	0B11001100, //U
	0B11001100,
	0B11001100,
	0B11111100,
	0B00000000
	},{
	0B11001100,
	0B11001100,
	0B11001100, // V
	0B11001100,
	0B11001100,
	0B01111000,
	0B00110000,
	0B00000000
	},{
	0B11000110,
	0B11000110,
	0B11000110, //W
	0B11010110,
	0B11111110,
	0B11101110,
	0B11000110,
	0B00000000
	},{
	0B11000110,
	0B11000110,
	0B01101100,
	0B00111000,//x
	0B00111000,
	0B01101100,
	0B11000110,
	0B00000000
	},{
	0B11001100,
	0B11001100,
	0B11001100,
	0B01111000,
	0B00110000, //y
	0B00110000,
	0B01111000,
	0B00000000
	},{
	0B11111110,
	0B11000110,
	0B10001100,// Z
	0B00011000,
	0B00110010,
	0B01100110,
	0B11111110,
	0B00000000
	},{
	0B01111000,
	0B01100000,
	0B01100000,
	0B01100000,
	0B01100000,
	0B01100000, //[
	0B01111000,
	0B00000000
},
{
	0B11000000,
	0B01100000,
	0B00110000,
	0B00011000,
	0B00001100, // (\)
	0B00000110,
	0B00000010,
	0B00000000
},
{
	0B01111000,
	0B00011000,
	0B00011000,
	0B00011000, // ]
	0B00011000,
	0B00011000,
	0B01111000,
	0B00000000
},
{
	0B00010000,
	0B00111000,
	0B01101100,
	0B11000110,
	0B00000000, // ^
	0B00000000,
	0B00000000,
	0B00000000
},
{
	0B00000000,
	0B00000000,
	0B00000000,
	0B00000000, // _
	0B00000000,
	0B00000000,
	0B11111100,
	0B00000000
},
{
	0B00110000,
	0B00110000,
	0B00011000,
	0B00000000, // `
	0B00000000,
	0B00000000,
	0B00000000,
	0B00000000
},
	{
	0B00000000,
	0B00000000,
	0B01111000,
	0B00001100,
	0B01111100,
	0B11001100,
	0B01110110,
	0B00000000
	},{
	0B11100000,
	0B01100000,
	0B01100000,
	0B01111100,
	0B01100110,
	0B01100110,
	0B11011100,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B01111000,
	0B11001100,
	0B11000000,
	0B11001100,
	0B01111000,
	0B00000000
	},{
	0B00011100,
	0B00001100,
	0B00001100,
	0B01111100,
	0B11001100,
	0B11001100,
	0B01110110,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B01111000,
	0B11001100,
	0B11111100,
	0B11000000,
	0B01111000,
	0B00000000
	},{
	0B00111000,
	0B01101100,
	0B01100000,
	0B11110000,
	0B01100000,
	0B01100000,
	0B11110000,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B01110110,
	0B11001100,
	0B11001100,
	0B01111100,
	0B00001100,
	0B11111000
	},{
	0B11100000,
	0B01100000,
	0B01101100,
	0B01110110,
	0B01100110,
	0B01100110,
	0B11100110,
	0B00000000
	},{
	0B00110000,
	0B00000000,
	0B01110000,
	0B00110000,
	0B00110000,
	0B00110000,
	0B01111000,
	0B00000000
	},{
	0B00001100,
	0B00000000,
	0B00001100,
	0B00001100,
	0B00001100,
	0B11001100,
	0B11001100,
	0B01111000
	},{
	0B11100000,
	0B01100000,
	0B01100110,
	0B01101100,
	0B01111000,
	0B01101100,
	0B11100110,
	0B00000000
	},{
	0B01110000,
	0B00110000,
	0B00110000,
	0B00110000,
	0B00110000,
	0B00110000,
	0B01111000,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B11001100,
	0B11111110,
	0B11111110,
	0B11010110,
	0B11000110,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B11111000,
	0B11001100,
	0B11001100,
	0B11001100,
	0B11001100,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B01111000,
	0B11001100,
	0B11001100,
	0B11001100,
	0B01111000,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B11011100,
	0B01100110,
	0B01100110,
	0B01111100,
	0B01100000,
	0B11110000
	},{
	0B00000000,
	0B00000000,
	0B01110110,
	0B11001100,
	0B11001100,
	0B01111100,
	0B00001100,
	0B00011110
	},{
	0B00000000,
	0B00000000,
	0B11011100,
	0B01110110,
	0B01100110,
	0B01100000,
	0B11110000,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B01111100,
	0B11000000,
	0B01111000,
	0B00001100,
	0B11111000,
	0B00000000
	},{
	0B00010000,
	0B00110000,
	0B01111100,
	0B00110000,
	0B00110000,
	0B00110100,
	0B00011000,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B11001100,
	0B11001100,
	0B11001100,
	0B11001100,
	0B01110110,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B11001100,
	0B11001100,
	0B11001100,
	0B01111000,
	0B00110000,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B11000110,
	0B11010110,
	0B11111110,
	0B11111110,
	0B01101100,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B11000110,
	0B01101100,
	0B00111000,
	0B01101100,
	0B11000110,
	0B00000000
	},{
	0B00000000,
	0B00000000,
	0B11001100,
	0B11001100,
	0B11001100,
	0B01111100,
	0B00001100,
	0B11111000
	},{
	0B00000000,
	0B00000000,
	0B11111100,
	0B10011000,
	0B00110000,
	0B01100100,
	0B11111100,
	0B00000000
},
{
	0B00011100,
	0B00110000,
	0B00110000,
	0B11100000,
	0B00110000, // {
	0B00110000,
	0B00011100,
	0B00000000
},
{
	0B00000000,
	0B10000000,
	0B10000000,
	0B10000000,
	0B10000000, // |
	0B10000000,
	0B10000000,
	0B00000000
},

{
	0B11100000,
	0B00110000,
	0B00110000,
	0B00011100,
	0B00110000,
	0B00110000,   // }
	0B11100000,
	0B00000000
},
{
	0B01110110,
	0B11011100,
	0B00000000,
	0B00000000,
	0B00000000, // ~
	0B00000000,
	0B00000000,
	0B00000000
}
};


/*hji i ma suamhar*/ 
// Message to be displayed, stored in flash
const char message[] PROGMEM = " hi Gruppe 6 " ;

// Function Prototypes

void initSPI(void);

void writeByte(uint8_t byte);

void writeWord(uint8_t address, uint8_t data);

void initMatrix(void);

void clearMatrix(void);



void writeAlphabet(void);
void uart_outputchar (char ch );
void uart_output (char *calledstring );
void Uart_init (void ); 
char uart_getnew ();
void  Uart_readnew (char *calledstring );
void car_car () ;
void initBuffer(void);

void pushBuffer(uint8_t x);

void pushCharacter(uint8_t c);
void display_road();
void displaycar();
void pushBuffer_hindrnace (uint8_t x );
void displayMessage(const char *arrayPointer, uint16_t arraySize);
void display_Hindrance();
void displayBuffer(void);
int minuten_erzeuger (int V ,int min );
int sekunden_erzeuger( int sec  );
int  stunden_erzeuger(int sec  , int min, int stu  );
void Max (int stu );
//test