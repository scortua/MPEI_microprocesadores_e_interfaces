#ifndef INC_FONTS_H_
#define INC_FONTS_H_

#include "xc.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

extern const uint8_t FONT_1[];
extern const uint8_t FONT_2[];
extern const uint8_t FONT_3[];

typedef struct {
	uint8_t FontWidth;
	uint8_t FontHeight;
	const uint16_t *data;
} FontDef_t;

typedef struct {
	uint16_t Length;
	uint16_t Height;
} FONTS_SIZE_t;


#endif
