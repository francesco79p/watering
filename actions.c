#include "actions.h"
#include "mytimers.h"
#include <p18cxxx.h>
#include "HardwareProfile.h"
#include <stdio.h>
//#include "printf.h"
#include "mytimers.h"
#include "i2clcd.h"
#include <usart.h>
#include "system.h"
#include <adc.h>
#include "i2cmod.h"

unsigned int iCapTime=0;
extern int tacs;





unsigned char checkAlarms(strAlarm a1, strAlarm a2, strTime t)
{

    if ((a1.hour == t.hour)&&(a1.min == t.min)&&(t.sec<5))
		return 1;
	if ((a2.hour == t.hour)&&(a2.min == t.min)&&(t.sec<5))
        return 2;
    else
        return 0;

}




/** Lettura dell'ora
    Funzione che legge l'ora, non ha parametri e ritorna la struttura strTime.
    \callgraph
*/
strTime readTime(void)
{
    strTime myT;
	unsigned int i=0;
	unsigned char ch[7];

	    
    i=i2c_ReadMany2(0x00,DSADDR,0x01,0x00,ch);
    Nop();
    
 
    myT.sec = ch[0];
    myT.min = ch[1];
    myT.hour= ch[2];
    myT.day = ch[4];
    myT.month=ch[5];
    myT.year= ch[6];
          
    return myT;
}



void start_clock(strTime *myT){

        unsigned char dec, uni;
        
        
        
        i2c_WriteOne2(0x00,DSADDR,0);//secondi
        delayNcy(100);
          
        i2c_WriteOne2(0x01,DSADDR,myT->min); //minuti
        delayNcy(100);
        
        i2c_WriteOne2(0x02,DSADDR,myT->hour); //ore
        delayNcy(100);
        
        i2c_WriteOne2(0x06,DSADDR,myT->year); //anno
        delayNcy(100);
        
        i2c_WriteOne2(0x05,DSADDR,myT->month); //mese
        delayNcy(100);
        
        i2c_WriteOne2(0x04,DSADDR,myT->day); //giorno
        delayNcy(100);


}



char iReadTemperature(void)
{
    unsigned int b=0;
    char index;
	int temperatura;
	
	

    //SetChanADC(ADC_CH1);
    ConvertADC();
    while(BusyADC());
    b = ADRES;

    temperatura = (char)(  ((float)(b)-187.5)/2.5 + 25.0   );
    
    return temperatura;

}


void update(StatusReg * _pic, char temp, char tmax, char tmin, strTime t, unsigned int k, int wr)
{

	unsigned char buffer[24];
  
    /// col cls sfarfalla lo schermo
    //clear_screen();
    LCD_HOME();
    sprintf(buffer,"T:%2dC,%2d/%2d,hr:%d WL:%3d",temp, tmax,tmin,k, wr);      
    //sprintf(buffer,"T:%2dC,%2d/%2d,hr:%d WL:%3d",temp, tmax,tmin,tacs, wr);
    Lcd_nPrintf(buffer,24);
    
    Lcd_XY( 0, 1);
    if(_pic->fWatering==0)
      sprintf(buffer,"%d%d:%d%d:%d%d %d%d/%d%d/%d%d   SW:%d",t.hod,t.hou,t.mid,t.miu,t.sed,t.seu,t.yed,t.yeu, t.mnd,t.mnu, t.dad,t.dau,_pic->fVALV);
    else
      sprintf(buffer,"%d%d:%d%d:%d%d tmout= %d        SW:%d",t.hod,t.hou,t.mid,t.miu,t.sed,t.seu,tacs,_pic->fVALV);
    Lcd_nPrintf(buffer,24);

}



void manageMenu(StatusReg * _pic, menuItem ** _cm, unsigned char keyb){

    	
    	switch(keyb){
        
            case 1:
                /// il bottone 1 corrisponde ad enter, quindi se non faccio nulla
                /// allora entro nel menu
                if(_pic->TASK==TASK_IDLE){
                    _pic->TASK=TASK_MENU;
                    display_menu(*_cm);
                    return;
                }

                if(_pic->TASK==TASK_MENU){
                   _pic->TASK=TASK_SUBMENU;
                }

                
            break;
            
            case 2:
                /// se premo bottone 2 allora scorro le voci del menu.
                //clear_screen();
                if((_pic->TASK==TASK_MENU)){
                    *_cm = (*_cm)->next;
                    display_menu(*_cm);//next_menu();
                    
                }
         
            break;
            
            case 0:            
                _pic->TASK=TASK_IDLE;
                
            break;
        
        }
	    
	
	
	
}



void doMenuActions(StatusReg * _pic, menuItem ** _cm, strTime *t, unsigned char keyb){

    
	switch((*_cm)->id){
	
		case M_CHDISPLAY:
		case M_ALARM1:
		case M_ALARM2:
			_pic->TASK=TASK_IDLE;
			break;
		case M_SETUP:
			if(keyb==2)
			_pic->TASK=TASK_SETDATE; 
			break;
		
		case M_WATER:
			_pic->TASK=TASK_IDLE;
			_pic->fManualW=1;    
			break;
		
		default:
			_pic->TASK=TASK_IDLE;
			break;
	
	}
	
}





void doDate2(StatusReg * _pic,  strTime *t, unsigned char keyb){
    unsigned char buffer[24];
    unsigned char *dateElem;
    unsigned char d,u;
    char dateTxt;
    unsigned char k;
    
    while(1){
        k=4;
    
              
                
        if(_pic->fRB){
    	    delayT0(500);
            k = (PORTB & 0x03);
            _pic->fRB = 0;
            clear_screen();
        }	        
    
    
    
        switch(_pic->TASK){
        
            case TASK_SETDATE:
            case TASK_SETYEAR:
                dateElem=(&t->year);
                dateTxt=0;
                break;
            
            case TASK_SETMONTH:
                dateElem=(&t->month);
                dateTxt=1;
                break;
           case TASK_SETDAY:
                dateElem=(&t->day);
                dateTxt=2;
                break;
           case TASK_SETHOUR:
                dateElem=(&t->hour);
                dateTxt=3;
                break;
           case TASK_SETMIN:
                dateElem=(&t->min);
                dateTxt=4;
                break;
           
           
           
           
        }
        
        u = (*dateElem) & 0x0F;
        d = ((*dateElem) & 0xF0) >> 4;
        
        if(k==1){
            u++;
            if(u==10){
                u=0;
                d++;
            }    
                
        }    
        if(k==2){
            u--;
            if(u==255){
               u=9;
               d--;
            }    
                
        }
        
        if(_pic->fTm500)
          clear_screen();
        Lcd_XY( 0, 0);
        
        (*dateElem)= (d<<4)|(u);
    
        str2ram(buffer,_ldate[dateTxt]);
        Lcd_Printf(buffer);
        sprintf(buffer,"%d%d\0", d,u);
        Lcd_Printf(buffer);
        
        
        if(k==0){
            if(_pic->TASK<=TASK_SETMIN)
            _pic->TASK++;
            else{
                _pic->TASK=TASK_IDLE;
                start_clock(t);
                return;
            }    
            
        }  
            _asm CLRWDT _endasm; //se mi incastro allora sblocco    
    }//end while 1
}


void readSensors(StatusReg * _pic, char * temp, char tmax, char tmin, strTime *t, unsigned int *hr, int wr){
    char tmp,i;
    float fhr;
    
    tmp= *temp;
    
    
    *temp = iReadTemperature();
    *t = readTime();
    

    //capture module
	CCP1CON = 0b00000111; //capture mode every rising edge
    T3CON   = 0b11000101; //timer3 16bit mode, for bothCCP modules, 1:1 prescaler, Fosc/4, ON  
    CCPR1=0;
  	TMR3H=0;
    TMR3L=0;  
    PIR1bits.CCP1IF=0;
    delayT0(100);
    
    for(i=0;i<8;i++){
    while(PIR1bits.CCP1IF==0){
        
    }
    PIR1bits.CCP1IF=0;
    iCapTime+=CCPR1;
    iCapTime>>=1;
    TMR3H=0;
    TMR3L=0;
    }
    CCP1CON=0;
    T3CON=0;
    //
    
    //iCapTime>>=1;


    
    fhr=(float)( 0.0264*((float)iCapTime) - 585.0);
    *hr=(unsigned int )fhr;   
    
    //if(_pic->fTm500)
    
   //update(_pic, *temp, tmax, tmin, *t, (unsigned int )fhr, wr);
}




void display_menu(menuItem * _cm){
    char i;
    menuItem *tm;
    unsigned char buffer[23];
    
	clear_screen();
    tm=_cm;
    
    for(i=0;i<2;i++){
        Lcd_XY(0,i);
        str2ram(buffer,tm->text);
        if(i==0)
            buffer[2]='>';
        Lcd_nPrintf(buffer,20);
        //Lcd_Printf(buffer);
        tm=tm->next;
       
    }
    
    
}



void clear_screen(void){
    LCD_CLS();
    LCD_HOME();
}




void str2ram(static char *dest, static char rom *src)
{
  while ((*dest++ = *src++) != '\0')
    ;
}





