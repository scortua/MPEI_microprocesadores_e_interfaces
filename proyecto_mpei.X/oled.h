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

void OLED_command(uint8_t c);
void OLED_Begin();//inicializa la pantalla
void OLED_DrawPixel(uint8_t x, uint8_t y);//cambia de color un pixel de la pantalla
void OLED_Display(void);//muestra los datos en pantalla
void OLED_ClearDisplay(void);//borra la pantalla
void OLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);//crea una linea (x,y,ancho,alto,color 1/0)
void OLED_DrawText(uint8_t x, uint8_t y, char *_text);//coloca un texto
void OLED_TextSize(uint8_t t_size);//cambia el tama?o del texto
void OLED_GotoXY(uint8_t x, uint8_t y);//posicion de inicion del texto
void OLED_Print(uint8_t c);//imprime el texto
void OLED_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);//rellena el area de un color(x,y,ancho,alto,color 1/0)
void OLED_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h);//linea vertical (x,y,diametro,color)
void OLED_On(bool enable);


#endif	/* OLED_H */

