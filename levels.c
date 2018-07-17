#include "levels.h"
// Each level object is used for both rendering and logic
// Level Data Structure
// Each level is a 2d array of uint8_t
// 0: PATH
// 1: LAVA
// 2: Obstical
// 3: WinZone
//
// BOARD IS INDEXED "BACKWARDS"! lvl[y][x] 
//
// Helpful elements to build levels
// {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
// {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
uint8_t lvl_0[LVL_H][LVL_W] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

uint8_t lvl_1[LVL_H][LVL_W] = {
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{3,3,3,3,3,3,3,1,1,1,1,1,1,1,1},
	{3,3,3,3,3,3,3,1,1,1,1,1,1,1,1},
	{3,3,3,3,3,3,3,1,1,1,1,1,1,1,1},
};

uint8_t lvl_2[LVL_H][LVL_W] = {
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,2,0,0,0,0,0,1,1},
	{0,0,0,0,0,0,0,2,0,0,0,0,0,1,1},
	{0,0,0,0,0,0,0,2,0,0,0,0,0,1,1},
	{0,0,1,1,1,0,1,1,1,1,1,0,0,1,1},
	{0,0,1,1,1,0,1,1,1,1,1,0,2,1,1},
	{0,0,1,1,1,0,1,1,1,1,1,0,0,1,1},
	{2,2,1,1,1,0,1,1,1,1,1,0,0,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
	{0,0,0,0,0,0,0,1,1,1,1,0,0,1,1},
	{0,0,2,0,0,0,0,1,1,1,1,0,0,1,1},
	{0,0,0,0,0,2,0,1,1,1,1,0,0,1,1},
	{0,2,0,0,0,0,0,1,1,1,1,0,0,1,1},
	{3,3,3,3,3,3,3,1,1,1,1,0,0,1,1},
	{3,3,3,3,3,3,3,2,2,2,1,0,0,1,1},
	{3,3,3,3,3,3,3,0,0,0,0,0,0,1,1},
};

uint8_t lvl_3[LVL_H][LVL_W] = {
	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,1,1,1,1,1,1,0,0,1,1},
	{0,0,0,0,0,1,1,1,1,1,1,0,0,1,1},
	{1,0,0,0,0,1,1,1,1,1,1,0,0,1,1},
	{1,0,0,0,0,2,0,0,0,0,1,0,0,1,1},
	{1,0,0,0,0,2,0,0,0,0,1,0,0,1,1},
	{0,0,0,0,1,1,1,0,0,0,0,0,0,1,1},
	{0,0,0,0,1,1,1,0,0,0,0,0,0,1,1},
	{0,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
	{0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
	{1,1,1,1,1,1,1,1,1,0,0,0,2,0,0},
	{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{3,3,3,3,3,3,0,0,0,0,0,1,1,1,1},
	{3,3,3,3,3,3,0,2,0,0,0,1,1,1,1},
	{3,3,3,3,3,3,0,0,0,0,0,1,1,1,1},
};