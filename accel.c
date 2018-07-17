/**
	* Carly and Eric
	*
	**/
	#include "accel.h"

	
	
	/** 
		* Initializes the accelerometer
		* peripheral to be able to read in data.  
		**/
	void accel_init(void){
		uint8_t tx_data[2];
		uint8_t rx_data[2];
		GPIOA_Type *gpio; 

		// Configure GPIO to talk to SPI
		gpio_enable_port(ACCEL_GPIO_BASE);
		
		// Configure SPI CLK
		gpio_config_digital_enable(ACCEL_GPIO_BASE, ACCEL_CLK_PIN);
		gpio_config_alternate_function(ACCEL_GPIO_BASE, ACCEL_CLK_PIN);
		gpio_config_port_control(ACCEL_GPIO_BASE, ACCEL_SPI_CLK_PCTL_M, ACCEL_CLK_PIN_PCTL);
	 
		// Configure SPI MISO
		gpio_config_digital_enable(ACCEL_GPIO_BASE, ACCEL_MISO_PIN);
		gpio_config_alternate_function(ACCEL_GPIO_BASE, ACCEL_MISO_PIN);
		gpio_config_port_control(ACCEL_GPIO_BASE, ACCEL_SPI_MISO_PCTL_M, ACCEL_MISO_PIN_PCTL);
	 
		// Configure SPI MOSI
		gpio_config_digital_enable(ACCEL_GPIO_BASE, ACCEL_MOSI_PIN);
		gpio_config_alternate_function(ACCEL_GPIO_BASE, ACCEL_MOSI_PIN);
		gpio_config_port_control(ACCEL_GPIO_BASE, ACCEL_SPI_MOSI_PCTL_M, ACCEL_MOSI_PIN_PCTL);
		
		// Configure CS to be a normal GPIO pin that is controlled
		// explicitly by software
		gpio_enable_port(ACCEL_CS_BASE);
		gpio_config_digital_enable(ACCEL_CS_BASE,ACCEL_CS_PIN);
		gpio_config_enable_output(ACCEL_CS_BASE,ACCEL_CS_PIN);

		// SPI Select
		initialize_spi(ACCEL_SPI_BASE, 3, 10);
		spi_select_init();  
		// Four calls to spiTx()

		// Shifting Macros by one so LSB is a 0 signaling write
		tx_data[0] = ACCEL_INT1_CTRL;
		tx_data[1] = 0x01; // Enable interrupt upon data ready
		accel_CSN_low(); 
		spiTx(ACCEL_SPI_BASE, tx_data, 2, rx_data);
		accel_CSN_high(); 
		
		tx_data[0] = ACCEL_CTRL1_XL;
		tx_data[1] = 0x53; // Setup accel for 208Hz data rate
		accel_CSN_low(); 
		spiTx(ACCEL_SPI_BASE, tx_data, 2, rx_data);
		accel_CSN_high(); 
		
		tx_data[0] = ACCEL_CTRL2_G;
		tx_data[1] = 0x52; // Setup up gyro for 416 Hz
		accel_CSN_low(); 
		spiTx(ACCEL_SPI_BASE, tx_data, 2, rx_data);
		accel_CSN_high();
		
		tx_data[0] = ACCEL_CTRL5_C;
		tx_data[1] = 0x60; // Turn rounding on 
		accel_CSN_low(); 
		spiTx(ACCEL_SPI_BASE, tx_data, 2, rx_data);
		accel_CSN_high(); 
		
		// Configure interrupts
		gpio_enable_port(ACCEL_INTERRUPT_BASE); 
		gpio_config_digital_enable(ACCEL_INTERRUPT_BASE, ACCEL_PD3); 
		gpio_config_enable_input(ACCEL_INTERRUPT_BASE, ACCEL_PD3); 
		// 
		gpio = (GPIOA_Type*)ACCEL_INTERRUPT_BASE; 
		gpio->IM &= ~ACCEL_PD3; // Clear IME field
		gpio->IS &= ~ACCEL_PD3; //Interrupt Sense
		gpio->IBE &= ~ACCEL_PD3; // Set to be controlled by GPIOIEV
		gpio->IEV |= ACCEL_PD3; // Set pin to trigger mask
		gpio->IM |= ACCEL_PD3; 	// Re-enable
		gpio->ICR |= ACCEL_PD3; // Clear any interrupts
		
		NVIC_SetPriority(GPIOD_IRQn, 1); 
		NVIC_EnableIRQ(GPIOD_IRQn);
	}
	
	/**
		* Reads and returns accelerometer data
		*
	**/
	uint8_t accel_read(uint8_t reg){

		uint8_t tx_data[2];
		uint8_t rx_data[2];
		
		accel_CSN_low();
		tx_data[0] = reg;
		spiTx(ACCEL_SPI_BASE, tx_data, 2, rx_data);
		accel_CSN_high();
		
		return rx_data[1];
	}

	/** CS Functions **/
	void accel_CSN_low(void){
		GPIOA->DATA &= ~PA3;
	}
	void accel_CSN_high(void){
		GPIOA->DATA |= PA3;
	}