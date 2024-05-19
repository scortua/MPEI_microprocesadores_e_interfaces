#include "display.h"

#define OLED_swap(a, b) { int16_t t = a; a = b; b = t; }

uint8_t _i2caddr, _vccstate, x_pos, y_pos, text_size;
bool wrap = true;

void OLED_command(uint8_t c)
{
  uint8_t control = 0x00;   // Co = 0, D/C = 0
  I2C_Transfer(OLED_I2C_ADDRESS,control,c);
}

uint8_t vccstate = OLED_SWITCHCAPVCC;
uint8_t i2caddr = OLED_I2C_ADDRESS;
void OLED_Begin()
{
    
  _vccstate = vccstate;
  _i2caddr  = i2caddr;
  __delay_ms(10);

  #ifdef OLED_RST
    output_low(OLED_RST);
    output_drive(OLED_RST);
    delay_ms(10);
    output_high(OLED_RST);
  #endif
  
  // Init sequence
  OLED_command(OLED_DISPLAYOFF);                    // 0xAE
  OLED_command(OLED_SETDISPLAYCLOCKDIV);            // 0xD5
  OLED_command(0x80);                                  // the suggested ratio 0x80

  OLED_command(OLED_SETMULTIPLEX);                  // 0xA8
  OLED_command(OLED_LCDHEIGHT - 1);

  OLED_command(OLED_SETDISPLAYOFFSET);              // 0xD3
  OLED_command(0x0);                                   // no offset
  OLED_command(OLED_SETSTARTLINE | 0x0);            // line #0
  OLED_command(OLED_CHARGEPUMP);                    // 0x8D
  if (vccstate == OLED_EXTERNALVCC)
    { OLED_command(0x10); }
  else
    { OLED_command(0x14); }
  OLED_command(OLED_MEMORYMODE);                    // 0x20
  OLED_command(0x00);                                  // 0x0 act like ks0108
  OLED_command(OLED_SEGREMAP | 0x1);
  OLED_command(OLED_COMSCANDEC);

 #if defined SSD1306_128_32
  OLED_command(OLED_SETCOMPINS);                    // 0xDA
  OLED_command(0x02);
  OLED_command(OLED_SETCONTRAST);                   // 0x81
  OLED_command(0x8F);

#elif defined SSD1306_128_64
  OLED_command(OLED_SETCOMPINS);                    // 0xDA
  OLED_command(0x12);
  OLED_command(OLED_SETCONTRAST);                   // 0x81
  if (vccstate == OLED_EXTERNALVCC)
    { OLED_command(0x9F); }
  else
    { OLED_command(0xCF); }

#elif defined SSD1306_96_16
  OLED_command(OLED_SETCOMPINS);                    // 0xDA
  OLED_command(0x2);   //ada x12
  OLED_command(OLED_SETCONTRAST);                   // 0x81
  if (vccstate == OLED_EXTERNALVCC)
    { OLED_command(0x10); }
  else
    { OLED_command(0xAF); }

#endif

  OLED_command(OLED_SETPRECHARGE);                  // 0xd9
  if (vccstate == OLED_EXTERNALVCC)
    { OLED_command(0x22); }
  else
    { OLED_command(0xF1); }
  OLED_command(OLED_SETVCOMDETECT);                 // 0xDB
  OLED_command(0x40);
  OLED_command(OLED_DISPLAYALLON_RESUME);           // 0xA4
  OLED_command(OLED_NORMALDISPLAY);                 // 0xA6

  OLED_command(OLED_DEACTIVATE_SCROLL);

  OLED_command(OLED_DISPLAYON);//--turn on oled panel
  
  // set cursor to (0, 0)
  x_pos = 0;
  y_pos = 0;
  // set text size to 1
  text_size = 1;
}

void OLED_DrawPixel(uint8_t x, uint8_t y, bool color)
{
  color = true;
  if ((x >= OLED_LCDWIDTH) || (y >= OLED_LCDHEIGHT))
    return;
  if (color)
    OLED_buffer[x + (uint16_t)(y / 8) * OLED_LCDWIDTH] |=  (1 << (y & 7));
  else
    OLED_buffer[x + (uint16_t)(y / 8) * OLED_LCDWIDTH] &=  ~(1 << (y & 7));
}

void OLED_StartScrollRight(uint8_t start, uint8_t stop)
{
  OLED_command(OLED_RIGHT_HORIZONTAL_SCROLL);
  OLED_command(0X00);
  OLED_command(start);
  OLED_command(0X00);
  OLED_command(stop);
  OLED_command(0X00);
  OLED_command(0XFF);
  OLED_command(OLED_ACTIVATE_SCROLL);
}

void OLED_StartScrollLeft(uint8_t start, uint8_t stop)
{
  OLED_command(OLED_LEFT_HORIZONTAL_SCROLL);
  OLED_command(0X00);
  OLED_command(start);
  OLED_command(0X00);
  OLED_command(stop);
  OLED_command(0X00);
  OLED_command(0XFF);
  OLED_command(OLED_ACTIVATE_SCROLL);
}

void OLED_StartScrollDiagRight(uint8_t start, uint8_t stop)
{
  OLED_command(OLED_SET_VERTICAL_SCROLL_AREA);
  OLED_command(0X00);
  OLED_command(OLED_LCDHEIGHT);
  OLED_command(OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  OLED_command(0X00);
  OLED_command(start);
  OLED_command(0X00);
  OLED_command(stop);
  OLED_command(0X01);
  OLED_command(OLED_ACTIVATE_SCROLL);
}

void OLED_StartScrollDiagLeft(uint8_t start, uint8_t stop)
{
  OLED_command(OLED_SET_VERTICAL_SCROLL_AREA);
  OLED_command(0X00);
  OLED_command(OLED_LCDHEIGHT);
  OLED_command(OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  OLED_command(0X00);
  OLED_command(start);
  OLED_command(0X00);
  OLED_command(stop);
  OLED_command(0X01);
  OLED_command(OLED_ACTIVATE_SCROLL);
}

void OLED_StopScroll(void)
{
  OLED_command(OLED_DEACTIVATE_SCROLL);
}

void OLED_Dim(bool dim)
{
  uint8_t contrast;
  if (dim)
    contrast = 0; // Dimmed display
  else {
    if (_vccstate == OLED_EXTERNALVCC)
      contrast = 0x9F;
    else
      contrast = 0xCF;
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  OLED_command(OLED_SETCONTRAST);
  OLED_command(contrast);
}

void OLED_Display(void)
{
   uint16_t n2=0;
   rom uint8_t Page[8]={0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7};
    for (uint8_t p=1; p<=OLED_LCDHEIGHT/8; ++p)
   { 
      OLED_command(0x01);//Set Lower Column Address (1) 
      OLED_command(0x10);//Set Higher Column Address (2) 
      OLED_command(Page[p-1]);//Set Page Address(12)    
      I2C_Start(); // inicia la comunicacion
      I2C_Tx(OLED_I2C_ADDRESS); //direccion I2C    
      IdleI2C();
      ACKStatus();
      I2C_Tx (0x40); //Control de direccion
      IdleI2C();
      ACKStatus();
      for (uint8_t i=1; i<=OLED_LCDWIDTH; ++i)
      {      
         I2C_Tx (OLED_buffer[n2]); 
         IdleI2C();
         ACKStatus();
         n2++;
      }
       I2C_Stop (); //termina la comunicaci?n
   }
}

void OLED_ClearDisplay(void)
{
    uint8_t Page[8]={0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7};
    memset(OLED_buffer, 0, sizeof(OLED_buffer));
   for (uint8_t p=1; p<=OLED_LCDHEIGHT/8; ++p)
   {  
      OLED_command(0x01);//Set Lower Column Address (1) 
      OLED_command(0x10);//Set Higher Column Address (2) 
      OLED_command(Page[p-1]);//Set Page Address(12)    
      I2C_Start (); // inicia la comunicaci?n
      I2C_Tx (OLED_I2C_ADDRESS); //direccion I2C
      IdleI2C();
      ACKStatus();
      I2C_Tx (0x40); //Control de direcci?n
      IdleI2C();
      ACKStatus();
      for (uint8_t i=0; i<=OLED_LCDWIDTH; ++i)//cuando se envia un dato la columa se recorre
      {     
         I2C_Tx (0x00); // --- blanco (0xFF) 
         IdleI2C();
         ACKStatus();
      }
      I2C_Stop (); //termina la comunicaci?n
   }
}

void OLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool color = true)
{
  bool steep;
  int8_t ystep;
  uint8_t dx, dy;
  int16_t err;
  steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    OLED_swap(x0, y0);
    OLED_swap(x1, y1);
  }
  if (x0 > x1) {
    OLED_swap(x0, x1);
    OLED_swap(y0, y1);
  }
  dx = x1 - x0;
  dy = abs(y1 - y0);

  err = dx / 2;
  if (y0 < y1)
    ystep = 1;
  else
    ystep = -1;

  for (; x0 <= x1; x0++) {
    if (steep) {
      if(color) OLED_DrawPixel(y0, x0);
      else      OLED_DrawPixel(y0, x0, false);
    }
    else {
      if(color) OLED_DrawPixel(x0, y0);
      else      OLED_DrawPixel(x0, y0, false);
    }
    err -= dy;
    if (err < 0) {
      y0  += ystep;
      err += dx;
    }
  }
}

void OLED_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, bool color = true)
{
   OLED_DrawLine(x, y, x + w - 1, y, color);
}

void OLED_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, bool color = true)
{
  OLED_DrawLine(x, y, x, y + h - 1, color);
}

void OLED_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color = true)
{
  for (int16_t i = x; i < x + w; i++)
    OLED_DrawFastVLine(i, y, h, color);
}

void OLED_FillScreen(bool color = true) {
  OLED_FillRect(0, 0, OLED_LCDWIDTH, OLED_LCDHEIGHT, color);
}

void OLED_DrawCircle(int16_t x0, int16_t y0, int16_t r)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  OLED_DrawPixel(x0  , y0 + r);
  OLED_DrawPixel(x0  , y0 - r);
  OLED_DrawPixel(x0 + r, y0);
  OLED_DrawPixel(x0 - r, y0);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    OLED_DrawPixel(x0 + x, y0 + y);
    OLED_DrawPixel(x0 - x, y0 + y);
    OLED_DrawPixel(x0 + x, y0 - y);
    OLED_DrawPixel(x0 - x, y0 - y);
    OLED_DrawPixel(x0 + y, y0 + x);
    OLED_DrawPixel(x0 - y, y0 + x);
    OLED_DrawPixel(x0 + y, y0 - x);
    OLED_DrawPixel(x0 - y, y0 - x);
  }

}

void OLED_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      OLED_DrawPixel(x0 + x, y0 + y);
      OLED_DrawPixel(x0 + y, y0 + x);
    }
    if (cornername & 0x2) {
      OLED_DrawPixel(x0 + x, y0 - y);
      OLED_DrawPixel(x0 + y, y0 - x);
    }
    if (cornername & 0x8) {
      OLED_DrawPixel(x0 - y, y0 + x);
      OLED_DrawPixel(x0 - x, y0 + y);
    }
    if (cornername & 0x1) {
      OLED_DrawPixel(x0 - y, y0 - x);
      OLED_DrawPixel(x0 - x, y0 - y);
    }
  }

}

void OLED_FillCircle(int16_t x0, int16_t y0, int16_t r, bool color = true)
{
  OLED_DrawFastVLine(x0, y0 - r, 2 * r + 1, color);
  OLED_FillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void OLED_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, bool color = true) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x01) {
      OLED_DrawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
      OLED_DrawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
    }
    if (cornername & 0x02) {
      OLED_DrawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
      OLED_DrawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
    }
  }
}

// Draw a rectangle
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
  OLED_DrawFastHLine(x, y, w);
  OLED_DrawFastHLine(x, y + h - 1, w);
  OLED_DrawFastVLine(x, y, h);
  OLED_DrawFastVLine(x + w - 1, y, h);
}

// Draw a rounded rectangle
void OLED_DrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
  // smarter version
  OLED_DrawFastHLine(x + r, y, w - 2 * r); // Top
  OLED_DrawFastHLine(x + r, y + h - 1, w - 2 * r); // Bottom
  OLED_DrawFastVLine(x, y + r, h - 2 * r); // Left
  OLED_DrawFastVLine(x + w - 1, y + r, h - 2 * r); // Right
  // draw four corners
  OLED_DrawCircleHelper(x + r, y + r, r, 1);
  OLED_DrawCircleHelper(x + w - r - 1, y + r, r, 2);
  OLED_DrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4);
  OLED_DrawCircleHelper(x + r, y + h - r - 1, r, 8);
}

// Fill a rounded rectangle
void OLED_FillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, bool color = true)
{
  // smarter version
  OLED_FillRect(x + r, y, w - 2 * r, h, color);
  // draw four corners
  OLED_FillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
  OLED_FillCircleHelper(x + r        , y + r, r, 2, h - 2 * r - 1, color);
}

// Draw a triangle
void OLED_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  OLED_DrawLine(x0, y0, x1, y1);
  OLED_DrawLine(x1, y1, x2, y2);
  OLED_DrawLine(x2, y2, x0, y0);
}

// Fill a triangle
void OLED_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color = true)
{
  int16_t a, b, y, last;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    OLED_swap(y0, y1); OLED_swap(x0, x1);
  }
  if (y1 > y2) {
    OLED_swap(y2, y1); OLED_swap(x2, x1);
  }
  if (y0 > y1) {
    OLED_swap(y0, y1); OLED_swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    OLED_DrawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1;
  int32_t  sa   = 0, sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1 - 1; // Skip it

  for(y = y0; y <= last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) OLED_swap(a, b);
    OLED_DrawFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y <= y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) OLED_swap(a, b);
    OLED_DrawFastHLine(a, y, b - a + 1, color);
  }
}

// invert the display
void OLED_InvertDisplay(bool i)
{
  if (i)
    OLED_command(OLED_INVERTDISPLAY_);
  else
    OLED_command(OLED_NORMALDISPLAY);
}

void OLED_SetTextWrap(bool w)
{
  wrap = w;
}

void OLED_DrawChar(uint8_t x, uint8_t y, uint8_t c, uint8_t size = 1)
{
  OLED_GotoXY(x, y);
  OLED_TextSize(size);
  OLED_Print(c);
}

void OLED_DrawText(uint8_t x, uint8_t y, char *_text, uint8_t size = 1)
{
  OLED_GotoXY(x, y);
  OLED_TextSize(size);
  while(*_text != '\0')
    OLED_Print(*_text++);
}

// move cursor to position (x, y)
void OLED_GotoXY(uint8_t x, uint8_t y)
{
  if((x >= OLED_LCDWIDTH) || (y >= OLED_LCDHEIGHT))
    return;
  x_pos = x;
  y_pos = y;
}

// set text size
void OLED_TextSize(uint8_t t_size)
{
  if(t_size < 1)
    t_size = 1;
  text_size = t_size;
}

/* print single char
    \a  Set cursor position to upper left (0, 0)
    \b  Move back one position
    \n  Go to start of current line
    \r  Go to line below
*/
void OLED_Print(uint8_t c)
{
  bool _color;
  uint8_t i, j, line;
  
  if (c == ' ' && x_pos == 0 && wrap)
    return;
  if(c == '\a') {
    x_pos = y_pos = 0;
    return;
  }
  if( (c == '\b') && (x_pos >= text_size * 6) ) {
    x_pos -= text_size * 6;
    return;
  }
  if(c == '\r') {
    x_pos = 0;
    return;
  }
  if(c == '\n') {
    y_pos += text_size * 8;
    if((y_pos + text_size * 7) > OLED_LCDHEIGHT)
      y_pos = 0;
    return;
  }

  if((c < ' ') || (c > '~'))
    c = '?';
  
  for(i = 0; i < 5; i++ ) {
    if(c < 'S')
      line = Font[(c - ' ') * 5 + i];
    else
      line = Font2[(c - 'S') * 5 + i];
    
    for(j = 0; j < 7; j++, line >>= 1) {
      if(line & 0x01)
        _color = true;
      else
        _color = false;
      if(text_size == 1) OLED_DrawPixel(x_pos + i, y_pos + j, _color);
      else               OLED_FillRect(x_pos + (i * text_size), y_pos + (j * text_size), text_size, text_size, _color);
    }
  }

  OLED_FillRect(x_pos + (5 * text_size), y_pos, text_size, 7 * text_size, false);
  
  x_pos += text_size * 6;

  if( x_pos > (OLED_LCDWIDTH + text_size * 6) )
    x_pos = OLED_LCDWIDTH;

  if (wrap && (x_pos + (text_size * 5)) > OLED_LCDWIDTH)
  {
    x_pos = 0;
    y_pos += text_size * 8;
    if((y_pos + text_size * 7) > OLED_LCDHEIGHT)
      y_pos = 0;
  }
}


// end of driver code.