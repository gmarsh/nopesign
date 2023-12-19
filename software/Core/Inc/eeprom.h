/*
 * eeprom.h
 *
 *  Created on: Dec 15, 2023
 *      Author: Gary
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

void eeprom_read(uint8_t *readbuffer, uint8_t internal_address, int bytecount);
void eeprom_write(uint8_t *writebuffer, uint8_t internal_address, int bytecount);

#endif /* INC_EEPROM_H_ */
