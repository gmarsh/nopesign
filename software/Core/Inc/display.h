/*
 * display.h
 *
 *  Created on: Dec 10, 2023
 *      Author: Gary
 */


#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_
#include <stdbool.h>

#define MAX_MODE_COUNT 16

void display_init(void);
void display_load_tx_buffer(uint32_t *src, uint16_t *output_buffer, uint8_t global_dim);
bool display_update(uint32_t *fb, int frames);
void display_zero(uint16_t *output_buffer);

struct config_struct {
	uint8_t dim, color, mode;
	uint8_t mode_color[MAX_MODE_COUNT];
};

struct state_struct {
	int dim_debounce_counter, color_debounce_counter, mode_debounce_counter;
	int dim_dfu_hold_counter;
	int eeprom_write_timeout;
	bool abort_flag;
	bool eeprom_write_flag;
	bool dim_pushed, color_pushed, mode_pushed;
	bool mode_push_edge, color_push_edge;
};


#endif /* SRC_DISPLAY_H_ */
