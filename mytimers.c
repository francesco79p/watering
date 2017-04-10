#include "mytimers.h"
#include "system.h"

unsigned int uchT1tics;
unsigned int iLedtics;

void startT1(unsigned int imilli){
  PIR1bits.TMR1IF=0;
	PIE1bits.TMR1IE=1;
	IPR1bits.TMR1IP=1;
  T1CON = 0b11110001;
  TMR1H=(TM1_RES_TICS & 0xFF00)>>8;
	TMR1L=(TM1_RES_TICS & 0x00FF);
}


// ___________________________________________________________________ [delayT0]
void delayT0(unsigned int uiTime)
{
  unsigned int temp;
  temp = 0xFFFF - uiTime;
  
  TMR0H = (unsigned char)((temp>>8) & 0x00FF);
  TMR0L = (temp & 0x00FF);

  T0CON = 0b10000111;
;  while(INTCONbits.TMR0IF==0){}
  T0CON = 0b00000111;// il settimo bit ferma il TIMER.
  INTCONbits.TMR0IF=0;
  
}// end: _______________________________________________________________ delayT0





// ___________________________________________________________________ [delayT2]
void delayT2(unsigned char uchTime)
{
  unsigned char temp;
  temp = 0xFF - uchTime;
  
  TMR2 = (temp);

  T2CON = 0b00000100;
  while(PIR1bits.TMR2IF==0){}
  T2CON = 0b00000000;// il settimo bit ferma il TIMER.
  PIR1bits.TMR2IF=0;
  
}// end: _______________________________________________________________ delayT2

// __________________________________________________________________ [delayNcy]
/*!
\brief Delay proporzionale a n Cicli di clock
La funzione delayNcy, realizza un delay che e' proporzionale a i cicli di clock
ma il setup delle variabili e la chiamata a funzione, aggiungono un delay
ulteriore: per avere delay piu' precisi occorre una verifica con l'oscilloscopio

\param unsigned char i: n. di cicli da attendere.
*/
void delayNcy(unsigned char i)
{
  while(i--){Nop();}
}// end: ______________________________________________________________ delayNcy
