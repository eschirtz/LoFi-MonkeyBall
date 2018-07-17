#ifndef __ACCEL_H__
#define __ACCEL_H__

#include "spi.h"
#include "gpio_port.h"
#include "spi_select.h"
#include "main.h"

// Custom Macros
#define   ACCEL_CLK_PIN     		 PA2
#define   ACCEL_MISO_PIN    		 PA4
#define   ACCEL_MOSI_PIN    		 PA5
#define   ACCEL_GPIO_BASE   		 GPIOA_BASE
#define   ACCEL_SPI_CLK_PCTL_M   GPIO_PCTL_PA2_M
#define   ACCEL_SPI_MISO_PCTL_M  GPIO_PCTL_PA4_M
#define   ACCEL_SPI_MOSI_PCTL_M  GPIO_PCTL_PA5_M

#define   ACCEL_CLK_PIN_PCTL		 GPIO_PCTL_PA2_SSI0CLK
#define   ACCEL_MISO_PIN_PCTL    GPIO_PCTL_PA4_SSI0RX
#define   ACCEL_MOSI_PIN_PCTL    GPIO_PCTL_PA5_SSI0TX

#define   ACCEL_CS_BASE    		 GPIOA_BASE
#define   ACCEL_CS_PIN      	 PA3
#define   ACCEL_CS_PORT     	 GPIOA //might not need

#define 	ACCEL_INT1_CTRL 			0x0D
#define 	ACCEL_CTRL1_XL				0x10
#define 	ACCEL_CTRL2_G 				0x11
#define 	ACCEL_CTRL5_C 				0x14

#define		ACCEL_SPI_BASE				SSI0_BASE 

#define		ACCEL_INTERRUPT_BASE 	GPIOD_BASE
#define		ACCEL_PD3							0x8 // Pin 3


// Function Declarations
/**
	* Initializes Hardware
	**/
void accel_init(void);
uint8_t accel_read(uint8_t reg); 
void accel_CSN_high(void);
void accel_CSN_low(void);


#endif 