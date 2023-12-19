/*
 * modes_colors.c
 *
 *  Created on: Dec 14, 2023
 *      Author: Gary
 */

#include <modes_colors.h>
#include <stdint.h>
#include <stdbool.h>
#include "display.h"
#include "modes_colors.h"

// make sure this matches display.c definition
#define PIXEL_COUNT 65

// externs from display.c
extern struct config_struct config;
extern struct state_struct state;

// framebuffers
uint32_t fb1[PIXEL_COUNT];
uint32_t fb2[PIXEL_COUNT];
uint32_t fb3[PIXEL_COUNT];




// framebuffer addresses
#define HALF_OVAL_COUNT 14
const uint8_t oval_top_addresses[HALF_OVAL_COUNT] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };
const uint8_t oval_bottom_addresses[HALF_OVAL_COUNT] = {64,63,62,61,60,59,58,57,56,55,54,53,52,51 };

#define OVAL_COUNT 28
const uint8_t oval_addresses[OVAL_COUNT] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,64,63,62,61,60,59,58,57,56,55,54,53,52,51 };

#define N_COUNT 10
const uint8_t n_addresses[OVAL_COUNT] = { 14,15,16,17,18,19,20,21,22,23 };
#define O_COUNT 9
const uint8_t o_addresses[OVAL_COUNT] = { 24,25,26,27,28,29,30,31,32 };
#define P_COUNT 8
const uint8_t p_addresses[OVAL_COUNT] = { 33,34,35,36,37,38,39,40 };
#define E_COUNT 10
const uint8_t e_addresses[OVAL_COUNT] = { 41,42,43,44,45,46,47,48,49,50 };

// all letters
#define LETTER_COUNT 37
const uint8_t letter_addresses[LETTER_COUNT] = { 14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50 };


// == color tables ==

const uint32_t color_black[] = {
	(PIXEL_COUNT << 24) | 0x000000,
	0	// EOF
};

// aka color_red_blue[]
const uint32_t color_classic[] = {
    // classic
    (14 << 24) | 0x0000FF,  // bottom swoosh = blue
    (10 << 24) | 0xFF0000,  // N = red
    (9 << 24) | 0xFF0000,   // O = red
    (8 << 24) | 0xFF0000,   // P = red
    (10 << 24) | 0xFF0000,  // E = red
    (14 << 24) | 0x0000FF,  // top swoosh = blue
    0   // EOF
};

const uint32_t color_red_green[] = {
    (14 << 24) | 0x00FF00,
    (37 << 24) | 0xFF0000,
    (14 << 24) | 0x00FF00,
    0   // EOF
};

const uint32_t color_red_cyan[] = {
    (14 << 24) | 0x00FFFF,
    (37 << 24) | 0xFF0000,
    (14 << 24) | 0x00FFFF,
    0   // EOF
};

const uint32_t color_blue_red[] = {
    (14 << 24) | 0xFF0000,
    (37 << 24) | 0x0000FF,
    (14 << 24) | 0xFF0000,
    0   // EOF
};

const uint32_t color_blue_yellow[] = {
	(14 << 24) | 0xFFFF00,
	(37 << 24) | 0x0000FF,
	(14 << 24) | 0xFFFF00,
	0   // EOF
};

const uint32_t color_green_red[] = {
    (14 << 24) | 0xFF0000,
    (37 << 24) | 0x00FF00,
    (14 << 24) | 0xFF0000,
    0   // EOF
};

const uint32_t color_green_blue[] = {
	(14 << 24) | 0x0000FF,
	(37 << 24) | 0x00FF00,
	(14 << 24) | 0x0000FF,
	0   // EOF
};

const uint32_t color_green_magenta[] = {
    (14 << 24) | 0xFF00FF,
    (37 << 24) | 0x00FF00,
    (14 << 24) | 0xFF00FF,
    0   // EOF
};


const uint32_t color_cyan_red[] = {
	(14 << 24) | 0xFF0000,
	(37 << 24) | 0x00FFFF,
	(14 << 24) | 0xFF0000,
	0   // EOF
};

const uint32_t color_cyan_magenta[] = {
	(14 << 24) | 0xFF00FF,
	(37 << 24) | 0x00FFFF,
	(14 << 24) | 0xFF00FF,
	0   // EOF
};

const uint32_t color_magenta_green[] = {
	(14 << 24) | 0x00FF00,
	(37 << 24) | 0xFF00FF,
	(14 << 24) | 0x00FF00,
	0   // EOF
};

const uint32_t color_magenta_yellow[] = {
	(14 << 24) | 0xFFFF00,
	(37 << 24) | 0xFF00FF,
	(14 << 24) | 0xFFFF00,
	0   // EOF
};

const uint32_t color_yellow_blue[] = {
	(14 << 24) | 0x0000FF,
	(37 << 24) | 0xFFFF00,
	(14 << 24) | 0x0000FF,
	0   // EOF
};

const uint32_t color_yellow_cyan[] = {
	(14 << 24) | 0x00FFFF,
	(37 << 24) | 0xFFFF00,
	(14 << 24) | 0x00FFFF,
	0   // EOF
};

const uint32_t color_80s[] = {
    (14 << 24) | 0x007FFF,
    (37 << 24) | 0xFF007F,
    (14 << 24) | 0x007FFF,
    0   // EOF
};

const uint32_t color_cyan_purple[] = {
    (14 << 24) | 0xFF007F,
    (37 << 24) | 0x007FFF,
    (14 << 24) | 0xFF007F,
    0   // EOF
};

const uint32_t color_rgb[] = {
    (14 << 24) | 0xFFFFFF,
    (10 << 24) | 0xFF0000,
    (9 << 24) | 0x00FF00,
    (8 << 24) | 0x0000FF,
    (10 << 24) | 0xFFFF00,
    (14 << 24) | 0xFFFFFF,
    0   // EOF
};

const uint32_t color_tricolor[] = {
	0x01FF6688,0x05FFFFFF,0x12009A49,0x0DFFFFFF,0x03FF6688,0x01FFFFFF,0x12FF6688,0x05FFFFFF,
	0x01009A49,0
};

const uint32_t color_pride[] = {
	0x01016692,0x01037E2E,0x0125901C,0x01BDD209,0x01FCEA00,0x01FFCC00,0x01FF9600,0x01FC8000,
	0x01F14000,0x01E70B00,0x02E50200,0x01E50300,0x01E91700,0x01EC2900,0x01F24800,0x01F65F00,
	0x01FB7800,0x01FE8600,0x01FE8B00,0x01FF9000,0x01FF9800,0x01FFA700,0x01FFB700,0x01FCEA00,
	0x01C8D607,0x016BAE13,0x0196C10E,0x01C8D607,0x01FAEA01,0x01FFD600,0x01FFD100,0x01FFDE00,
	0x01439D18,0x011E8D1D,0x010B8521,0x01058126,0x01016F6D,0x010058CC,0x0101639F,0x01027D32,
	0x01600FA0,0x012833D8,0x010748F8,0x01024CF9,0x01014FF3,0x010054DD,0x01034BF9,0x011D3BE3,
	0x010C45F3,0x01352BCB,0x016B0894,0x01770289,0x01780389,0x0174038C,0x015616AA,0x01183EE8,
	0x01014EF5,0x0101639F,0x01027D32,0x0125901C,0x01B1CC0A,0x01FCEA00,0x01FFCC00,0x01FF9800,
	0
};

const uint32_t color_trans[] = {
	0x01E24A60,0x01ED8A99,0x02FFFFFF,0x01FEF6F7,0x03E24A60,0x080C86F0,0x011184EA,0x01B65374,
	0x06E24A60,0x01FEF6F7,0x04FFFFFF,0x01FEF6F7,0x03E24A60,0x03FFFFFF,0x01FEF6F7,0x03E24A60,
	0x01E55B6F,0x010C86F0,0x011184EA,0x05E24A60,0x016E659E,0x01E24A60,0x060C86F0,0x01B65374,
	0x02E24A60,0x01E55B6F,0x02FFFFFF,0x01FEF6F7,0x02E24A60,0
};

const uint32_t color_rainbow_swoosh_red[] = {

	// 14 total around the rainbow
	(1 << 24) | 0xFF0000,(1 << 24) | 0xFF3F00,(1 << 24) | 0xFF7F00,(1 << 24) | 0xFFFF00,
	(1 << 24) | 0x7FFF00,(1 << 24) | 0x3FFF00,(1 << 24) | 0x00FF00,(1 << 24) | 0x00FF3F,
	(1 << 24) | 0x00FF7F,(1 << 24) | 0x00FFFF,(1 << 24) | 0x007FFF,(1 << 24) | 0x003FFF,
	(1 << 24) | 0x0000FF,(1 << 24) | 0x3F00FF,

	(37 << 24) | 0xFF0000,  // NOPE = red

	// 14 total around the rainbow
	(1 << 24) | 0x00FFFF,(1 << 24) | 0x00FF7F,(1 << 24) | 0x00FF3F,(1 << 24) | 0x00FF00,
	(1 << 24) | 0x3FFF00,(1 << 24) | 0x7FFF00,(1 << 24) | 0xFFFF00,(1 << 24) | 0xFF7F00,
	(1 << 24) | 0xFF3F00,(1 << 24) | 0xFF0000,(1 << 24) | 0xFF003F,(1 << 24) | 0xFF007F,
	(1 << 24) | 0xFF00FF,(1 << 24) | 0x7F00FF,
	0   // EOF
};

const uint32_t color_rainbow_swoosh_purple[] = {
	// 14 total around the rainbow
	(1 << 24) | 0xFF0000,(1 << 24) | 0xFF3F00,(1 << 24) | 0xFF7F00,(1 << 24) | 0xFFFF00,
	(1 << 24) | 0x7FFF00,(1 << 24) | 0x3FFF00,(1 << 24) | 0x00FF00,(1 << 24) | 0x00FF3F,
	(1 << 24) | 0x00FF7F,(1 << 24) | 0x00FFFF,(1 << 24) | 0x007FFF,(1 << 24) | 0x003FFF,
	(1 << 24) | 0x0000FF,(1 << 24) | 0x3F00FF,

	(37 << 24) | 0x7F00FF,  // NOPE = blue

	// 14 total around the rainbow
	(1 << 24) | 0x00FFFF,(1 << 24) | 0x00FF7F,(1 << 24) | 0x00FF3F,(1 << 24) | 0x00FF00,
	(1 << 24) | 0x3FFF00,(1 << 24) | 0x7FFF00,(1 << 24) | 0xFFFF00,(1 << 24) | 0xFF7F00,
	(1 << 24) | 0xFF3F00,(1 << 24) | 0xFF0000,(1 << 24) | 0xFF003F,(1 << 24) | 0xFF007F,
	(1 << 24) | 0xFF00FF,(1 << 24) | 0x7F00FF,
	0   // EOF
};

const uint32_t color_rainbow_swoosh_rgb[] = {
	// 14 total around the rainbow
	(1 << 24) | 0xFF0000,(1 << 24) | 0xFF3F00,(1 << 24) | 0xFF7F00,(1 << 24) | 0xFFFF00,
	(1 << 24) | 0x7FFF00,(1 << 24) | 0x3FFF00,(1 << 24) | 0x00FF00,(1 << 24) | 0x00FF3F,
	(1 << 24) | 0x00FF7F,(1 << 24) | 0x00FFFF,(1 << 24) | 0x007FFF,(1 << 24) | 0x003FFF,
	(1 << 24) | 0x0000FF,(1 << 24) | 0x3F00FF,

	(10 << 24) | 0xFF0000,  // N = red
	(9 << 24) | 0x00FF00,   // O = green
	(8 << 24) | 0x0000FF,   // P = blue
	(10 << 24) | 0xFFFF00,  // E = yellow

	// 14 total around the rainbow
	(1 << 24) | 0x00FFFF,(1 << 24) | 0x00FF7F,(1 << 24) | 0x00FF3F,(1 << 24) | 0x00FF00,
	(1 << 24) | 0x3FFF00,(1 << 24) | 0x7FFF00,(1 << 24) | 0xFFFF00,(1 << 24) | 0xFF7F00,
	(1 << 24) | 0xFF3F00,(1 << 24) | 0xFF0000,(1 << 24) | 0xFF003F,(1 << 24) | 0xFF007F,
	(1 << 24) | 0xFF00FF,(1 << 24) | 0x7F00FF,
	0   // EOF
};


const uint32_t color_rainbow_swirl_rgb[] = {
	// 14 total around the rainbow
	0x01FF0000,0x01FFFF00,0x0100FF00,0x0100FFFF,0x010000FF,0x01FF00FF,0x01FF0000,
	0x01FFFF00,0x0100FF00,0x0100FFFF,0x010000FF,0x01FF00FF,0x01FF0000,0x01FFFF00,
	// N
	0x01FF0000,0x01FFFF00,0x0100FF00,0x0100FFFF,0x010000FF,0x01FF00FF,0x01FF0000,0x01FFFF00,0x0100FF00,0x0100FFFF,
	// O
	0x010000FF,0x01FF00FF,0x01FF0000,0x01FFFF00,0x01FF0000,0x01FFFF00,0x0100FF00,0x0100FFFF,0x010000FF,
	// P
	0x01FF00FF,0x01FF0000,0x01FFFF00,0x0100FF00,0x0100FFFF,0x010000FF,0x01FF00FF,0x01FF0000,
	// E
	0x01FFFF00,0x01FF0000,0x01FFFF00,0x0100FF00,0x0100FFFF,0x010000FF,0x01FF00FF,0x01FF0000,0x01FFFF00,0x0100FF00,
	// 14 total around the rainbow
	0x01FF0000,	0x01FFFF00,0x0100FF00,0x0100FFFF,0x010000FF,0x01FF00FF,0x01FF0000,
	0x01FFFF00,0x0100FF00,0x0100FFFF,0x010000FF,0x01FF00FF,0x01FF0000,0x01FFFF00,
	0   // EOF
};


// master table of modes
// format:
// - pointer to function for this mode
// - count of colors following
// - pointer to color structure

#define MODE_COUNT 8
const struct mode_struct modes[MODE_COUNT] = {
	{ mode_static, 12, {color_classic, color_red_green, color_green_red, color_80s, color_rgb, color_rainbow_swoosh_red, color_rainbow_swoosh_purple, color_rainbow_swoosh_rgb, color_rainbow_swirl_rgb, color_tricolor, color_pride, color_trans} },
	{ mode_twinkle, 12, {color_classic, color_red_green, color_green_red, color_80s, color_rgb, color_rainbow_swoosh_red, color_rainbow_swoosh_purple, color_rainbow_swoosh_rgb, color_rainbow_swirl_rgb, color_tricolor, color_pride, color_trans} },
	{ mode_twinkle_fade, 12, {color_classic, color_red_green, color_green_red, color_80s, color_rgb, color_rainbow_swoosh_red, color_rainbow_swoosh_purple, color_rainbow_swoosh_rgb, color_rainbow_swirl_rgb, color_tricolor, color_pride, color_trans} },
	{ mode_spell_it_out, 12, {color_classic, color_red_green, color_green_red, color_80s, color_rgb, color_rainbow_swoosh_red, color_rainbow_swoosh_purple, color_rainbow_swoosh_rgb, color_rainbow_swirl_rgb, color_tricolor, color_pride, color_trans} },
	{ mode_write_in, 12, {color_classic, color_red_green, color_green_red, color_80s, color_rgb, color_rainbow_swoosh_red, color_rainbow_swoosh_purple, color_rainbow_swoosh_rgb, color_rainbow_swirl_rgb, color_tricolor, color_pride, color_trans} },
	{ mode_fade_and_flash, 12, {color_classic, color_red_green, color_green_red, color_80s, color_rgb, color_rainbow_swoosh_red, color_rainbow_swoosh_purple, color_rainbow_swoosh_rgb, color_rainbow_swirl_rgb, color_tricolor, color_pride, color_trans} },
	{ mode_sackville, 12, {color_classic, color_red_cyan, color_green_red, color_green_magenta, color_blue_red, color_blue_yellow, color_cyan_magenta, color_cyan_red, color_magenta_yellow, color_magenta_green, color_yellow_cyan, color_yellow_blue } },
	{ mode_hue, 6, { color_classic, color_rgb, color_rainbow_swoosh_red, color_rainbow_swoosh_rgb, color_pride, color_rainbow_swirl_rgb } },
};


// dot product values for hue shift
const uint32_t hue_shift_table[256] = {
	0x3FC0000,0x3F00600,0x3E40C00,0x3D81200,0x3CC1800,0x3C01E00,0x3B42400,0x3A82A00,
	0x39C3000,0x3903600,0x3843C00,0x3784200,0x36C4800,0x3604E00,0x3545400,0x3485A00,
	0x33C6000,0x3306600,0x3246C00,0x3187200,0x30C7800,0x3007E00,0x2F48400,0x2E88A00,
	0x2DC9000,0x2D09600,0x2C49C00,0x2B8A200,0x2ACA800,0x2A0AE00,0x294B400,0x288BA00,
	0x27CC000,0x270C600,0x264CC00,0x258D200,0x24CD800,0x240DE00,0x234E400,0x228EA00,
	0x21CF000,0x210F600,0x204FC00,0x1F90200,0x1ED0800,0x1E10E00,0x1D51400,0x1C91A00,
	0x1BD2000,0x1B12600,0x1A52C00,0x1993200,0x18D3800,0x1813E00,0x1754400,0x1694A00,
	0x15D5000,0x1515600,0x1455C00,0x1396200,0x12D6800,0x1216E00,0x1157400,0x1097A00,
	0xFD8000,0xF18600,0xE58C00,0xD99200,0xCD9800,0xC19E00,0xB5A400,0xA9AA00,
	0x9DB000,0x91B600,0x85BC00,0x79C200,0x6DC800,0x61CE00,0x55D400,0x49DA00,
	0x3DE000,0x31E600,0x25EC00,0x19F200,0xDF800,0x1FE00,0x1FA02,0x1F405,
	0x1EE08,0x1E80B,0x1E20E,0x1DC11,0x1D614,0x1D017,0x1CA1A,0x1C41D,
	0x1BE20,0x1B823,0x1B226,0x1AC29,0x1A62C,0x1A02F,0x19A32,0x19435,
	0x18E38,0x1883B,0x1823E,0x17C41,0x17644,0x17047,0x16A4A,0x1644D,
	0x15E50,0x15853,0x15256,0x14C59,0x1465C,0x1405F,0x13A62,0x13465,
	0x12E68,0x1286B,0x1226E,0x11C71,0x11674,0x11077,0x10A7A,0x1047D,
	0xFE80,0xF883,0xF286,0xEC89,0xE68C,0xE08F,0xDA92,0xD495,
	0xCE98,0xC89B,0xC29E,0xBCA1,0xB6A4,0xB0A7,0xAAAA,0xA4AD,
	0x9EB0,0x98B3,0x92B6,0x8CB9,0x86BC,0x80BF,0x7AC2,0x74C5,
	0x6EC8,0x68CB,0x62CE,0x5CD1,0x56D4,0x50D7,0x4ADA,0x44DD,
	0x3EE0,0x38E3,0x32E6,0x2CE9,0x26EC,0x20EF,0x1AF2,0x14F5,
	0x0EF8,0x08FB,0x02FE,0x800FE,0x1400FB,0x2000F8,0x2C00F5,0x3800F2,
	0x4400EF,0x5000EC,0x5C00E9,0x6800E6,0x7400E3,0x8000E0,0x8C00DD,0x9800DA,
	0xA400D7,0xB000D4,0xBC00D1,0xC800CE,0xD400CB,0xE000C8,0xEC00C5,0xF800C2,
	0x10400BF,0x11000BC,0x11C00B9,0x12800B6,0x13400B3,0x14000B0,0x14C00AD,0x15800AA,
	0x16400A7,0x17000A4,0x17C00A1,0x188009E,0x194009B,0x1A00098,0x1AC0095,0x1B80092,
	0x1C4008F,0x1D0008C,0x1DC0089,0x1E80086,0x1F40083,0x2000080,0x20C007D,0x218007A,
	0x2240077,0x2300074,0x23C0071,0x248006E,0x254006B,0x2600068,0x26C0065,0x2780062,
	0x284005F,0x290005C,0x29C0059,0x2A80056,0x2B40053,0x2C00050,0x2CC004D,0x2D8004A,
	0x2E40047,0x2F00044,0x2FC0041,0x308003E,0x314003B,0x3200038,0x32C0035,0x3380032,
	0x344002F,0x350002C,0x35C0029,0x3680026,0x3740023,0x3800020,0x38C001D,0x398001A,
	0x3A40017,0x3B00014,0x3BC0011,0x3C8000E,0x3D4000B,0x3E00008,0x3EC0005,0x3F80002,
};


const uint8_t random_fill_table[] = {
	27,38,6,32,57,35,15,48,9,42,29,23,20,18,14,26,12,50,63,7,17,36,21,37,33,40,11,58,46,64,3,59,
	51,49,5,52,30,54,1,24,16,47,62,53,2,34,41,28,19,43,13,22,39,44,8,0,45,10,60,4,55,25,31,61,56,
};



void modes_run(void) {

	// make sure display_init() is called first!!

	// make sure mode/colors are sane, set them to 0 otherwise
	if (config.mode >= MODE_COUNT) config.mode = 0;
	config.color = config.mode_color[config.mode];
	if (config.color >= modes[config.mode].colorcount) config.color = 0;


	// main loop!
	while(1) {

		// clear abort flag
		state.abort_flag = false;

		// check buttons

		// if mode pushed, increment mode
		if (state.mode_push_edge) {
			state.mode_push_edge = false;

			if (++config.mode >= MODE_COUNT) config.mode = 0;
			config.color = config.mode_color[config.mode];
			if (config.color >= modes[config.mode].colorcount) config.color = 0;

			state.eeprom_write_flag = true;
		}

		// if color pushed, increment mode
		if (state.color_push_edge) {
			state.color_push_edge = false;
			if (++config.color >= modes[config.mode].colorcount) config.color = 0;

			config.mode_color[config.mode] = config.color;
			state.eeprom_write_flag = true;
		}

		// now call mode function
		modes[config.mode].func();
	}

}


// == DISPLAY MODE FUNCTIONS ==


void mode_static(void) {

	color_load(modes[config.mode].colors[config.color], fb1);

	// just refresh display at 1Hz
	while(1) {
		if (display_update(fb1,60)) return;
	}
}

void mode_twinkle(void) {

	uint32_t seed = 0xDEADBEEF;

	color_load(modes[config.mode].colors[config.color], fb1);

	while(1) {
		color_twinkle(fb1,fb2,&seed,192);
		if (display_update(fb2,6)) return;
	}
}


void mode_twinkle_fade(void) {

	int i;
	int tmp;

	// load FB1 and display until buttons released
	color_load(modes[config.mode].colors[config.color], fb1);
	color_load(modes[config.mode].colors[config.color], fb2);
	while (state.mode_pushed || state.color_pushed) {
		if (display_update(fb1,6)) return;
	}

	// stay faded in for a moment
	if (display_update(fb2,10)) return;

	while(1) {

		// fade out
		for(i=0;i<PIXEL_COUNT;i++) {
			tmp = random_fill_table[i];
			fb2[tmp] = 0;
			if (display_update(fb2,1)) return;
		}

		// stay faded out for a half second
		if (display_update(fb2,15)) return;

		// fade in
		for(i=0;i<PIXEL_COUNT;i++) {
			tmp = random_fill_table[i];
			fb2[tmp] = fb1[tmp];
			if (display_update(fb2,1)) return;
		}

		// stay faded in for a second
		if (display_update(fb2,60)) return;
	}
}


void mode_spell_it_out(void) {

	int i;

	// load FB1 and display until buttons released
	color_load(modes[config.mode].colors[config.color], fb1);
	while (state.mode_pushed || state.color_pushed) {
		display_update(fb1,6);
	}

	while(1) {

		// zero fb2
		color_load(color_black, fb2);

		// load oval from fb1 into fb2
		for(i=0;i<OVAL_COUNT;i++) {
			fb2[oval_addresses[i]] = fb1[oval_addresses[i]];
		}

		// just oval for half a second
		if (display_update(fb2,30)) return;

		// turn on N
		for(i=0;i<N_COUNT;i++) fb2[n_addresses[i]] = fb1[n_addresses[i]];
		if (display_update(fb2,30)) return;
		// turn on O
		for(i=0;i<O_COUNT;i++) fb2[o_addresses[i]] = fb1[o_addresses[i]];
		if (display_update(fb2,30)) return;
		// turn on P
		for(i=0;i<P_COUNT;i++) fb2[p_addresses[i]] = fb1[p_addresses[i]];
		if (display_update(fb2,30)) return;
		// turn on E
		for(i=0;i<E_COUNT;i++) fb2[e_addresses[i]] = fb1[e_addresses[i]];
		if (display_update(fb2,30)) return;
	}
}


void mode_write_in(void) {

	int i;

	// load FB1 and display until buttons released
	color_load(modes[config.mode].colors[config.color], fb1);
	color_load(modes[config.mode].colors[config.color], fb2);
	while (state.mode_pushed || state.color_pushed) {
		if (display_update(fb1,6)) return;
	}

	// stay faded in for a moment
	if (display_update(fb2,10)) return;

	while(1) {

		// write out letters
		for (i=0;i<LETTER_COUNT;i++) {
			fb2[letter_addresses[i]] = 0;
			if (display_update(fb2,1)) return;
		}
		// write out oval
		for (i=0;i<OVAL_COUNT;i++) {
			fb2[oval_addresses[i]] = 0;
			if (display_update(fb2,1)) return;
		}

		// quick wait to avoid visible glitch
		if (display_update(fb2,5)) return;

		// write in oval
		for (i=0;i<OVAL_COUNT;i++) {
			fb2[oval_addresses[i]] = fb1[oval_addresses[i]];
			if (display_update(fb2,1)) return;
		}
		// write in letters
		for (i=0;i<LETTER_COUNT;i++) {
			fb2[letter_addresses[i]] = fb1[letter_addresses[i]];
			if (display_update(fb2,1)) return;
		}

		// stay faded in for 3/4 second
		if (display_update(fb2,45)) return;
	}
}


void mode_fade_and_flash(void) {

	int i;

	// load and display color into FB2
	color_load(modes[config.mode].colors[config.color], fb1);
	while (state.mode_pushed || state.color_pushed) {
		if (display_update(fb1,6)) return;
	}

	// load 2nd color into FB2, but black out the letters
	// we can fade between FB1 and FB2 to fade in/out letters
	color_load(modes[config.mode].colors[config.color], fb2);
	for(i=0;i<LETTER_COUNT;i++)
		fb2[letter_addresses[i]] = 0;

	while(1) {
		// stay lit for a moment
		if (display_update(fb1,20)) return;

		// blink 3 times
		if (display_update(fb2,10)) return;
		if (display_update(fb1,10)) return;
		if (display_update(fb2,10)) return;
		if (display_update(fb1,10)) return;
		if (display_update(fb2,10)) return;
		if (display_update(fb1,10)) return;

		// fade out
		for (i = 0; i < 65536; i += (65536/60)) {
			color_fade(fb1,fb2,fb3,(i >> 8));
			if (display_update(fb3,1)) return;
		}

		// fade back in
		for (i = 0; i < 65536; i += (65536/90)) {
			color_fade(fb2,fb1,fb3,(i >> 8));
			if (display_update(fb3,1)) return;
		}
	}

}


void mode_sackville(void) {

	int i,addr;

	// load FB1 and display until buttons released
	color_load(modes[config.mode].colors[config.color], fb1);
	while (state.mode_pushed || state.color_pushed) {
		display_update(fb1,6);
	}

	while(1) {
		for(i=0;i<OVAL_COUNT;i++) {
			addr = oval_addresses[i];
			fb1[addr] = color_rotate(fb1[addr]);
			if (display_update(fb1,1)) return;
		}
	}
}


void mode_hue(void) {

	int i = 0;

	color_load(modes[config.mode].colors[config.color], fb1);
	while(1) {
		color_rotate_fine_fb(fb1,fb2,(i >> 8 & 0xFF));
		i += (65536/300);
		if (display_update(fb2,1)) return;
	}

}


// === COLOR EFFECTS ====

void color_load(const uint32_t *readptr, uint32_t *framebuffer) {

    uint32_t colorvalue;
    int writecount;

    while(*readptr) {
        colorvalue = (*readptr & 0x00FFFFFF) | 0x80000000;
        writecount = (*readptr & 0xFF000000) >> 24;
        while(writecount--) *framebuffer++ = colorvalue;
        readptr++;
    }
}

// R->G, G->B, B->R
uint32_t color_rotate(uint32_t value) {
	return (value & 0xFF000000) | ((value & 0x00FFFF00) >> 8) | ((value & 0x000000FF) << 16);

}

// replaces pixel dim value with dimvalue
uint32_t color_dim(uint32_t value, int dimvalue) {
	return (value & 0x00FFFFFF) | ((dimvalue & 0xFF) << 24);
}

// 0-256 = 0-360 rotation
uint32_t color_rotate_fine(uint32_t value, int shift) {

	// grab multipliers
	uint32_t tablevalue = hue_shift_table[shift & 0xFF];
	int x = (tablevalue >> 18) & 0x1FF;
	int y = (tablevalue >> 9) & 0x1FF;
	int z = (tablevalue >> 0) & 0x1FF;

	int r = (value >> 16) & 0xFF;
	int g = (value >> 8) & 0xFF;
	int b = (value >> 0) & 0xFF;

	value &= 0xFF000000;

	value |= ( ( (x*r) + (y*g) + (z*b) ) << 8) & 0x00FF0000;
	value |= ( ( (x*g) + (y*b) + (z*r) ) >> 8) & 0X0000FF00;
	value |= ( ( (x*b) + (y*r) + (z*g) ) >> 16) & 0x000000FF;

	return value;
}

// whole-framebuffer version of color_rotate_fine
void color_rotate_fine_fb(uint32_t *fbin, uint32_t *fbout, int shift) {

	uint32_t pixel;
	int i,r,g,b,x,y,z;

	// grab multipliers
	uint32_t tablevalue = hue_shift_table[shift & 0xFF];
	x = (tablevalue >> 18) & 0x1FF;
	y = (tablevalue >> 9) & 0x1FF;
	z = (tablevalue >> 0) & 0x1FF;

	for(i=0; i<PIXEL_COUNT; i++) {
		pixel = *fbin++;

		r = (pixel >> 16) & 0xFF;
		g = (pixel >> 8) & 0xFF;
		b = (pixel >> 0) & 0xFF;

		pixel &= 0xFF000000;

		pixel |= ( ( (x*r) + (y*g) + (z*b) ) << 8) & 0x00FF0000;
		pixel |= ( ( (x*g) + (y*b) + (z*r) ) << 0) & 0X0000FF00;
		pixel |= ( ( (x*b) + (y*r) + (z*g) ) >> 8) & 0x000000FF;

		*fbout++ = pixel;
	}
}


// 256 for fb2, 0 for fb1, 128 for halfway
void color_fade(uint32_t *fba, uint32_t *fbb, uint32_t *fbc, int fbb_amount) {

	int i;
	if (fbb_amount > 256) fbb_amount = 256;
	int fba_amount = 256 - fbb_amount;
	uint32_t rb, ag;

	for (i=0;i<PIXEL_COUNT;i++) {
		rb = (*fba & 0x00FF00FF) * fba_amount;
		rb += (*fbb & 0x00FF00FF) * fbb_amount;
		rb >>= 8;
		rb &= 0x00FF00FF;

		ag = ((*fba++ >> 8) & 0x00FF00FF) * fba_amount;
		ag += ((*fbb++ >> 8) & 0x00FF00FF) * fbb_amount;
		ag &= 0xFF00FF00;

		*fbc++ = (rb | ag);
	}
}


// 256 for maximum twinkle, 0 for no twinkle
void color_twinkle(uint32_t *fbin, uint32_t *fbout, uint32_t *seed, int twinkle_amount) {

	uint32_t prng = *seed;
	uint32_t tmp;

	int i;

	for (i=0;i<PIXEL_COUNT;i++) {

		// update PRNG
		prng *= 1664525;
		prng += 1013904223;

		// calculate new local dim value
		tmp = 0x80000000 - ((prng & 0x007FFFFF) * twinkle_amount);

		// and replace in output
		fbout[i] = (fbin[i] & 0x00FFFFFF) | (tmp & 0xFF000000);
	}

	*seed = prng;
}

