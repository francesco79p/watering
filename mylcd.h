/*!
\file
Header file del modulo LCD. Il modulo LCD non ha interrupts.


*/


#define LCD_WINSTAR



#define LCD_CONF_8BIT_IF           3
#define LCD_CONF_4BIT_IF           2
#define LCD_CONF_FUNCTION_SET      0b00101100
#define LCD_CONF_4BIT_LINE_CURSOR  9
#define LCD_CONF_BIAS_LINES       21
#define LCD_CONF_CONTRAST        123
#define LCD_CONF_POWER_CTL        94
#define LCD_CONF_FOLLOWER        108  
#define LCD_CONF_DISPLAY_IO       12
#define LCD_CONF_DISPLAY_OFF       8
#define LCD_CONF_DISPLAY_ON       0x0e
#define LCD_CONF_CLEAR_DISPL       1
#define LCD_CONF_ENTRY_MODE        6
#define LCD_CONF_RETURN_HOME       2


#define LCD_CONF_MAX_X_CHARS    0x0F

#ifdef LCD_COG
  #define LCD_CONF_MAX_Y_LINES    0x02
#elif defined(LCD_WINSTAR)
  #define LCD_CONF_MAX_Y_LINES    0x02
#endif

#define LCD_FLAG_INSTRUCTION       0
#define LCD_FLAG_DATA              1

#ifdef LCD_COG
  #define LCD_HOME_TO_1ST_LINE    0x80
  #define LCD_HOME_TO_2ND_LINE    0x90
  #define LCD_HOME_TO_3RD_LINE    0xA0
#elif defined(LCD_WINSTAR)
  #define LCD_HOME_TO_1ST_LINE    0x80
  #define LCD_HOME_TO_2ND_LINE    0xC0
#endif

#define LCD_CLEAR_DISPLAY          1



#define Lcd_ON();   Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CONF_DISPLAY_IO);
#define Lcd_OFF();  Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CONF_DISPLAY_OFF);

#define mLcdEnable()                   po_Lcd_EN = 1;
#define mLcdDisable()                  po_Lcd_EN = 0;
#define mLcdSendInstruction()          po_Lcd_RS = 1;
#define mLcdSendData()                 po_Lcd_RS = 0;
#define mLcdWriteToRAM()               Nop(); //po_Lcd_RW = 0;
#define mLcdReadFromRAM()              Nop(); //po_Lcd_RW = 1;
#define mLcdBkLightON()                po_Lcd_BK = 1;
#define mLcdBkLightOFF()               po_Lcd_BK = 0;
#define mLcdBkLightTG()                po_Lcd_BK = po_Lcd_BK^1;


void Lcd_CLS(void);
void Lcd_Init(void);
void Lcd_SendNibble(char nibble);
void Lcd_SendByte(char addr, char byte);
void Lcd_XY( char x, char y);
void Lcd_Putc(char c);
void Lcd_Printf(rom const unsigned char * stringa);
void Lcd_nPrintf(unsigned char *stringa, unsigned char len);


