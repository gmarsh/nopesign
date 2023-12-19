/*
 * eeprom.c
 *
 * quick'n'dirty 24C0x I2C read/write functions
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stm32f0xx_hal.h>
#include "eeprom.h"

// 7-bit, right justified  EEPROM address
#define EEPROM_ADDRESS 0x50
// EEPROM maximum page program time
#define EEPROM_TWR_MS 5

// I2C1 HAL handle
extern I2C_HandleTypeDef hi2c1;

void eeprom_read(uint8_t *readbuffer, uint8_t internal_address, int bytecount) {
	HAL_I2C_Mem_Read(&hi2c1, (EEPROM_ADDRESS << 1), internal_address, I2C_MEMADD_SIZE_8BIT, readbuffer, bytecount, HAL_MAX_DELAY);
}


void eeprom_write(uint8_t *writebuffer, uint8_t internal_address, int bytecount) {

	int page_write_bytes;

	// write pages at a time
	while(bytecount) {
		// calculate how many bytes to write to the current page
		page_write_bytes = 16 - (internal_address & 0x0F);
		if (bytecount < page_write_bytes) page_write_bytes = bytecount;

		// page program
		HAL_I2C_Mem_Write(&hi2c1,(EEPROM_ADDRESS << 1), internal_address, I2C_MEMADD_SIZE_8BIT, writebuffer, page_write_bytes, HAL_MAX_DELAY);

		// increment addresses and decrement count
		writebuffer = &writebuffer[page_write_bytes];
		internal_address += page_write_bytes;
		bytecount -= page_write_bytes;

		// if we're programming another page, wait page program time
		if (bytecount) HAL_Delay(EEPROM_TWR_MS);
	}
}

