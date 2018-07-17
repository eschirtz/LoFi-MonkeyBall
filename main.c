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

#include "main.h"


char individual_1[] = "Eric Schirtzinger";
char individual_2[] = "Carly Krebs";

///////////////////////////
// Global declared next //
/////////////////////////
// TIMER FLAGS
volatile bool timerA = false, timerB = false; 
volatile bool debounce_int = false;
volatile bool joystick_int = false;
volatile bool sw1_pressed	 = false; 
volatile bool accel_flag = true; //to read the first time
// ADC Globals
volatile bool start_conversion = false; 
volatile bool adc_ready = false; 
uint32_t ps2_x, ps2_y; 
left_right_t joystick_left_right;
up_down_t joystick_up_down;
// EEPROM 
score high_score; 
score current_score; 
// Initial select
inital_sel main_menu_select;  
inital_sel last_select;  
// Accel and Touchscreen
pos touchscreen; 
bool new_touch = false; 
// Tilt in signed values
tilt accel_tilt; 
// Ball 
ball_t ball;
// Initials
char abc[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int8_t initial_index_0 = 0;
int8_t initial_index_1 = 0;
int8_t initial_index_2 = 0;
uint8_t highlight = 0; // The highlight number for initials
bool update_char = true;
// Level
uint8_t (*lvl)[15];
// Sprite flag
bool sprite; 

//*****************************************************************************
//*****************************************************************************
void EnableInterrupts(void){
	__asm {
		CPSIE I
	}
}
void DisableInterrupts(void){
	__asm {
		CPSID I
	}
}
/**
	* Set up GPIOF for SW1 to set an interrupt
 **/
void sw1_initialize(void){
		GPIOA_Type *gpio;
	// Configure interrupts
		gpio_enable_port(GPIOF_BASE); 
		gpio_config_digital_enable(GPIOF_BASE, SW_PIN); 
		gpio_config_enable_input(GPIOF_BASE, SW_PIN); 
		// 
		gpio = (GPIOA_Type*)GPIOF_BASE; 
		gpio->IM &= ~SW_PIN; // Clear IME field
		gpio->IS &= ~SW_PIN; //Interrupt Sense
		gpio->IBE &= ~SW_PIN; // Set to be controlled by GPIOIEV
		gpio->IEV |= SW_PIN; // Set pin to trigger mask
		gpio->IM |= SW_PIN; 	// Re-enable
		gpio->ICR |= SW_PIN; // Clear any interrupts
		
		NVIC_SetPriority(GPIOF_IRQn, 1); 
		NVIC_EnableIRQ(GPIOF_IRQn);
}

//*****************************************************************************
//*****************************************************************************

/**
	* Responsible for initializing the hardware and kicking off
	* timers and configuring gpio pins. 
 **/
void initialize_hardware(void)
{
	uint32_t mode; 
	TIMER0_Type *gp_timer;
	uint16_t TIMER0A_ticks, TIMER0B_ticks; 
	uint8_t TIMER0A_prescale, TIMER0B_prescale; 
	// Spi variables
	uint8_t null; 
	initialize_serial_debug();
	// Setup lcd GPIO, config the screen, and clear it
	lcd_config_screen(); 
	// Config Debug 
	DisableInterrupts();
	init_serial_debug(true, true); 
	EnableInterrupts();
	// Setup the timers
	// Both timers config
	mode = TIMER_TAMR_TAMR_PERIOD | TIMER_TBMR_TBMR_PERIOD;
	gp_timer_config_16(TIMER0_BASE, mode, false, true);  
	gp_timer = (TIMER0_Type *)TIMER0_BASE; 
	// Stops both timers (A and B)
  gp_timer->CTL &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN);
	// Timer A setup (10ms)
	// Sets the interval
	TIMER0A_ticks = 33333;
	TIMER0A_prescale = 15; 
	gp_timer->TAILR = TIMER0A_ticks;
	gp_timer->TAPR = TIMER0A_prescale; 
	// Clears any timer A timeout
	gp_timer->ICR |= TIMER_ICR_TATOCINT; 
	// Starts only timer A
	gp_timer->CTL |= TIMER_CTL_TAEN;
	// Config Interrupts
	NVIC_SetPriority(TIMER0A_IRQn, 0); 
	NVIC_EnableIRQ(TIMER0A_IRQn);	
	// Timer B setup (20ms)
	// Sets the interval
	TIMER0B_ticks = 32258; 
	TIMER0B_prescale = 31; 
	gp_timer->TBILR = TIMER0B_ticks;
	gp_timer->TBPR = TIMER0B_prescale;	
	// Clears any timer B timeout
	gp_timer->ICR |= TIMER_ICR_TBTOCINT; 
	// Starts only timer B
	gp_timer->CTL |= TIMER_CTL_TBEN;
	// Config Interrupts 
	NVIC_SetPriority(TIMER0B_IRQn, 0); 
	NVIC_EnableIRQ(TIMER0B_IRQn); 
	//// setup GPIO for SW1
	sw1_initialize(); 
	//// Setup ADC to convert on PS2 joystick using SS2 and interrupts
	ps2_initialize_hw3(); 
	initialize_adc_hw3(); 
	// Setup accelerometor 
	accel_init(); 
	// Initialize EEPROM
	eeprom_init(); 
	// Initialize Touch Screen
	ft6x06_init(); 
}
//*****************************************************************************
//*****************************************************************************
/**
	* Update the main menu 
	* struct with the current Joystick values
 **/
void check_joystick_update(void){
		int8_t initial_index;
		if(main_menu_select == ONE_SELECTED){
			initial_index = initial_index_0;
			highlight = 1; // First Initial 
		}else if(main_menu_select == TWO_SELECTED){
			initial_index = initial_index_1;
						highlight = 2; // Second Initial
		}else if(main_menu_select == THREE_SELECTED){
			initial_index = initial_index_2;
		  highlight = 3; // Third Initial
		}
		if(joystick_up_down == UP){
			initial_index = (initial_index - 1) % ABC_SIZE;
			if(initial_index < 0){
				initial_index = ABC_SIZE - 1;
			}
			update_char = true;
		}else if(joystick_up_down == DOWN){
			initial_index = (initial_index + 1) % ABC_SIZE;
			if(initial_index > ABC_SIZE - 1){
				initial_index = 0; // No Highlight 
			}
		update_char = true;
		}else{ //idle
			update_char = false;
		}
		if(main_menu_select == ONE_SELECTED){
			initial_index_0 = initial_index;
		}else if(main_menu_select == TWO_SELECTED){
			initial_index_1 = initial_index;
		}else if(main_menu_select == THREE_SELECTED){
			initial_index_2 = initial_index;
		}
	}

//*****************************************************************************
//*****************************************************************************
/**
	* Checks touchscreen for a touch event
	* updates the struct
 **/
	void read_touchscreen(void){
		uint8_t touch_points;
		touch_points = ft6x06_read_td_status();
		if(touch_points > 0){
			new_touch = true; 
			touchscreen.x = ft6x06_read_x();
			touchscreen.y = ft6x06_read_y();
		}
	}
//*****************************************************************************
//*****************************************************************************
/**
	* Read in accelerometer data
	* interupt driven
 **/
	void read_accel(void){
		if(accel_flag){
			uint8_t accel_axl = accel_read(0xA8);
			uint8_t accel_axh = accel_read(0xA9);
			uint8_t accel_ayl = accel_read(0xAA);
			uint8_t accel_ayh = accel_read(0xAB);
			accel_tilt.x = (int16_t)(accel_axl | (accel_axh << 8)); 
			accel_tilt.y = (int16_t)(accel_ayl | (accel_ayh << 8)); 
			accel_flag = false;
		}
	}
	
//*****************************************************************************
//*****************************************************************************
/**
	* Does the analog conversion 
	* 
 **/
	void convert_ps2(void){
		/**
			*	ADC conversion, kick off and read
		 **/
		if(start_conversion){
			// Kick off the ADC conversions
			start_adc_conversion(PS2_X_ADC_CHANNEL);
			// Reset flag
			start_conversion = false; 
		}
		// Read values when they are ready
		if(adc_ready){
			ps2_x = get_adc_value_hw3(); 
			ps2_y = get_adc_value_hw3(); 
			adc_ready = false; 
			// Set direction for LR
			if(ps2_x > ADC_THREE_QUARTER){ 
				joystick_left_right = LFT;
			}
			else if(ps2_x < ADC_QUARTER)
				joystick_left_right = RGHT; 
			else
				joystick_left_right = IDLE_lr;
			// Set direction for UD
			if(ps2_y > ADC_THREE_QUARTER)
				joystick_up_down = UP;
			else if(ps2_y < ADC_QUARTER)
				joystick_up_down = DOWN; 
			else
				joystick_up_down = IDLE_ud; 			
		}
	}
//*****************************************************************************
//*****************************************************************************
/**
	* Logic for user input states
**/
bool check_initial_sel_state(){
	switch (main_menu_select){
		case ONE_SELECTED:
			if(joystick_left_right == RGHT){
				main_menu_select = TWO_SELECTED;
			}else{
				main_menu_select = ONE_SELECTED;
			}
		break; 
		case TWO_SELECTED:
			if(joystick_left_right == RGHT){
				main_menu_select = THREE_SELECTED;
			}else if(joystick_left_right == LFT){
				main_menu_select = ONE_SELECTED;
			}else{
				main_menu_select = TWO_SELECTED;
			}
		break;
		case THREE_SELECTED:
			if(joystick_left_right == LFT){
				main_menu_select = TWO_SELECTED;
			}else if(joystick_left_right == RGHT){
				main_menu_select = PLAY_GAME;
			}else{
				main_menu_select = THREE_SELECTED;
			}
		break;
		case PLAY_GAME:
			if(joystick_left_right == LFT){
				main_menu_select = THREE_SELECTED;
			}else{
				main_menu_select = PLAY_GAME;
			}
		break;
		default:
			main_menu_select = ONE_SELECTED; 
		break; 
	}
}
//*****************************************************************************
//*****************************************************************************
/**
	* Handler for GPIOD
	* (Accel)
 **/
void GPIOD_Handler(void){
	GPIOD->ICR |= ACCEL_PD3; 
	accel_flag = true;
	//set flag, will cause you to read the accel in main loop
}
//*****************************************************************************
//*****************************************************************************
/**
	* SW1 handler for GPIOF
	* 
 **/

void GPIOF_Handler(void){
	// SW1 button press event
	//printf("SW1 pressed/GPIOF handler hit\n");
	//if(sw1_debounce_fsm() == true){
		sw1_pressed = true; // Set flag
	//}
	GPIOF->ICR |= SW_PIN;
}
//*****************************************************************************
//*****************************************************************************
/**
	* 10ms Timer handler
	* 
 **/
void TIMER0A_Handler(void){
	TIMER0_Type *timer;
	timerA = true; // Flag up
	
	// SW1 button press event
	//if(sw1_debounce_fsm() == true){
	//	sw1_pressed = true; // Set flag
//	}
	
	// Clear TIMER0A interupt
	timer = (TIMER0_Type *)TIMER0_BASE;
	timer->ICR |= TIMER_ICR_TATOCINT; 
}
//*****************************************************************************
//*****************************************************************************
/**
	* 20ms Timer handler
	* 
 **/
void TIMER0B_Handler(void){
	TIMER0_Type *timer;
		timerB = true; // Flag up
	// Start ADC conversion
	start_conversion = true; 
	// Clear TIMER0B interupt
	timer = (TIMER0_Type *)TIMER0_BASE;
	timer->ICR |= TIMER_ICR_TBTOCINT; 
}
//*****************************************************************************
//*****************************************************************************
/**
	* ADC interrupt handler
	* 
 **/
void ADC0SS2_Handler(void){
	ADC0_Type *myADC = (ADC0_Type *)ADC0_BASE;
	// Set ready flag
	adc_ready = true; 
	// Clear interrupt status
	myADC->ISC |= ADC_ISC_IN2;  
}
//*****************************************************************************
//*****************************************************************************
/**
* Chooses a random level
 **/
void choose_level(void){
	uint8_t r;
	read_accel(); // Using to generate random number
	srand(accel_tilt.x * accel_tilt.y); 
  r = (uint8_t)rand();
	r = r % 2; //mod-ing with number of levels
	switch(r){
		case 0:
			lvl = &(lvl_2[0]);
		break;
		case 1:
			lvl = &(lvl_3[0]);
		break;
		default:
			lvl = &(lvl_0[0]);
	}
}

//*****************************************************************************
//*****************************************************************************
/**
  * This is the main method for the program
	* sets up the app then goes into an infinite loop that 
	* is interrupted to handle user events
 **/
int 
main(void)
{
	static uint32_t debounce_cnt=0;
	uint16_t hs_h; 
	uint8_t spinnerA = 0, spinnerB = 0, spinnerC = 0; // For scaling on the timers
	
  initialize_hardware();
	// Chose Level!
	choose_level();
	// Setup 
	// Read in highscore
	eeprom_byte_read(I2C1_BASE, HIGH_SCORE_ADDR, (uint8_t*)&high_score.initials[0]);  
	eeprom_byte_read(I2C1_BASE, HIGH_SCORE_ADDR + 1, (uint8_t*)&high_score.initials[1]);
	eeprom_byte_read(I2C1_BASE, HIGH_SCORE_ADDR + 2, (uint8_t*)&high_score.initials[2]);
	eeprom_byte_read(I2C1_BASE, HIGH_SCORE_ADDR + 3, (uint8_t*)&high_score.time);
	// Read in last user name
	eeprom_byte_read(I2C1_BASE, HIGH_SCORE_ADDR + 4, (uint8_t*)&current_score.initials[0]);
	eeprom_byte_read(I2C1_BASE, HIGH_SCORE_ADDR + 5, (uint8_t*)&current_score.initials[1]);
	eeprom_byte_read(I2C1_BASE, HIGH_SCORE_ADDR + 6, (uint8_t*)&current_score.initials[2]);
	// Graphics
	draw_main_menu(); 
	// High score (Magic numbers are for positioning the text)
	hs_h = ROWS/2 - 0.5*PADDING; // height for high score display
	lcd_print_character(COLS/2 - FONT_WIDTH/1.5, hs_h, LCD_COLOR_WHITE, MENU_BG, ':'); 
	print_initials(high_score.initials, 3.8*PADDING, hs_h, LCD_COLOR_WHITE, MENU_BG, CLR_SELECT, 0); 
	print_time(COLS/2 + 1.4 * FONT_WIDTH, hs_h, high_score.time, LCD_COLOR_WHITE, MENU_BG); 
	// User info
	print_initials(current_score.initials, COLS/2, ROWS/2 + 4.5*PADDING, LCD_COLOR_WHITE, MENU_BG, CLR_SELECT, highlight);
	main_menu_select = ONE_SELECTED;
	// MAIN MENU LOOP
	printf("Main Menu\n"); 
	while(1){
		convert_ps2(); 
		if(timerA){
			read_touchscreen(); 
		}
		// listen for analog input (intervallically) (only display when it's a new value)
		if(spinnerA == 0 && timerB){
			check_joystick_update();
			// Over write the current score with the new user selected values
			if(update_char){
				current_score.initials[0] = abc[initial_index_0];
				current_score.initials[1] = abc[initial_index_1];
				current_score.initials[2] = abc[initial_index_2];
				print_initials(current_score.initials, COLS/2, ROWS/2 + 4.5*PADDING, LCD_COLOR_WHITE, MENU_BG, CLR_SELECT, highlight); 
			}	
			check_initial_sel_state(); 
			// If user selected a new state, render that 
			if(main_menu_select != last_select){
				last_select = main_menu_select; 
				switch(main_menu_select){
					case ONE_SELECTED:  
						highlight = 1; 
						print_initials(current_score.initials, COLS/2, ROWS/2 + 4.5*PADDING, LCD_COLOR_WHITE, MENU_BG, CLR_SELECT, highlight); 
						break;
					case TWO_SELECTED: 
						highlight = 2; 
						print_initials(current_score.initials, COLS/2, ROWS/2 + 4.5*PADDING, LCD_COLOR_WHITE, MENU_BG, CLR_SELECT, highlight); 
						break;
					case THREE_SELECTED: 
						select_playgame(false);
						highlight = 3; 
						print_initials(current_score.initials, COLS/2, ROWS/2 + 4.5*PADDING, LCD_COLOR_WHITE, MENU_BG, CLR_SELECT, highlight);  
						break;
					case PLAY_GAME: 
						select_playgame(true); 
					  printf("PLAY GAME\n"); 
						highlight = 0; 
						print_initials(current_score.initials, COLS/2, ROWS/2 + 4.5*PADDING, LCD_COLOR_WHITE, MENU_BG, CLR_SELECT, highlight); 
						break;
					default: 
						printf("Error\n"); 
						break; 	
				}
			}			
		}
		if(sw1_pressed){
			printf("SW1 pressed\n");
		}
		// listen for sw1 to start game
		if(sw1_pressed && main_menu_select == PLAY_GAME){
			//Start game
			break; // leave while loop
		}
		// Handle touch event
		if(spinnerA == 0 && timerB && new_touch){
			bool start_touched = false; 
			printf("[Touch Event] (%d, %d)\n", s2b_x(touchscreen.x), s2b_y(touchscreen.y));
			// Big if statement to handle touch event. Not great, but it's a one time thing
			if(touchscreen.x >= PADDING && touchscreen.x <= COLS - PADDING && 
				 touchscreen.y >= ROWS - PADDING * 4 && touchscreen.y < ROWS - PADDING){
					start_touched = true; 
				}
			// Only start the game on touch if already highlighted
			if(main_menu_select == PLAY_GAME && start_touched){
				break; // start game
			}
			else if(start_touched){
				// Select start game by touch
				main_menu_select = PLAY_GAME;
			}
			new_touch = false; 
		}
		// Increment spinnerA every timerB
		if(timerB){
			spinnerA = (spinnerA + 1) % TIMER_DELAY; // check every 200 ms
		}
		// Acknowledge flags
		sw1_pressed = false;
		timerA = false; 
		timerB = false; 
	}
	// Save current user data
	eeprom_byte_write(I2C1_BASE, HIGH_SCORE_ADDR + 4, (uint8_t)current_score.initials[0]);  
	eeprom_byte_write(I2C1_BASE, HIGH_SCORE_ADDR + 5, (uint8_t)current_score.initials[1]);
	eeprom_byte_write(I2C1_BASE, HIGH_SCORE_ADDR + 6, (uint8_t)current_score.initials[2]);
	//
	printf("Game Started!\n"); 
	// Draw initial board 
	draw_initial_board(lvl);
	// Init ball
	ball.x_hres = 16 * SCALING_FACTOR; 
	ball.y_hres = 16 * SCALING_FACTOR; 
	ball.x_speed = 0; 
	ball.y_speed = 0; 
	update_ball(&ball, accel_tilt); 
	// Reset spinnerA
	spinnerA = 1; 
	current_score.time = 0;
  sprite = true; 
  // Game Loop
  while(check_ball_status(&ball, lvl) != 1
		 && check_ball_status(&ball, lvl) != 2
		 && check_ball_status(&ball, lvl) != 3	){ //while on the grass
		// Read from hardware controlls
		read_accel(); 
		convert_ps2(); 
		// Clocked on A
		if(timerA){
			if(spinnerB == 0){
				read_touchscreen();
			}				
			if(new_touch && spinnerB == 0){
				uint16_t x = s2b_x(touchscreen.x);
				uint16_t y = s2b_y(touchscreen.y);
				uint8_t el = lvl[y][x]; 
				printf("[Touch Event] (%d, %d)\n", s2b_x(touchscreen.x), s2b_y(touchscreen.y));
				if(el == 2){
					printf("Removing fire!\n"); 
					lvl[y][x] = 0x00; 
					draw_box(x, y, 0x00);
				}
			}
			new_touch = false; 
			spinnerB = (spinnerB + 1) % TIMER_DELAY / 2; 
			spinnerC = (spinnerC + 1) % 25; // 250ms for sprites
		}
		// Clocked on B
		if(timerB){
			// Update ball position
			update_ball(&ball, accel_tilt);
			// Increment every second
			if(spinnerA == 0){
				current_score.time++; 
			}
			spinnerA = (spinnerA + 1) % 50;
		}
		// Draw sprites
		if(timerA && spinnerC == 0){
			int i,j; // pxl index
			bool alternating = sprite; 
			for(i=0; i<LVL_H; i++){
				for(j=0; j<LVL_W; j++){
					switch(lvl[i][j]){ 
						case 2: 
							if(alternating){
								lcd_draw_image(b2s_x(j), IMG_WIDTH, b2s_y(i), IMG_HEIGHT, small_fire1_image, LCD_COLOR_ORANGE, MB_GRASS); 
							}else{
								lcd_draw_image(b2s_x(j), IMG_WIDTH, b2s_y(i), IMG_HEIGHT, small_fire2_image, LCD_COLOR_ORANGE, MB_GRASS); 
							}
							alternating = !alternating; 
							break; 
						default: 
							break;
					}
				}
			}
			sprite = !sprite; 
		}
		sw1_pressed = false; 
		timerA = false; // acknowledge flag
		timerB = false; // acknowledge flag
  }
	// End Screens
	// Uncomment, compile, and run to reset high score
	// eeprom_byte_write(I2C1_BASE, HIGH_SCORE_ADDR + 3, 255);
	// Game Over Screen
	if(check_ball_status(&ball, lvl) == 1 || check_ball_status(&ball, lvl) == 2){
		sprite = true; 
		// draw lose screen
		draw_lose(); 
		draw_fire_sprite(sprite); 
		sprite = !sprite; 
		printf("You Lose!\n");
		printf("Time: %d\n", current_score.time); 
		spinnerA = 0; 
		while(1){
			// Animation
			if(timerA){
				spinnerA = (spinnerA + 1) % 15; // Hits 0 every 150ms
				if(spinnerA==0){
					draw_fire_sprite(sprite); 
					sprite = !sprite;
				}
			}
			timerA = false; // Ack
		}
	}
	// Win Screen
	else{
		bool sprite = true; 
		//draw win screen
		draw_win(); 
		printf("You Win!\n"); 
		// Your Time
		printf("Time: %d\n", current_score.time); 
		hs_h = ROWS/2 + 0.8 * FONT_HEIGHT; // height for high score display
		print_time(COLS/2, hs_h, current_score.time, LCD_COLOR_WHITE, MENU_BG);
		// Best Time
		hs_h = (uint16_t)(ROWS - 1.5 * FONT_HEIGHT ); // height for high score display
		print_time(COLS/2, hs_h, high_score.time, LCD_COLOR_WHITE, MENU_BG);
		// Check if high score, if yes update
		if(current_score.time <= high_score.time){
			eeprom_byte_write(I2C1_BASE, HIGH_SCORE_ADDR, (uint8_t)current_score.initials[0]);  
			eeprom_byte_write(I2C1_BASE, HIGH_SCORE_ADDR + 1, (uint8_t)current_score.initials[1]);
			eeprom_byte_write(I2C1_BASE, HIGH_SCORE_ADDR + 2, (uint8_t)current_score.initials[2]);
			eeprom_byte_write(I2C1_BASE, HIGH_SCORE_ADDR + 3, current_score.time );
		}
		while(1){
			// Animation
			if(timerA){
				spinnerA = (spinnerA + 1) % 25; // Hits 0 every 250ms
				if(spinnerA==0){
					draw_star_sprite(sprite); 
					sprite = !sprite;
				}
			}
			timerA = false; // Ack
		}
	}
}


