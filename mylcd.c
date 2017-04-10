#include <p18cxxx.h>
#include <stdio.h>
#include "HardwareProfile.h"
#include "mytimers.h"
#include "mylcd.h"




// _______________________________________________ Application Specific Includes




// ______________________________________________ Application Specific Variables

extern rom char lcdlines[];
static char lcdline=0;
static char curx=0;


// _____________________________________________________________________ [LcdIO]
/*!
\brief Gestisce l'LCD
LcdIO gestisce le operazioni da fare con l'LCD in base ai flag di stato, E'
una funzione specifica per l'applicazione in uso. Quindi e' un modulo che
deve essere personalizzato in base alle esigenze dell'utente.
*/
void LcdIO(void)
{
  
  
  
  
}// end: _________________________________________________________________ LcdIO













// ************************************************************************** //
// ************************************************************************** //
// *****************                                         **************** //
// *****************                                         **************** //
// *****************       DA QUI IN POI NON TOCCARE         **************** //
// *****************                                         **************** //
// *****************                                         **************** //
// ************************************************************************** //
// ************************************************************************** //




// __________________________________________________________________ [Lcd_Init]
/*!
\brief Init dell'LCD.
L'LCD lo usiamo nella modalità nibble. Questo modello avrebbe anche 
l'SPI ma non lo usiamo... magari nel DACS2.
*/
void Lcd_Init(void)
{
  // tutti i pin dell'LCD sono outputs e all'inizio sono low.
  LcdPort    = 0;
  tr_LcdPort = 0;
  
  // dal momento in cui tutti i pin di controllo sono a 0
  // devono passare 100ms.
  delayT0(T0_CONST_0100_MS);
  
  Lcd_SendNibble(LCD_CONF_8BIT_IF);
  delayT0(T0_CONST_0015_MS);

  Lcd_SendNibble(LCD_CONF_8BIT_IF);
  delayT0(T0_CONST_0015_MS);

  Lcd_SendNibble(LCD_CONF_8BIT_IF);
  delayT0(T0_CONST_0002_MS);

  Lcd_SendNibble(LCD_CONF_4BIT_IF);
  delayT0(T0_CONST_0002_MS);

  // da qui in poi sto in 4bit mode



  Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CONF_FUNCTION_SET);
  delayT0(T0_CONST_0002_MS);

  Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CONF_DISPLAY_OFF);
  delayT0(T0_CONST_0002_MS);

  Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CONF_CLEAR_DISPL);
  delayT0(T0_CONST_0002_MS);

  Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CONF_ENTRY_MODE);
  delayT0(T0_CONST_0002_MS);



  Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CONF_DISPLAY_ON);
  delayT0(T0_CONST_0002_MS);


/*
  Lcd_SendByte(LCD_FLAG_INSTRUCTION,0x80);
  delayT0(T0_CONST_0002_MS);






  



  Lcd_SendByte(LCD_FLAG_INSTRUCTION,64);
  delayT0(T0_CONST_0002_MS);



  Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CONF_DISPLAY_IO);
  delayT0(T0_CONST_0002_MS);


   Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CONF_RETURN_HOME);
  delayT0(T0_CONST_0002_MS);
 
*/


}//end: Lcd_Init _______________________________________________________________




#ifdef DACS2_BOARD
// ______________________________________________________________ [setupTrimmer]
/*!
\brief Gestisce il contrasto dell'LCD 
setupTrimmer è prevista solo per il DACs II, perchè ha un pot digitale  che regola il contrasto.
*/
void setupTrimmer(unsigned char taps, unsigned char dir)
{
  unsigned char i;

  
  po_Trim_UD = dir;
  for(i=0;i<taps;i++)
  {
    delayT0(T0_CONST_0030_US); 
    po_Trim_CK=0;
    delayT0(T0_CONST_0030_US);
    po_Trim_CK=1;

  }
  
}// end: __________________________________________________________ setupTrimmer
#endif






// ____________________________________________________________ [Lcd_SendNibble]
/*!
\brief Manda un nibble all'LCD.
Manda un nubble all'LCD. Il nibble deve stare engli LSb del byte 
passato come parametro. 
Chiama: delayT0
\param Nibble: è il byte che contiene nei suoi LSb il nibble da 
spedire. 
*/
void Lcd_SendNibble(char nibble)
{
  // i dati vengono caricati sul falling edge della linea enable
  // quindi la alzo
  mLcdEnable();

  // attendo 4 Cicli
  mFourCyDelay();

  // preparo il nibble
  LcdPort &=0x0f;
  LcdPort |= ((nibble & 0x0f)<<4 );

  // attendo 4 Cicli il dato stabile
  mFourCyDelay();

  // asserisco la linea enable per caricare il nibble
  mLcdDisable();

  // attendo prima di consentire altre operazioni
  delayT0(T0_CONST_0002_US);

}// end: ________________________________________________________ Lcd_SendNibble





// ______________________________________________________________ [Lcd_SendByte]
/*!
\brief Manda un Byte all'LCD
Questa funzione manda un byte all'LCD, chiama la funzione 
Lcd_SendNibble due volte.
La linea RS indica la scrittura nella RAM dell'LCD di un 
instruction solo quando questa è a 1.
*/
void Lcd_SendByte(char addr, char byte)
{

  po_Lcd_RW = 1;
  po_Lcd_RS = 0;
  delayT0(T0_CONST_0010_US);

  po_Lcd_RS = addr;
  po_Lcd_RW = 0;
  delayT0(T0_CONST_0010_US);
  mLcdDisable();

  Lcd_SendNibble(byte >> 4);


  mFourCyDelay();
  mFourCyDelay();

  Lcd_SendNibble(byte & 0x0f);

po_Lcd_RW = 1;
  po_Lcd_RS = 0;

}// end: __________________________________________________________ Lcd_SendByte






// ____________________________________________________________________ [Lcd_XY]
void Lcd_XY( char x, char y)
{

  // l'ascissa non puo' essere maggiore di LCD_CONF_MAX_X_CHARS
  x &= LCD_CONF_MAX_X_CHARS;

  // l'ordinata non puo' mai essere maggiore del num di linee
  if(y>LCD_CONF_MAX_Y_LINES)
    y=LCD_CONF_MAX_Y_LINES;

  curx=x;
  lcdline=y;
  
  Lcd_SendByte(LCD_FLAG_INSTRUCTION,lcdlines[y]|(x));
  mFourCyDelay();
}//end: _________________________________________________________________ Lcd_XY





// __________________________________________________________________ [Lcd_Putc]
/*!
\biref Manda un carattere all'lcd.
CHIEDERE DELUCIDAZIONI SUI "\f" E "\b". COSI' MESSO lcdline NON SERVE A
NIENTE
*/
void Lcd_Putc(char c) 
{
  

  switch (c) {
  
    case '\n':
	case '\r':
      curx=0;
      lcdline++;
      if(lcdline>LCD_CONF_MAX_Y_LINES)
        lcdline=0;     
      Lcd_XY(curx,lcdline);

      break;

    case 4: break;

    default: 
      //questo è il problema della stampa che salta di una riga
      //sull'LCD winstar: se x scavalla devo impostare lcdlines
      //altrimenti salta di una riga
      if((c<32)||(c>128)) return;
      if(curx>LCD_CONF_MAX_X_CHARS)
      {
        curx=0;
        
        lcdline++;
        if(lcdline>LCD_CONF_MAX_Y_LINES)
          lcdline=0;
        Lcd_XY(curx,lcdline);  /* */
      }
      Lcd_SendByte(1,c);     
      curx++;
      break;
  }
}//end: _______________________________________________________________ Lcd_Putc




// ________________________________________________________________ [Lcd_Printf]
/*!
\brief Implementa la scrittura di stringhe costanti predefinite.
\param string è un puntatore ad array di caratteri che devono essere
definiti in fase di compilazione.
*/
void Lcd_Printf(rom const unsigned char * stringa)
{
  while(*stringa) Lcd_Putc(*stringa++);
}// end: ____________________________________________________________ Lcd_Printf



// _______________________________________________________________ [Lcd_nPrintf]
/*!
\brief Implementa la scrittura di stringhe da array di chars
\param string e' un puntatore ad array di caratteri
\param len e' la lunghezza della stringa
*/
void Lcd_nPrintf(unsigned char *stringa, unsigned char len)
{
  unsigned char k;
  for(k=0;k<len;k++)
    Lcd_Putc(stringa[k]);
}// end: ___________________________________________________________ Lcd_nPrintf




void Lcd_CLS(void)
{
  Lcd_SendByte(LCD_FLAG_INSTRUCTION,LCD_CLEAR_DISPLAY);
  curx=0;
  lcdline=0;
  delayT0(T0_CONST_0002_MS);
}





#ifdef TEST_LCD

// _________________________________________________________ [Lcd_Test]
/*!
\brief Test dell'LCD
Parte dal primo carattere stampabile e finche' puo scrive a video
caratteri
*/
void Lcd_Test(void)
{
  char i,k,j;
  j=32;
  for(k=0;k<(LCD_CONF_MAX_Y_LINES+1);k++){
    for(i=0;i<(LCD_CONF_MAX_X_CHARS+1);i++){
      Lcd_XY(i,k);
      Lcd_Putc(j++);
      delayT0(T0_CONST_0010_MS);
    }
  }

}//end: ______________________________________________________ Lcd_Test
#endif


