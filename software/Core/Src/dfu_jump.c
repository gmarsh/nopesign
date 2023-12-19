/*
 * dfu_jump.c
 *
 *  Created on: Dec 10, 2023
 *      Author: Gary
 */

#include <stdint.h>
#include <stm32f0xx_hal.h>
#include "dfu_jump.h"

//for ST32F042
#define BOOTLOADER_START_ADDR 0x1fffc400

void dfu_jump(void) {

	int i;
	void (*SysMemBootJump)(void);

	// disable interrupts
	__disable_irq();

	// deinit clock system
	HAL_RCC_DeInit();

	// disable systick
	SysTick->CTRL = 0;

	// clear all interrupt enable bits
	for (i = 0; i < sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0]); i++) {
		NVIC->ICER[i] = 0xFFFFFFFF;
		NVIC->ICPR[i] = 0xFFFFFFFF;
	}

	// re-enable IRQs
	__enable_irq();

	// do some voodoo to jump into bootloader
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BOOTLOADER_START_ADDR + 4)));
	__set_MSP(*(uint32_t *)BOOTLOADER_START_ADDR);
	SysMemBootJump();

	while(1);

}

