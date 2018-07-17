#ifndef __HW3_IMAGES_H__
#define __HW3_IMAGES_H__

#include <stdint.h>
#include <stdio.h>

#define       PLANE_HEIGHT 	58	// height of plane image in pixels
#define       PLANE_WIDTH  	47	// width of plane image in pixels
#define				MISSLE_HEIGHT	11	// height of missle image in pixels
#define				MISSLE_WIDTH	7		// width of missle image in pixels

extern const uint8_t planeBitmap[];
extern const uint8_t missleBitmap[];
extern const uint8_t missleErase[];

#endif