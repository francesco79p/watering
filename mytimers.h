#include <p18cxxx.h>



// -------------------------------------------------------------------- [TIMER0]
// lo deve decidere l'utente
#define T0_PRESCALER 256

#define GetCPUClock() 10000000
#define T0_CONST_2_100NS ( GetCPUClock() / T2_PRESCALER / T2_POSTSCALER )
#define T0_CONST_2_MS ( GetCPUClock() / T0_PRESCALER / 1000 )
#define T0_CONST_2_US ( GetCPUClock() / T0_PRESCALER / 1000000 )


#define T0_CONST_0650_NS    6 * T0_CONST_2_100NS
#define T0_CONST_0750_NS    7 * T0_CONST_2_100NS
#define T0_CONST_1500_NS   15 * T0_CONST_2_100NS

#define T0_CONST_0002_US    2 * T0_CONST_2_US
#define T0_CONST_0030_US   40 * T0_CONST_2_US
#define T0_CONST_0002_US    2 * T0_CONST_2_US
#define T0_CONST_0010_US   10 * T0_CONST_2_US
#define T0_CONST_0100_US  100 * T0_CONST_2_US
#define T0_CONST_1500_US 1500 * T0_CONST_2_US

#define T0_CONST_0100_MS  100 * T0_CONST_2_MS
#define T0_CONST_0002_MS    2 * T0_CONST_2_MS
#define T0_CONST_0005_MS    5 * T0_CONST_2_MS
#define T0_CONST_0015_MS   15 * T0_CONST_2_MS
#define T0_CONST_0010_MS   10 * T0_CONST_2_MS
#define T0_CONST_0020_MS   20 * T0_CONST_2_MS
#define T0_CONST_0050_MS   50 * T0_CONST_2_MS
#define T0_CONST_0075_MS   75 * T0_CONST_2_MS
#define T0_CONST_0250_MS  250 * T0_CONST_2_MS
#define T0_CONST_0500_MS  500 * T0_CONST_2_MS
#define T0_CONST_1000_MS 1000 * T0_CONST_2_MS


// -------------------------------------------------------------------- [TIMER1]

#define T1CON_CONF = 0b11110001
//                     |||||||+-> 0/1 :start/stop timer
//                     ||||||+--> : internal/external clock
//                     |||||+---> : don't sync/sync
//                     ||||+----> : on/off timer1 osc for ext. clock
//                     ||++-----> : prescaler 11=1:8, 00=1:1
//                     |+-------> : --
//                     +--------> : 16bit/8bit timer register loading




// -------------------------------------------------------------------- [TIMER2]
#define T2_PRESCALER 1
#define T2_POSTSCALER 1



#define mFourCyDelay()  Nop();Nop();Nop();Nop();


void delayT0(unsigned int uiTime);
void TIMER1_ISR(void);
void delayT2(unsigned char uchTime);
void delayNcy(unsigned char i);
void startT1(unsigned int imilli);



