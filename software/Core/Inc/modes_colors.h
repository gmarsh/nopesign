/*
 * colors_animations.h
 *
 *  Created on: Dec 14, 2023
 *      Author: Gary
 */

#ifndef INC_MODES_COLORS_H_
#define INC_MODES_COLORS_H_

#define MAX_COLORS_PER_MODE 16

#include <stdint.h>
#include <stdbool.h>

struct mode_struct {
	const void (*func)(void);
	const uint8_t colorcount;
	const uint32_t *colors[MAX_COLORS_PER_MODE];
};

void modes_run(void);

void mode_static(void);
void mode_sackville(void);
void mode_fade_all(void);
void mode_twinkle(void);
void mode_spell_it_out(void);
void mode_fade_and_flash(void);
void mode_hue(void);
void mode_twinkle_fade(void);
void mode_write_in(void);

uint32_t color_rotate(uint32_t value);
uint32_t color_dim(uint32_t value, int dimvalue);
uint32_t color_rotate_fine(uint32_t value, int shift);
void color_rotate_fine_fb(uint32_t *fbin, uint32_t *fbout, int shift);

void color_load(const uint32_t *readptr, uint32_t *framebuffer);
void color_fade(uint32_t *fb1, uint32_t *fb2, uint32_t *fb3, int fb2_amount);
void color_twinkle(uint32_t *fbin, uint32_t *fbout, uint32_t *seed, int twinkle_amount);

#endif /* INC_MODES_COLORS_H_ */
