#ifndef OLED_H
#define	OLED_H

#include <stdint.h>
#include "xc.h"
#include "i2c.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "p33FJ128MC802.h" 
#include <libpic30.h>

#define SH1106_128_64

#define OLED_I2C_ADDRESS 0x7A //? 0x7A  direcci?n I2C

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#if !defined SSD1306_128_32 && !defined SSD1306_96_16 && !defined SSD1306_128_64 && !defined SH1106_128_64
   #define OLED_LCDWIDTH            128
   #define OLED_LCDHEIGHT            64
#endif

#if defined SH1106_128_64
   #define OLED_LCDWIDTH            128
   #define OLED_LCDHEIGHT            64
#endif
#if defined SSD1306_128_64
   #define OLED_LCDWIDTH            128
   #define OLED_LCDHEIGHT            64
#endif
#if defined SSD1306_128_32
   #define OLED_LCDWIDTH            128
   #define OLED_LCDHEIGHT            32
#endif
#if defined SSD1306_96_16
   #define OLED_LCDWIDTH             96
   #define OLED_LCDHEIGHT            16
#endif

#define OLED_SETCONTRAST          0x81
#define OLED_DISPLAYALLON_RESUME  0xA4
#define OLED_DISPLAYALLON         0xA5
#define OLED_NORMALDISPLAY        0xA6
#define OLED_INVERTDISPLAY_       0xA7
#define OLED_DISPLAYOFF           0xAE
#define OLED_DISPLAYON            0xAF
#define OLED_SETDISPLAYOFFSET     0xD3
#define OLED_SETCOMPINS           0xDA
#define OLED_SETVCOMDETECT        0xDB
#define OLED_SETDISPLAYCLOCKDIV   0xD5
#define OLED_SETPRECHARGE         0xD9
#define OLED_SETMULTIPLEX         0xA8
#define OLED_SETLOWCOLUMN         0x00
#define OLED_SETHIGHCOLUMN        0x10
#define OLED_SETSTARTLINE         0x40
#define OLED_MEMORYMODE           0x20
#define OLED_COLUMNADDR           0x21
#define OLED_PAGEADDR             0x22
#define OLED_COMSCANINC           0xC0
#define OLED_COMSCANDEC           0xC8
#define OLED_SEGREMAP             0xA0
#define OLED_CHARGEPUMP           0x8D
#define OLED_EXTERNALVCC          0x01
#define OLED_SWITCHCAPVCC         0x02

// Scrolling #defines
#define OLED_ACTIVATE_SCROLL                      0x2F
#define OLED_DEACTIVATE_SCROLL                    0x2E
#define OLED_SET_VERTICAL_SCROLL_AREA             0xA3
#define OLED_RIGHT_HORIZONTAL_SCROLL              0x26
#define OLED_LEFT_HORIZONTAL_SCROLL               0x27
#define OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A

#define OLED_swap(a, b) { int16_t t = a; a = b; b = t; }

static void sh1106_send_command(uint8_t command);
static void sh1106_send_data(uint8_t data);
void sh1106_init(void);
void sh1106_clear(void);
void sh1106_display(void);
void sh1106_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void sh1106_draw_char(uint8_t x, uint8_t y, char c);
void sh1106_draw_string(uint8_t x, uint8_t y, const char* str);


#endif	/* OLED_H */

