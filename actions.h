#include "system.h"
#include "i2clcd.h"

typedef enum{
    TASK_IDLE      ,
    TASK_MENU      ,
    TASK_ALARM1_SET,
    TASK_ALARM2_SET,
    TASK_LCD_INIT  ,
    TASK_LCD_OP    ,
    TASK_SUBMENU   ,
    TASK_SETDATE   ,
    TASK_SETA1     ,
    TASK_SETA2     ,
    TASK_SETYEAR   ,
    TASK_SETMONTH  ,
    TASK_SETDAY    ,
    TASK_SETHOUR   ,
    TASK_SETMIN    ,
	TASK_WU
}tasks;

/*
typedef enum {
	M1_CH_DISPLAY,
	M2_ALARM1,
	M3_ALARM2,
	M4_SETUP,
	M5_WATER
} state;
*/

#pragma romdata datetxt
extern rom const unsigned char _ldate[][8];
extern rom const  char temptable[61];
#pragma romdata

void manageMenu(StatusReg * _pic, menuItem ** _cm, unsigned char keyb); 
void doMenuActions(StatusReg * _pic, menuItem ** _cm, strTime *t, unsigned char keyb);  
void doDate(StatusReg * _pic, menuItem ** _cm, strTime *t, unsigned char keyb);
void doDate2(StatusReg * _pic,  strTime *t, unsigned char keyb);
void readSensors(StatusReg * _pic, char * temp, char tmax, char tmin, strTime *t, unsigned int *hr, int wr);
void str2ram(static char *dest, static char rom *src);
void clear_screen(void);
void start_clock(strTime *myT);
void display_menu(menuItem * _cm);



void update(StatusReg * _pic, char temp, char tmax, char tmin, strTime t, unsigned int k, int wr);
unsigned char checkAlarms(strAlarm a1, strAlarm a2, strTime t);
//void saveAlarms(strAlarm a1, strAlarm a2);
char iReadTemperature(void);
strTime readTime(void);
