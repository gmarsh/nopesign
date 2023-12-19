/*
 * display.c
 *
 *  Created on: Dec 10, 2023
 *      Author: Gary
 */

#include <stdint.h>
#include <stdbool.h>
#include "display.h"
#include "main.h"
#include "dfu_jump.h"
#include "eeprom.h"
#include <stm32f0xx_hal.h>

// also defined in modes_colors.c, make sure values match
#define PIXEL_COUNT 65

// GPIO defines
#define GPIO_DIM_PUSHED		(HAL_GPIO_ReadPin(DIM_GPIO_Port, DIM_Pin) == 0)
#define GPIO_MODE_PUSHED	(HAL_GPIO_ReadPin(MODE_GPIO_Port,MODE_Pin) == 0)
#define GPIO_COLOR_PUSHED	(HAL_GPIO_ReadPin(COLOR_GPIO_Port,COLOR_Pin) == 0)

// timings
#define BUTTON_DEBOUNCE_COUNT 3
// time to wait before writing new mode/color/dim to EEPROM
#define EEPROM_WRITE_DELAY 120
// amount of time to hold dim button to jump to DFU
#define DIM_DFU_HOLD 60


// === variables/buffers ===

// peripheral handles from HAL
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma1;
extern TIM_HandleTypeDef htim14;

// lookup table for dim values
// 0x80 = maximum LED brightness
#define DIM_COUNT 6
const uint8_t dim_lut[] = { 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

// "timer IRQ fired" flag
volatile bool timer_flag = false;

// configuration structure
// read/written from EEPROM
struct config_struct config;

// display state, holds data between display_update() calls
struct state_struct state;

// SPI output buffer
#define RESET_WORDS 2
#define DISPLAY_OUTPUT_BUFFER_SIZE (RESET_WORDS + (PIXEL_COUNT * 6))
uint16_t display_output_buffer[DISPLAY_OUTPUT_BUFFER_SIZE];


// == FUNCTIONS ==

// display timer callback

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == htim14.Instance) {
		timer_flag = true;
	}
}

// display code initialization

void display_init(void) {

	// zero screen
	display_zero(display_output_buffer);

	// read config structure from EEPROM
	eeprom_read((uint8_t *) &config, 0, sizeof(config));

	// check dim value is sane, zero it if it isn't
	// mode code is responsible for validating modes/colors
	if (config.dim >= DIM_COUNT) config.dim = 0;

	// start 60hz timer interrupt
	HAL_TIM_Base_Start_IT(&htim14);
}


/*
 * bool display_update(*fb, frames);
 *
 * writes *fb to the display, then sleeps for 'delay' frames while doing button debouncing and handling.
 *
 * exits early and returns true if the mode or color changed, necessitating restarting the animation.
 * If this happened, state.color_push_edge and state.mode_push_edge can be checked by the mode code.
 *
 */

bool display_update(uint32_t *fb, int delay) {

	if (delay == 0) delay++;

	bool display_write_flag = true;

	while(delay--) {

		// sleep until next ISR
		while(!timer_flag) {
			HAL_SuspendTick();
			HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
			HAL_ResumeTick();
		}
		timer_flag = false;

		// update display if requested
		if (display_write_flag) {
			display_load_tx_buffer(fb,display_output_buffer,dim_lut[config.dim]);
			display_write_flag = false;
		}

		// handle buttons, decide what to do
		if (GPIO_DIM_PUSHED) {
			if (state.dim_debounce_counter < BUTTON_DEBOUNCE_COUNT) {
				state.dim_debounce_counter++;
				if (state.dim_debounce_counter == BUTTON_DEBOUNCE_COUNT) {
					if (!state.dim_pushed) {
						config.dim++;
						if (config.dim == DIM_COUNT) config.dim = 0;
						display_write_flag = true;
						state.eeprom_write_flag = true;
					}
					state.dim_pushed = true;
				}
			} else {
				if (++state.dim_dfu_hold_counter >= DIM_DFU_HOLD) goto dfu_jump;
			}
		} else {
			state.dim_dfu_hold_counter = 0;
			if (state.dim_debounce_counter > 0) {
				state.dim_debounce_counter--;
				if (state.dim_debounce_counter == 0) state.dim_pushed = false;
			}
		}

		if (GPIO_COLOR_PUSHED) {
			if (state.color_debounce_counter < BUTTON_DEBOUNCE_COUNT) {
				state.color_debounce_counter++;
				if (state.color_debounce_counter == BUTTON_DEBOUNCE_COUNT) {
					if (!state.color_pushed) {
						state.color_push_edge = true;
						state.color_pushed = true;
						state.abort_flag = true;
					}
				}
			}
		} else {
			if (state.color_debounce_counter > 0) {
				state.color_debounce_counter--;
				if (state.color_debounce_counter == 0) state.color_pushed = false;
			}
		}

		if (GPIO_MODE_PUSHED) {
			if (state.mode_debounce_counter < BUTTON_DEBOUNCE_COUNT) {
				state.mode_debounce_counter++;
				if (state.mode_debounce_counter == BUTTON_DEBOUNCE_COUNT) {
					if (!state.mode_pushed) {
						state.mode_push_edge = true;
						state.mode_pushed = true;
						state.abort_flag = true;
					}
				}
			}
		} else {
			if (state.mode_debounce_counter > 0) {
				state.mode_debounce_counter--;
				if (state.mode_debounce_counter == 0) state.mode_pushed = false;
			}
		}

		// handle configuration write to EEPROM
		if (state.eeprom_write_flag) {
			state.eeprom_write_flag = false;
			state.eeprom_write_timeout = EEPROM_WRITE_DELAY;
		} else if (state.eeprom_write_timeout) {
			if (--state.eeprom_write_timeout == 0) {
				eeprom_write((uint8_t *) &config, 0, sizeof(config));
			}
		}

		if (state.abort_flag) return true;
	}

	return state.abort_flag;


	dfu_jump:

		display_zero(display_output_buffer);
		dfu_jump();

		while(1);

}


/*
 * display_load_tx_buffer(*src, *dest, dimlevel)
 *
 * *src = source framebuffer. Format is 32 bit 0xddRRGGBB.
 * *dest = SPI transmit buffer
 * dimlevel = global dim level
 *
 */

// nibble lookup table for assembling SPI stream - requires 3Mbit/sec SPI data rate to meet timing spec
const uint16_t disp_spi_lut[16] = {
	 0x8888, 0x888E, 0x88E8, 0x88EE, 0x8E88, 0x8E8E, 0x8EE8, 0x8EEE,
	 0xE888, 0xE88E, 0xE8E8, 0xE8EE, 0xEE88, 0xEE8E, 0xEEE8, 0xEEEE,
};

void display_load_tx_buffer(uint32_t *src, uint16_t *output_buffer, uint8_t global_dim) {

	uint32_t pixel;
	uint32_t tmp, pixel_dim;
	int i;

	// load a reset (extended 0) into the transmit buffer
	for(i=0;i<RESET_WORDS;i++) {
		*output_buffer++ = 0x0000;
	}

	for(i=0;i<PIXEL_COUNT;i++) {
		// read pixel
		pixel = (*src++);

		// extract pixel dim value
		pixel_dim = (pixel >> 24) & 0xFF;

		// pull the green value
		tmp = ((pixel >> 8) & 0xFF);
		// scale it by the pixel and global dim values, to get the G value to write to the LED
		tmp = (tmp * pixel_dim) >> 7;
		tmp = (tmp * global_dim) >> 7;
		// clip if pixel value exceeds 255
		if (tmp > 255) tmp = 255;
		// encode the high and low nibbles using the transmit LUT, and write to the output buffer.
		*output_buffer++ = disp_spi_lut[(tmp >> 4) & 0x0F];
		*output_buffer++ = disp_spi_lut[tmp & 0x0F];

		// now do the same crap for red
		tmp = ((pixel >> 16) & 0xFF);
		tmp = (tmp * global_dim) >> 7;
		tmp = (tmp * pixel_dim) >> 7;
		if (tmp > 255) tmp = 255;
		*output_buffer++ = disp_spi_lut[(tmp >> 4) & 0x0F];
		*output_buffer++ = disp_spi_lut[tmp & 0x0F];

		// and blue
		tmp = ((pixel >> 0) & 0xFF);
		tmp = (tmp * global_dim) >> 7;
		tmp = (tmp * pixel_dim) >> 7;
		if (tmp > 255) tmp = 255;
		*output_buffer++ = disp_spi_lut[(tmp >> 4) & 0x0F];
		*output_buffer++ = disp_spi_lut[tmp & 0x0F];
	}

	// wait for previous DMA transfer to finish, if it hasn't already
	while (hspi1.State == HAL_SPI_STATE_BUSY);

	// and start a new transfer.
	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *) display_output_buffer, DISPLAY_OUTPUT_BUFFER_SIZE);
}


// zeros display - does not use DMA or interrupts

void display_zero(uint16_t *output_buffer) {

	int i;

	// kill any ongoing DMA xfer
	HAL_SPI_DMAStop(&hspi1);

	// fill framebuffer with 0
	for(i=0;i<RESET_WORDS;i++) {
		*output_buffer++ = 0x0000;
	}

	for(i=0;i<(PIXEL_COUNT*6);i++) {
		*output_buffer++ = 0x8888;
	}

	HAL_SPI_Transmit(&hspi1, (uint8_t *) display_output_buffer, DISPLAY_OUTPUT_BUFFER_SIZE, 1000);
}
