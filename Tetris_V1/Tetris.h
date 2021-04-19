/*
 * IncFile1.h
 *
 * Created: 06/04/2021 19:44:16
 *  Author: dakak
 */ 


#ifndef Tetris.h
#define Tetris.h
// Festlegung der Quarzfrequenz
#ifndef F_CPU                   // optional definieren
#define F_CPU 12288000UL        // ATmega 328 mit 12,288 MHz Quarz
#endif


#include <avr/io.h>

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#define maxInUse 2 
#define SLAVE_SELECT    PORTB &= ~(1 << PB2)
#define SLAVE_DESELECT  PORTB |= (1 << PB2)
volatile unsigned long timer1_millis;

void init_millis();

unsigned long millis();

void initSPI(void);

void initTaster(void);

int readBut(void);

void writeByte(uint8_t byte);

bool check_overlap();

void check_gameover();

void gameover();

void LEDRefresh();

bool moveleft();

bool moveright();

bool space_below();

bool space_left();

bool space_left2();

bool space_left3();

bool space_right();

bool space_right2();

bool space_right3();

void maxTransferOne(uint8_t whichMax, uint8_t address, uint8_t value);

void maxTransferSingle( uint8_t whichMax, uint8_t address, uint8_t value);

void maxTransferAll(uint8_t address, uint8_t value);

void maxShutdown(uint8_t cmd) ;

void setBrightness();

void newBlock();

void updateLED();

void writeWord(uint8_t address, uint8_t data);

void initMatrix(void);

void clearMatrix(void);

#endif Tetris.h