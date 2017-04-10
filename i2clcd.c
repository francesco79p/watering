#include "i2clcd.h"
    
        
// _______________________________________________________________ [initLCD]
/*!
\brief Implementa l'inizializzazione del display IIC
*/            
void initLCD(void){    
    
    //i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_SET_RE_0);
    //delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_SET_RE_0);
    delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_SET_RE_0);
    delayNcy(1000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_DISPLAY_OFF);
    delayNcy(1000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_ENTRY_MODE_SET);
    delayNcy(2000);
    
    //i2c_WriteOne2(0x00,LCDADDR,0x30);
    //delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_SET_RE_1);
    delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_NORMAL_MODE);
    delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_ENTRY_MODE_SET);
    delayNcy(2000);
    
    /// per funzionare con booster 2x va messo così LCD_CMD_BOOSTx2_FOLLW e
    /// non 0x16 come dice il datasheet.
    /// in aggiunta ci DEVE stare condensatore su VOUT altrimenti
    /// la polarizzazione dei segmenti balla e non si vede bene
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_BOOSTx2_FOLLW);
    delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_EXTENDED_FUNC);
    delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_EXTENDED_FUNC);
    delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_SET_RE_0);
    delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_ON_NOCURSOR_NO_BLINK);
    delayNcy(2000);   
    
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_CLS);
    delayNcy(2000);
    
    i2c_WriteOne2(0x00,LCDADDR,0x84);
    delayNcy(2000);
    
} 
// end: ________________________________________________________________ initLCD   




void LCD_CLS(void){
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_CLS);
}

void LCD_HOME(void){
    i2c_WriteOne2(0x00,LCDADDR,LCD_CMD_HOME);
}


// _______________________________________________________________ [Lcd_nPrintf]
/*!
\brief Implementa la scrittura di stringhe da array di chars
\param string e' un puntatore ad array di caratteri
\param len e' la lunghezza della stringa
*/
void Lcd_nPrintf(unsigned char *stringa, unsigned char len)
{

    i2c_WriteMany2(0x40,LCDADDR,stringa,len);
    delayNcy(2000);

    
}// end: ___________________________________________________________ Lcd_nPrintf

void Lcd_Printf(unsigned char * s)
{
    while(*s)
    {
        i2c_WriteOne2(0x40,LCDADDR,*s++);
        delayNcy(2000);
    }
}


// ____________________________________________________________________ [Lcd_XY]
void Lcd_XY( unsigned char x, unsigned char y)
{

  // l'ascissa non puo' essere maggiore di LCD_CONF_MAX_X_CHARS
    // l'ordinata non puo' mai essere maggiore del num di linee
  if(x>LCD_CONF_MAX_X_CHARS)
    x=LCD_CONF_MAX_X_CHARS;
  

  // l'ordinata non puo' mai essere maggiore del num di linee
  if(y==1)
    i2c_WriteOne2(0x00,LCDADDR,(0xC0+x));
  else
    i2c_WriteOne2(0x00,LCDADDR,(0x80+x));
  
  
  delayNcy(2000);

}//end: _________________________________________________________________ Lcd_XY
