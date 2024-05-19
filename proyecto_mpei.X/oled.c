#include "oled.h"

uint8_t _i2caddr, _vccstate, x_pos, y_pos, text_size;
bool wrap = true;

const uint8_t Font[] = {
0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x5F, 0x00, 0x00,
0x00, 0x07, 0x00, 0x07, 0x00,
0x14, 0x7F, 0x14, 0x7F, 0x14,
0x24, 0x2A, 0x7F, 0x2A, 0x12,
0x23, 0x13, 0x08, 0x64, 0x62,
0x36, 0x49, 0x56, 0x20, 0x50,
0x00, 0x08, 0x07, 0x03, 0x00,
0x00, 0x1C, 0x22, 0x41, 0x00,
0x00, 0x41, 0x22, 0x1C, 0x00,
0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
0x08, 0x08, 0x3E, 0x08, 0x08,
0x00, 0x80, 0x70, 0x30, 0x00,
0x08, 0x08, 0x08, 0x08, 0x08,
0x00, 0x00, 0x60, 0x60, 0x00,
0x20, 0x10, 0x08, 0x04, 0x02,
0x3E, 0x51, 0x49, 0x45, 0x3E,
0x00, 0x42, 0x7F, 0x40, 0x00,
0x72, 0x49, 0x49, 0x49, 0x46,
0x21, 0x41, 0x49, 0x4D, 0x33,
0x18, 0x14, 0x12, 0x7F, 0x10,
0x27, 0x45, 0x45, 0x45, 0x39,
0x3C, 0x4A, 0x49, 0x49, 0x31,
0x41, 0x21, 0x11, 0x09, 0x07,
0x36, 0x49, 0x49, 0x49, 0x36,
0x46, 0x49, 0x49, 0x29, 0x1E,
0x00, 0x00, 0x14, 0x00, 0x00,
0x00, 0x40, 0x34, 0x00, 0x00,
0x00, 0x08, 0x14, 0x22, 0x41,
0x14, 0x14, 0x14, 0x14, 0x14,
0x00, 0x41, 0x22, 0x14, 0x08,
0x02, 0x01, 0x59, 0x09, 0x06,
0x3E, 0x41, 0x5D, 0x59, 0x4E,
0x7C, 0x12, 0x11, 0x12, 0x7C,
0x7F, 0x49, 0x49, 0x49, 0x36,
0x3E, 0x41, 0x41, 0x41, 0x22,
0x7F, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x49, 0x49, 0x49, 0x41,
0x7F, 0x09, 0x09, 0x09, 0x01,
0x3E, 0x41, 0x41, 0x51, 0x73,
0x7F, 0x08, 0x08, 0x08, 0x7F,
0x00, 0x41, 0x7F, 0x41, 0x00,
0x20, 0x40, 0x41, 0x3F, 0x01,
0x7F, 0x08, 0x14, 0x22, 0x41,
0x7F, 0x40, 0x40, 0x40, 0x40,
0x7F, 0x02, 0x1C, 0x02, 0x7F,
0x7F, 0x04, 0x08, 0x10, 0x7F,
0x3E, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x09, 0x09, 0x09, 0x06,
0x3E, 0x41, 0x51, 0x21, 0x5E,
0x7F, 0x09, 0x19, 0x29, 0x46
};
const uint8_t Font2[] = {
0x26, 0x49, 0x49, 0x49, 0x32,
0x03, 0x01, 0x7F, 0x01, 0x03,
0x3F, 0x40, 0x40, 0x40, 0x3F,
0x1F, 0x20, 0x40, 0x20, 0x1F,
0x3F, 0x40, 0x38, 0x40, 0x3F,
0x63, 0x14, 0x08, 0x14, 0x63,
0x03, 0x04, 0x78, 0x04, 0x03,
0x61, 0x59, 0x49, 0x4D, 0x43,
0x00, 0x7F, 0x41, 0x41, 0x41,
0x02, 0x04, 0x08, 0x10, 0x20,
0x00, 0x41, 0x41, 0x41, 0x7F,
0x04, 0x02, 0x01, 0x02, 0x04,
0x40, 0x40, 0x40, 0x40, 0x40,
0x00, 0x03, 0x07, 0x08, 0x00,
0x20, 0x54, 0x54, 0x78, 0x40,
0x7F, 0x28, 0x44, 0x44, 0x38,
0x38, 0x44, 0x44, 0x44, 0x28,
0x38, 0x44, 0x44, 0x28, 0x7F,
0x38, 0x54, 0x54, 0x54, 0x18,
0x00, 0x08, 0x7E, 0x09, 0x02,
0x18, 0xA4, 0xA4, 0x9C, 0x78,
0x7F, 0x08, 0x04, 0x04, 0x78,
0x00, 0x44, 0x7D, 0x40, 0x00,
0x20, 0x40, 0x40, 0x3D, 0x00,
0x7F, 0x10, 0x28, 0x44, 0x00,
0x00, 0x41, 0x7F, 0x40, 0x00,
0x7C, 0x04, 0x78, 0x04, 0x78,
0x7C, 0x08, 0x04, 0x04, 0x78,
0x38, 0x44, 0x44, 0x44, 0x38,
0xFC, 0x18, 0x24, 0x24, 0x18,
0x18, 0x24, 0x24, 0x18, 0xFC,
0x7C, 0x08, 0x04, 0x04, 0x08,
0x48, 0x54, 0x54, 0x54, 0x24,
0x04, 0x04, 0x3F, 0x44, 0x24,
0x3C, 0x40, 0x40, 0x20, 0x7C,
0x1C, 0x20, 0x40, 0x20, 0x1C,
0x3C, 0x40, 0x30, 0x40, 0x3C,
0x44, 0x28, 0x10, 0x28, 0x44,
0x4C, 0x90, 0x90, 0x90, 0x7C,
0x44, 0x64, 0x54, 0x4C, 0x44,
0x00, 0x08, 0x36, 0x41, 0x00,
0x00, 0x00, 0x77, 0x00, 0x00,
0x00, 0x41, 0x36, 0x08, 0x00,
0x02, 0x01, 0x02, 0x04, 0x02
};

static uint8_t OLED_buffer[OLED_LCDHEIGHT * (OLED_LCDWIDTH / 8)];

void OLED_command(uint8_t c)
{
  uint8_t control = 0x00;   // Co = 0, D/C = 0
  I2C_Transfer(OLED_I2C_ADDRESS, control, c);
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

#elif defined SSD1306_128_64 || defined SH1106_128_64
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
  
  OLED_Display();
  
  OLED_On(true);
  
  
}

void OLED_DrawPixel(uint8_t x, uint8_t y)
{
    bool color = true;
  if ((x >= OLED_LCDWIDTH) || (y >= OLED_LCDHEIGHT))
    return;
  if (color)
    OLED_buffer[x + (uint16_t)(y / 8) * OLED_LCDWIDTH] |=  (1 << (y & 7));
  else
    OLED_buffer[x + (uint16_t)(y / 8) * OLED_LCDWIDTH] &=  ~(1 << (y & 7));
}

void OLED_Display(void)
{
    uint16_t index = 0;
    for (uint8_t p=0; p<(OLED_LCDHEIGHT + 7)/8; p++)
    {   
      I2C_Start (); // inicia la comunicaci?n
      I2C_Tx (OLED_I2C_ADDRESS); //direccion I2C
      IdleI2C();
      ACKStatus();
      I2C_Tx (0x00); //Control de direcci?n
      IdleI2C();
      ACKStatus();
      I2C_Tx(0XB0 + p);
      IdleI2C();
      ACKStatus();
      I2C_Tx(0X00);
      IdleI2C();
      ACKStatus();
      I2C_Tx(0X10);
      IdleI2C();
      ACKStatus();
      I2C_Stop (); //termina la comunicaci?n
      
      I2C_Start();
      I2C_Tx(OLED_I2C_ADDRESS);
      IdleI2C();
      ACKStatus();
      I2C_Tx(0X40);
      IdleI2C();
      ACKStatus();
      for(uint8_t column = 0; column < OLED_LCDWIDTH; column++)
      {
          I2C_Tx(OLED_buffer[index++]);
          IdleI2C();
          ACKStatus();
      }
      I2C_Stop();
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

void OLED_DrawText(uint8_t x, uint8_t y, char *_text)
{
  uint8_t size = 1;
  OLED_GotoXY(x, y);
  OLED_TextSize(size);
  while(*_text != '\0')
    OLED_Print(*_text++);
}

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
      if(text_size == 1) OLED_DrawPixel(x_pos + i, y_pos + j);
      else               OLED_FillRect(x_pos + (i * text_size), y_pos + (j * text_size), text_size, text_size);
    }
  }

  OLED_FillRect(x_pos + (5 * text_size), y_pos, text_size, 7 * text_size);
  
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

void OLED_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
  int16_t _x = x;
  for(int16_t i = _x; i < _x + w; i++){
      OLED_DrawFastVLine(i, y, h);
  }
}

void OLED_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h)
{
  OLED_DrawLine(x, y, x, y + h - 1);
}

void OLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  bool color = true;
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
      else      OLED_DrawPixel(y0, x0);
    }
    else {
      if(color) OLED_DrawPixel(x0, y0);
      else      OLED_DrawPixel(x0, y0);
    }
    err -= dy;
    if (err < 0) {
      y0  += ystep;
      err += dx;
    }
  }
}

void OLED_TextSize(uint8_t t_size)
{
  if(t_size < 1)
    t_size = 1;
  text_size = t_size;
}

void OLED_GotoXY(uint8_t x, uint8_t y)
{
  if((x >= OLED_LCDWIDTH) || (y >= OLED_LCDHEIGHT))
    return;
  x_pos = x;
  y_pos = y;
}

void OLED_On(bool _enable)
{
    I2C_Start();
    I2C_Tx(OLED_I2C_ADDRESS);
    IdleI2C();
    ACKStatus();
    I2C_Tx(0X00);
    if(_enable)
    {
        I2C_Tx(0X8D);
        IdleI2C();
        ACKStatus();
        I2C_Tx(0X14);
        IdleI2C();
        ACKStatus();
        I2C_Tx(0XAF);
        IdleI2C();
        ACKStatus();
    }
    else
    {
        I2C_Tx(0XAE);
        IdleI2C();
        ACKStatus();
        I2C_Tx(0X8D);
        IdleI2C();
        ACKStatus();
        I2C_Tx(0X10);
        IdleI2C();
        ACKStatus();
    }
    I2C_Stop();
}