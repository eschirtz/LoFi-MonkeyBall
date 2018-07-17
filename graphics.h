/**
 * graphics.c has lots of functions
 * that take care of drawing for the 
 * monkey ball game
 **/
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__
#include "final_project_images.h"
#include "lcd.h"
#include "main.h"
#include "levels.h"
#include "fonts.h"

// COLORS
#define MENU_BG 					0x0000
#define CLR_UNSELECT			0x31E7
#define CLR_SELECT				0xF0E7
#define MB_LIGHT_GREEN		0x07E0 
#define MB_DARK_GREEN			0xB723 
#define MB_GRASS					CLR_UNSELECT //0x07E0
#define MB_FIRE						LCD_COLOR_RED
#define MB_LAVA						LCD_COLOR_ORANGE
#define MB_BALL						LCD_COLOR_WHITE
// Helpfull Numbers
#define PADDING						16
// Function Prototypes 
void draw_main_menu(void);
void draw_win(void);
void draw_lose(void);
void draw_fire_sprite(bool flag);
void draw_star_sprite(bool flag);
void draw_initial_board(uint8_t (*lvl)[15]); 
void print_initials(char initials[], uint16_t x, uint16_t y, uint16_t fg, uint16_t bg, uint16_t hc, uint8_t highlight);
void print_time(uint16_t x, uint16_t y, uint16_t time, uint16_t fg, uint16_t bg);
void select_playgame(bool);
void draw_box(uint8_t x, uint8_t y, uint8_t el);
uint16_t b2s_x(uint16_t);
uint16_t b2s_y(uint16_t);
uint16_t s2b_x(uint16_t);
uint16_t s2b_y(uint16_t);

#endif