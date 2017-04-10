#ifndef I2CLCD_H
#define	I2CLCD_H


#include <p18cxxx.h>
#include "HardwareProfile.h"
#include "i2cmod.h"
#include "mytimers.h"



#define LCD_CONF_MAX_X_CHARS 23
#define LCD_CONF_MAX_Y_LINES 1

#define LCD_CMD_SET_RE_0             0x30
#define LCD_CMD_SET_RE_1             0x34
#define LCD_CMD_DISPLAY_OFF          0x08
#define LCD_CMD_ENTRY_MODE_SET       0x06
#define LCD_CMD_NORMAL_MODE          0x02
#define LCD_CMD_BOOSTx2_FOLLW        0x12
#define LCD_CMD_EXTENDED_FUNC        0x08
#define LCD_CMD_ON_NOCURSOR_NO_BLINK 0x0C
#define LCD_CMD_CLS                  0x01
#define LCD_CMD_HOME                 0x02

void initLCD(void);
void LCD_CLS(void);
void LCD_HOME(void);
void Lcd_XY(unsigned char x, unsigned char y);
void Lcd_nPrintf(unsigned char *stringa, unsigned char len);
void Lcd_Printf(unsigned char * s);


#endif
