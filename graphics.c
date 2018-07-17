/**
 * graphics.c has lots of functions
 * that take care of drawing for the 
 * monkey ball game
 **/
#include "graphics.h" 
/**
	* Draws the body of the main menu
	**/
void draw_main_menu(void){
	lcd_clear_screen(MENU_BG);
	lcd_draw_image(COLS/2, TITLE_WIDTH, TITLE_HEIGHT/2 + PADDING, TITLE_HEIGHT, title_image, CLR_SELECT, MENU_BG); 
	lcd_draw_image(COLS/2, BEST_TIME_W, TITLE_HEIGHT + 2.5*PADDING, BEST_TIME_H, besttime_image, LCD_COLOR_WHITE, MENU_BG); 
	lcd_draw_image(COLS/2, ENTER_W, ROWS/2 + ENTER_H, ENTER_H, enter_image, LCD_COLOR_WHITE, MENU_BG); 
	select_playgame(false); 
}
// Draw Win
void draw_win(void){
	lcd_clear_screen(MENU_BG);
	lcd_draw_image(COLS/2, SUCCESS_W, TITLE_HEIGHT/2 + PADDING/2, SUCCESS_H, success_image, CLR_SELECT, MENU_BG); 
	draw_star_sprite(true); 
	lcd_draw_image(COLS/2, TIME_W, TITLE_HEIGHT + 5*PADDING, TIME_H, time_image, CLR_SELECT, MENU_BG); 
	
	lcd_draw_image(COLS/2, BEST_TIME_W, ROWS/2 + 3*PADDING, BEST_TIME_H, besttime_image, CLR_SELECT, MENU_BG); 
	lcd_draw_image(COLS/2, RESET_W, ROWS - 2*PADDING, RESET_H, reset_image, LCD_COLOR_WHITE, MENU_BG); 
}
void draw_star_sprite(bool flag){
	if(flag)
		lcd_draw_image(COLS/2, STAR_W, TITLE_HEIGHT + 1.5*PADDING, STAR_H, star_image, LCD_COLOR_WHITE, MENU_BG); 
	else
		lcd_draw_image(COLS/2, STAR_W, TITLE_HEIGHT + 1.5*PADDING, STAR_H, star_image, CLR_SELECT, MENU_BG); 
}
// Draw Lose
void draw_lose(void){
	lcd_clear_screen(MENU_BG);
	lcd_draw_image(COLS/2, BURNT_W, BURNT_H/2 + PADDING, BURNT_H, burnt_image, CLR_SELECT, MENU_BG); 
	lcd_draw_image(COLS/2, RESET_W, ROWS - 2*PADDING, RESET_H, reset_image, LCD_COLOR_WHITE, MENU_BG); 
}
void draw_fire_sprite(bool flag){
	if(flag)
		lcd_draw_image(COLS/2, FIRE_W, ROWS/2 + PADDING, FIRE_H, fire1_image, MB_FIRE, MENU_BG); 
	else
		lcd_draw_image(COLS/2, FIRE_W, ROWS/2 + PADDING, FIRE_H, fire2_image, MB_FIRE, MENU_BG); 
}
 /** 
	*  Draws the initial screen for the game
	*/
void draw_initial_board(uint8_t (*lvl)[LVL_W]){
	// Draw Screen
	int i,j; // pxl index
	for(i=0; i<LVL_H; i++){
		for(j=0; j<LVL_W; j++){
			switch(lvl[i][j]){
				case 0: 
					lcd_draw_image(b2s_x(j), IMG_WIDTH, b2s_y(i), IMG_HEIGHT, grass_image, MB_GRASS, MB_GRASS); 
					break;
				case 1: 
					lcd_draw_image(b2s_x(j), IMG_WIDTH, b2s_y(i), IMG_HEIGHT, lava_image, LCD_COLOR_ORANGE, LCD_COLOR_RED); 
					break; 
				case 2: 
					lcd_draw_image(b2s_x(j), IMG_WIDTH, b2s_y(i), IMG_HEIGHT, small_fire1_image, LCD_COLOR_ORANGE, MB_GRASS); 				
					break; 
				case 3: 
					lcd_draw_image(b2s_x(j), IMG_WIDTH, b2s_y(i), IMG_HEIGHT, winzone_image, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
					break;
				default:
					printf("Error at draw_initial_board()\n"); 
					break;
			}
		}
	}
}
 /** 
	*  Draws the initial screen for the game
	*/
void draw_box(uint8_t x, uint8_t y, uint8_t el){
			switch(el){
				case 0: 
					lcd_draw_image(b2s_x(x), IMG_WIDTH, b2s_y(y), IMG_HEIGHT, grass_image, MB_GRASS, MB_GRASS); 
					break;
				case 1: 
					lcd_draw_image(b2s_x(x), IMG_WIDTH, b2s_y(y), IMG_HEIGHT, lava_image, LCD_COLOR_RED, LCD_COLOR_ORANGE); 
					break; 
				case 2: 
					lcd_draw_image(b2s_x(x), IMG_WIDTH, b2s_y(y), IMG_HEIGHT, small_fire1_image, LCD_COLOR_ORANGE, LCD_COLOR_GREEN); 				
					break; 
				case 3: 
					lcd_draw_image(b2s_x(x), IMG_WIDTH, b2s_y(y), IMG_HEIGHT, winzone_image, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
					break;
				default:
					printf("Error at draw_initial_board()\n"); 
					break;
			}
}

// Highlights or unhighlights the start button
void select_playgame(bool select){
	if(select){
		lcd_draw_image(COLS/2, PLAY_W, ROWS - PLAY_H/2 - PADDING, PLAY_H, play_image, CLR_SELECT, MENU_BG); 
	}else{
		lcd_draw_image(COLS/2, PLAY_W, ROWS - PLAY_H/2 - PADDING, PLAY_H, play_image, CLR_UNSELECT, MENU_BG); 
	}
}
// Prints out the time passed in 
void print_time(uint16_t x, uint16_t y, uint16_t time, uint16_t fg, uint16_t bg){
	char snum[3]; 
	sprintf(snum, "%d", time); 
	if(time < 100 && time >= 10){
		char tmp = snum[1]; 
		snum[2] = tmp; 
		tmp = snum[0]; 
		snum[1] = tmp; 
		snum[0] = '0'; // ZeroPad for two digit numbers
	}
	if(time < 10){
		snum[2] = snum[0]; 
		snum[1] = '0'; 
		snum[0] = '0'; // ZeroPad for one digit numbers
	}
	print_initials(snum, x, y, fg, bg, bg, 0);   
}
// Prints out initials centered about x,y
void print_initials(char initials[] , uint16_t x, uint16_t y, uint16_t fg, uint16_t bg, uint16_t hc, uint8_t highlight){
	// highlight 0 is no highlight, then 1 2 or 3 for each initial
	uint16_t curr_fg = fg; // holds current fg color
	uint8_t off = FONT_WIDTH / 2 + 4; // Centers on width
	uint8_t SPACE = FONT_WIDTH - 8; // Spacing between letters 8
	// Print 1st Initial
	if(highlight == 1){
		curr_fg = hc; 
	}
	lcd_print_character(x - SPACE - off , y, curr_fg, bg, initials[0]);
	curr_fg = fg; // rest fg color
	// Print 2nd Initial
	if(highlight == 2){
		curr_fg = hc; 
	}
	lcd_print_character(x - off, y, curr_fg, bg, initials[1]);
	curr_fg = fg; // rest fg color
	// Print 3rd Initial
	if(highlight == 3){
		curr_fg = hc; 
	}
	lcd_print_character(x + SPACE - off, y, curr_fg, bg, initials[2]);
	curr_fg = fg; // rest fg color
}
// Convert board coordinates (16x16 blocks) to screen coordinates 
uint16_t b2s_x(uint16_t x){
	return x*IMG_WIDTH + IMG_WIDTH/2; 
}
uint16_t b2s_y(uint16_t y){
	return y*IMG_HEIGHT + IMG_HEIGHT/2;
}
// Convert screen coordinates to board coordinates (16x16 blocks) 
uint16_t s2b_x(uint16_t x){  
	uint8_t coord = x / IMG_WIDTH; 
	// Bounds Check
	coord = (int)coord < 0 ? 0 : coord;
	coord = (int)coord >= LVL_W ? LVL_W - 1 : coord;
	return coord; 
}
uint16_t s2b_y(uint16_t y){
	uint8_t coord = y / IMG_HEIGHT; 
	// Bounds Check
	coord = (int)coord < 0 ? 0 : coord;
	coord = (int)coord >= LVL_H ? LVL_H - 1 : coord; 
	return coord;
}