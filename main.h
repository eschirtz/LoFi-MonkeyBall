// Copyright (c) 2015-16, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "TM4C123.h"
#include "driver_defines.h"
#include "gpio_port.h"
#include "serial_debug.h"
#include "validate.h"
#include "lcd.h"
#include "timers.h"
#include "ps2.h"
#include "launchpad_io.h"
#include "final_project_images.h"
#include "accel.h"
#include "eeprom.h"
#include "graphics.h"
#include "ft6x06.h"
#include "levels.h"
#include "ball.h"
#include "fonts.h"

///// CUSTOM MACROS /////
#define	BG_COLOR					0x000000 	// Black 
#define	LED_PINS					0xA				// Combination Red and Green pins
#define RED_PIN						0x2				// Red led pin mask
#define GREEN_PIN					0x8				// Green led pin mask
#define	SW_PIN						0x10			// Pin number for switch
#define	RED_TOGGLE_MAX		20				// Number of ISR executions before toggle
#define	GREEN_TOGGLE_MAX	20				// 'ditto'
#define ADC_QUARTER				1024			// 1/4 the max
#define ADC_THREE_QUARTER	3072			// 3/4 the max 
#define	HIGH_SCORE_ADDR		256				// The address for eeprom to read from for high score
#define HIGH_SCORE_NUM_BYTES 	4			// [First, Middle, Last, Score]
#define TIMER_DELAY						5			// Scale timerB to get 100ms event
#define ABC_SIZE					26				// Letters in the alphabet
///////////////////////////////

extern void initialize_serial_debug(void);

///////////////////////////////
// Declare any custom types //
/////////////////////////////
typedef enum {IDLE_lr, RGHT, LFT} left_right_t;
typedef enum {IDLE_ud, UP, DOWN} up_down_t;

// Holds user data
typedef struct{
	char initials[3];
	uint8_t time; 
}score;

// State for data entry
typedef enum{
	ONE_SELECTED,
	TWO_SELECTED,
	THREE_SELECTED,
	PLAY_GAME
}inital_sel;

//struct for debouncing SW1
typedef enum{
	DEBOUNCE_ONE,
	DEBOUNCE_1ST_ZERO,
	DEBOUNCE_2ND_ZERO,
	DEBOUNCE_PRESSED
} DEBOUNCE_STATES;

typedef struct{
	uint16_t x; 
	uint16_t y;
}pos; 
/**
typedef struct{
	int16_t x; 
	int16_t y; 
}tilt; 
**/


//////////////////////////////
// Function Prototype Next //
////////////////////////////


#endif
