
/*******************************************************************************
                            F U S E    B I T S 
 ******************************************************************************/
#pragma config OSC     = HSPLL
#pragma config PWRT    = OFF
#pragma config BOREN   = OFF
#pragma config BORV    = 0
#pragma config WDT     = ON
#pragma config WDTPS   = 512
#pragma config CCP2MX  = PORTC
#pragma config STVREN  = OFF
#pragma config LVP     = OFF
#pragma config DEBUG   = OFF
#pragma config PBADEN  = OFF
#pragma config MCLRE   = OFF
#pragma config LPT1OSC = OFF
#pragma config XINST   = OFF


#pragma  config CP0 = OFF
#pragma  config CP1 = OFF
//#pragma  config CP2 = OFF
//#pragma  config CP3 = OFF
#pragma  config CPB = OFF
#pragma  config CPD = OFF

#pragma config WRT0 = OFF
#pragma config WRT1 = OFF
//#pragma config WRT2 = OFF
//#pragma config WRT3 = OFF
#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD = OFF


#pragma config EBTR0 = OFF
#pragma config EBTR1 = OFF
//#pragma config EBTR3 = OFF
#pragma config EBTRB = OFF






/*******************************************************************************
                            I N C L U D E
 ******************************************************************************/
#include <p18cxxx.h>
#include "HardwareProfile.h"
#ifdef USE_SHORT_PRINTF
  #include "printf.h"
#endif

  #include <stdio.h>

#include "mytimers.h"
#include "i2clcd.h"
#include <usart.h>
#include "system.h"
#include <pwm.h>
#include <adc.h>
#include "i2cmod.h"
#include "actions.h"


/*******************************************************************************
                            C O N S T    R O M S
 ******************************************************************************/
#pragma romdata menu_data

rom const unsigned char _menu[][21]={"   1.Ch. Display   \0",
                                     "   2.Alarm1        \0",
                                     "   3.Alarm2        \0",
                                     "   4.Setup         \0",
                                     "   5.Water         \0"};

rom const unsigned char _ldate[][8]={"year:\0",
                                     "month:\0",
                                     "day:\0",
                                     "hour:\0",
                                     "min.:\0"};
                                                        
rom const  char temptable[61]={-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};
                  
#pragma code




/*******************************************************************************
                      F U N C T I O N    P R O T O T Y P E S
 ******************************************************************************/
void init(void);
void YourHighPriorityISRCode();
unsigned int AdcAcquire(void);
void UART_ISR( void );
void main(void);
void resumeSleep(void);
void prepareSleep(void);




/*******************************************************************************
                             D E C L A R A T I O N S 
																  G L B A L S 
 ******************************************************************************/

volatile StatusReg PIC;
unsigned char i;
int tacs=0;
unsigned char tics=0;
strAlarm myA1, myA2;
unsigned char buffer[24];

char temperature, tmax, tmin;
strTime myT;
menuItem m[5];
menuItem *cm;
unsigned char * ptrRomDate;
int wateringLimit, hrLimit;
unsigned char lightingLimit, fwHRLimit;
//volatile unsigned int iCapTime=0;
float fhr=0.0;
unsigned int hr;

/*******************************************************************************
                      I N T E R R U P T    V E C T O R S
 ******************************************************************************/
#pragma code RESET_VECTOR = 0x00


#pragma code HIGH_INTERRUPT_VECTOR = 0x08
void Remapped_High_ISR (void)
{
  _asm goto YourHighPriorityISRCode _endasm
}


#pragma code LOW_INTERRUPT_VECTOR = 0x18
void Remapped_Low_ISR (void)
{
  //_asm goto YourLowPriorityISRCode _endasm
	Nop();
}
/**/



/*******************************************************************************
             I N T E R R U P T    V E C T O R S    H A N D L E R S
 **************************************************************************** */
#pragma code

#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode()
{
  /// disabilita irq globali
  INTCONbits.GIE=0;
  INTCONbits.PEIE=0;
  

  /// controllo se int externo
  if(INTCONbits.INT0IF || INTCON3bits.INT1IF){
    INTCONbits.INT0IF = 0;
    INTCON3bits.INT1IF = 0;
    
    /// se si abilita flag nel registro di controllo
    PIC.fRB=1;
  }
  
   
  /// controlla se scaduto int di TIMER1
  if(PIR1bits.TMR1IF){
  
    PIR1bits.TMR1IF=0;             
    
    ///se si reimposta timout e setta flag nel registro di controllo
		TMR1H=(TM1_RES_TICS & 0xFF00)>>8;
		TMR1L=(TM1_RES_TICS & 0x00FF);
		PIC.fT1 = 1;
    
  }


  /// riabilita irq globali
  INTCONbits.GIE=1;
  INTCONbits.PEIE=1;

}









/*******************************************************************************
                              C O D E     S E C T I O N
 ******************************************************************************/
#pragma code


// ______________________________________________________________________ [MAIN]
/*! 
\brief Ciclo Main



*/
void main(void)
{
  unsigned char keyb=0;
	unsigned char checkA=0;

  init();
  initLCD();
    
 
  /// allarmi di default
  myA1.hour = 0x06;
  myA1.min  = 0x30;
  myA2.hour = 0x20;
  myA2.min  = 0x30;

#ifdef MANUAL_TIME_SETUP    
  myT.sec=0;
  myT.min=0;
  myT.hour=0;
  myT.year=0;
  start_clock(&myT);
#endif

  /// inizializzazione della struttura del menu
  m[M_CHDISPLAY].id=M_CHDISPLAY;    
  m[M_CHDISPLAY].next=&m[M_ALARM1];    
  m[M_CHDISPLAY].text=_menu[M_CHDISPLAY];

  m[M_ALARM1].id=M_ALARM1;          
  m[M_ALARM1].next=&m[M_ALARM2];    
  m[M_ALARM1].text=_menu[M_ALARM1];

  m[M_ALARM2].id=M_ALARM2;    
  m[M_ALARM2].next=&m[M_SETUP];    
  m[M_ALARM2].text=_menu[M_ALARM2];

  m[M_SETUP].id=M_SETUP;    
  m[M_SETUP].next=&m[M_WATER];    
  m[M_SETUP].text=_menu[M_SETUP];

  m[M_WATER].id=M_WATER;    
  m[M_WATER].next=&m[M_CHDISPLAY];    
  m[M_WATER].text=_menu[M_WATER];
    

  cm=&m[M_CHDISPLAY]; /// init puntatore al primo elemento della struttura del menu

  wateringLimit = 5; //5 min default watering time
  lightingLimit = 0; // 1minuto
  temperature = iReadTemperature();;
  tmax=-128;
  tmin=127;

  
  /// stampa di benvenuto all'accensione
  
  
  /**
    normalmente è in sleep e viene mostrato umidità e temperatura
    mentre quando si preme un tasto viene visualizzato orologio
    e sunto delle acquisizioni, stessa cosa quando scatta allarme per irrigazione
  */  
  // Nel ciclo principale,
  while(1){
      
		if(!(T1CON&0x0001))
			startT1(500);


		if(PIC.fT1)     
		{
			// controllo quando è passato 0.5
			if(++tics >= 20)
			{
				PIC.fTm500 = 1;
				tics=0;
			}		
			PIC.fT1=0;
		}// end if PIC.fTm1  
      
		// se viene premuti un tasto accende la backlight
		if(PIC.fRB)
		{
			PIC.fRB = 0;
			            
			/// delay per debounce          
			delayT0(200);      
			
			/// tasti mappati su RB0 e RB1.
			keyb = (PORTB & 0x03);
			
			/// comunque se si e' premuto un tasto accende la luce   ma solo
			/// se non è un irq sqpurio
			if (!(keyb==3)){
				poLIGHT=1;
			}  
			
			manageMenu(&PIC, &cm, keyb);       
			/// tutto questo va gestito nel manage menu o comunque senza if nel main
			if( PIC.TASK == TASK_SUBMENU ){
			  doMenuActions(&PIC, &cm, &myT, keyb);
			}
			
			if(PIC.TASK==TASK_SETDATE){
			  doDate2(&PIC, &myT, keyb);
			}
		
		}
             

      
      if(PIC.TASK==TASK_IDLE){
        /// acquisisce tutti i sensori
        readSensors(&PIC, &temperature, tmax, tmin, &myT, &hr, wateringLimit);
        //update(&PIC, &temperature, tmax, tmin, myT, (unsigned int )hr, wateringLimit);     

	      ///elaborateData(&myT, &strTemp);
	      //if(1){
	      if(PIC.fWatering==0){      

	        // dopo la mezzanotte resetta le statistiche  delle temperature
	        if(myT.hour==0){
	          tmax=-128;
	          tmin=127;
	        }
	        
	        // aggiorna Tmax e Tmin
	        if(temperature>tmax)
	            tmax=temperature;
	        
	        if(temperature<tmin)
	            tmin=temperature;
	        
	        /// HR non può essere maggiore di 90 per i nostri conti per il calcolo 
	        /// dei tempi di irrigazione.
	        if(hr>90)
	            hr=90;
	
	        /// funzione di calcolo del peso della HR sui tempi di irrigazione    
	        hrLimit=270-3*(hr);
	
	        /// funzione che calcola il tempo di irrigazione in base a Temp e HR
	        /// se non sto già irrigando
	        //if(PIC.fWatering==0){   
	          wateringLimit=((int)temperature)*10+120+hrLimit;
	          wateringLimit >>=1; //in secondi       
	        //}

	      }// end If Watering==0
      }//end If TASK_IDLE
                        
      

      
      /// controlla allarmi e setta il flag se è il momento e non ho mai
      /// premuto un tasto allora devo anche far partire il timer se no
      /// non si ferma più
      checkA = checkAlarms(myA1, myA2,myT);
      if( checkA && (PIC.fAlarm==0) && (PIC.fWatering==0)){
        PIC.fAlarm = 1;
      }

      /// se e' il momento allora parte irrigazione
      if(PIC.fAlarm)
      {
          poVALV = 1; //accende
          PIC.fVALV=1;
          PIC.fWatering =1;
          PIC.fAlarm = 0;  
          
      }

      /// se ho richiesto attivazione manuale      
      if(PIC.fManualW==1)
      {
        wateringLimit=120;//WSHORTLIMIT;
        PIC.fManualW=0;
        PIC.fWatering=1;
        //PIC.fAlarm = 1;
        poVALV = 1; //accende
        //PIC.fVALV=1;
//        if(!(T1CON&0x0001))
//            startT1(500);
      }

      

      
	/// ogni 500ms controllo che sia scaduto timeout utente
	if(PIC.fTm500){  

		/// ogni 500ms se non devo gestire interazioni con l'utente
		/// se sto irrigando ed è scaduto il tempo allora spengo
		//if( PIC.fTm500 && PIC.TASK==TASK_IDLE &&  PIC.fWatering==1 && ++tacs==wateringLimit){        
		if(  PIC.fWatering==1 && ++tacs==wateringLimit){  
			poVALV = 0;
			PIC.fVALV=0;
			PIC.fWatering = 0;
			tacs=0;
			T1CON=0;
			//wateringLimit=WLIMIT;
		}

		
		if(++lightingLimit==LIGHTLIMIT){
			poLIGHT=0;
			lightingLimit=0;  
			T1CON=0;
		}    
        
        /// se sto innaffiando aggiorno ogni mezzo secondo
        if(PIC.fWatering){
          update(&PIC, temperature, tmax, tmin, myT, (unsigned int )hr, wateringLimit);
        }

        PIC.fTm500=0;
      }//end if 500ms
  	

      /// se non sto irrigando e non interagisco con l'utente       
      if( PIC.fWatering==0 && PIC.TASK==TASK_IDLE){                                    
        update(&PIC, temperature, tmax, tmin, myT, (unsigned int )hr, wateringLimit);
        prepareSleep();
        _asm sleep _endasm;
        
        /// da qui riprende l'esecuzione al risveglio dallo sleep  
        /// anche se scatta il WDT si riparte da qui
        Nop();
		Nop();
        resumeSleep();
      }
      else{
        _asm CLRWDT _endasm;
      }
            
  }//end while
    
    
}//end main









/*******************************************************************************
                     S U P P O R T    F U N C T I O N S
 **************************************************************************** */









// ______________________________________________________________________ [init]
/*! 
\brief Configurazione di sistema

*/
void init(void){

	// abilita priorità su irq
	RCONbits.IPEN=1;

	resumeSleep();

	// system register (defined by user for convenience)
	PIC.LSB = 0;
	PIC.MSB = 0;
	PIC.USB = 0;
	PIC.VSB = 0;
	PIC.XSB = 0;


	// -----------------------
	// interrupts ON
	
	INTCON2bits.INTEDG0=0;  /// falling edge su RB0
	INTCON2bits.INTEDG1=0;  /// falling edge su RB1
	
	INTCONbits.INT0IF=0;   /// clear the flag
	INTCONbits.INT0IE=1;  /// interrupt abilitato
	
	INTCON3bits.INT1IP=1;
	INTCON3bits.INT1IF=0;
	INTCON3bits.INT1IE=1;
		
	INTCONbits.GIE=1;
  INTCONbits.PEIE=1;
  


}

void resumeSleep(void){
	
	/// PORTB tutta in output e i 4 bit + significativi dedicati all'LCD
	PORTB=3;
	TRISB=3;
	PORTBbits.RB0=1;
	PORTBbits.RB1=1;

#ifdef PIC18F452
	/// PORTD tutta in output e i 4 bit + significativi dedicati all'LCD
	PORTD=0;
	TRISD=0;
	PORTE=0;
	TRISE=0;
#endif


	PORTA=0xff;
	TRISA=0xff;

	PORTCbits.RC2=1;
	TRISCbits.TRISC2=1;
	PORTCbits.RC1=0;
	TRISCbits.TRISC1=0;
	
	PORTCbits.RC0=0;
	TRISCbits.TRISC0=0;
	
		
	//i2c
  PORTCbits.RC3=1;
	TRISCbits.TRISC3=0;
	PORTCbits.RC4=1;
	TRISCbits.TRISC4=0;


	// ADC CONFIGURATION
	ADCON0 = 0x05;
	ADCON1 = 0x1D;
  ADCON2 = 0xF1;


	
	// -----------------------
	// TIMER 1 configuration
	// -----------------------

	T1CON=0;

	T1CONbits.RD16=0;     // abilita scrittura a 16 bit
	T1CONbits.TMR1ON=0;	  // spento
	
	T1CONbits.T1CKPS1=1;  // prescaler divide per 8 10MHz/8 = 1.25 MHz
	T1CONbits.T1CKPS0=1;

	/*
	PIR1bits.TMR1IF=0;
	PIE1bits.TMR1IE=1;
	IPR1bits.TMR1IP=1;
	TMR1H=0;
	TMR1L=0;
	T1CONbits.TMR1ON=1;	  // accensione
	*/
  PIR1bits.CCP1IF=0;
	PIE1bits.CCP1IE=0;
	IPR1bits.CCP1IP=0;
	CCP1CON = 0b00000111; //capture mode every rising edge
  T3CON   = 0b11000101; //timer3 16bit mode, for bothCCP modules, 1:1 prescaler, Fosc/4, ON  
  CCP2CON = 0;
    

	// ---------------------
	// I2c
	OpenI2C(MASTER, SLEW_OFF);
	SSPADD = 0x3f;
	IdleI2C();
	
}



void prepareSleep(void){

	PORTA=0xFF;
	TRISA=0xff;
	PORTB=0xFF;
	TRISB=0xff;
	PORTC=0xfc;
	TRISC=0xFc;
    

	// -----------------------
	// ADC CONFIGURATION
	// -----------------------
	//OpenADC( ADC_FOSC_64 &  ADC_RIGHT_JUST,  ADC_CH1 &  ADC_INT_OFF, ADC_VREFPLUS_EXT);
	ADCON0 = 0x0;
	ADCON1 = 0x0;
    ADCON2 = 0x0;


	
	// -----------------------
	// TIMER 1 configuration
	// -----------------------

	T1CON=0;
	CCP1CON = 0; //capture mode every rising edge
    T3CON   = 0; //timer3 16bit mode, for bothCCP modules, 1:1 prescaler, Fosc/4, ON  
    CCP2CON = 0;
    
}





// _______________________________________________________________ [AdcAcquire]
/*! 
\brief Gestione della acquisizione.
Acquisice dal solo canale AN0 e ritorna l'intero letto a meno dei 
2bit meno significativi
*/
unsigned int AdcAcquire(void)
{
    unsigned char BYTELOW, BYTEHIGH;
    
    ADCON0bits.GO=1;
    while(ADCON0bits.DONE){;}
    BYTEHIGH = ADRESH;
	return BYTEHIGH;

}
// end: ____________________________________________________________ AdcAcquire



