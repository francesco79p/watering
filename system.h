#ifndef SYSTEM_H
#define SYSTEM_H


typedef union _StatusReg
{
  struct{
  unsigned TASK         :7;///< Codice di errore che indica il task corrente
  unsigned done         :1;///< task completato


  unsigned fT1          :1;///< Flag che indica irq del timer1
  unsigned fUartRx      :1;///< Flag che indica irq UART ricezione
  unsigned fVALV        :1;
  unsigned fTm500       :1;///< Flag che indica il timer 500ms
  unsigned fAlarm       :1;
  unsigned fWatering    :1;
  unsigned fRB          :1;///< RBx interrupt
  unsigned fManualW     :1;///< vuoto	

  // gestione uarts
  unsigned fUBufWrap   :1;///< Il buffer ha iniziato d'accapo
  unsigned fUMsgOk     :1;///< Msg OK nel buffer            
  unsigned fUErr       :1;///< Errore nel modulo UART
  unsigned fURxOk      :1;///< Byte ricevuto correttamente dalla UART
  unsigned fTemLimits  :1;///< indica se ho superato il limite tre volte di tmax o tmin
  unsigned xxx         :3;

  unsigned fWLimitL    :8; ///< indica quale limite sono
    
  };
  struct{
    unsigned others3     :7;
    unsigned done        :1;///< Indica che l'azione corrente è completata
  };

  struct{
    unsigned char LSB;
    unsigned char MSB;
    unsigned char USB;
    unsigned char VSB;
    unsigned char XSB;
  };
} StatusReg;





/** Struttura per la data
    i suoi elementi sono tutti unsigned char.
*/
typedef union _strTime{
    struct{
        unsigned seu  :4;      ///< variabile dei secondi
        unsigned sed  :4;
        unsigned miu  :4;      ///< variabile dei minuti
        unsigned mid  :4;
        unsigned hou  :4;      ///< variabile delle ore
        unsigned hod  :4;
        unsigned yeu  :4;      ///< variabile dell'anno, nel formato yy ovvero solo ultimi due numeri
        unsigned yed  :4;
        unsigned mnu  :4;      ///< variabile del mese
        unsigned mnd  :4;
        unsigned dau  :4;      ///< variabile del giorno
        unsigned dad  :4;
    };
    struct{
        unsigned sec  :8;
        unsigned min  :8;
        unsigned hour :8;
        unsigned year :8;
        unsigned month:8;
        unsigned day  :8;
    };
} strTime;



/** Struttura per l'allarme
    i suoi elementi sono tutti unsigned char. Per il momento l'allarme è giornaliero
    e prevede solo ore e minuti.
*/
typedef struct{
    unsigned char min;      ///< variabile dei minuti
    unsigned char hour;     ///< variabile delle ore
} strAlarm;



/** Struttura del menu
    la struttura è l'elemento base di una linked list. Prevede infatti al suo interno
    un elemento *next che e' un puntatore
*/
typedef struct _menuItem{
    unsigned char id;               ///< elemento che indica l'ordinale della voce di menu
    rom const unsigned char *text;  ///< testo del menu. si recupera con l'ordinale all'interno di un'array di stringhe di tipo rom const 
    struct _menuItem * next;        ///< puntatore all'elemento menu' successivo: da inizializzare nel codice
} menuItem;





/*
typedef enum {
	S0_IDLE,
	S1_NORMAL,
	S2_TIMESET,
	S3_ALARM1S,
	S4_ALARM2S,
	S5_SHOWMENU
} state;

typedef enum {
	A0_NONE,
	A1_WORK,
	A2_TIMESET,
	A3_ALARM1SET,
	A4_ALARM2SET,
	A5_SHOWMENU_0
} action;

typedef enum {
	E0,
	E1_B1_PRESS,
	E2_B2_PRESS,
	E3_B1_B2_PRESS,
	E4_T0S5_ELAPSED
	E5_T25MS_ELAPSED
} event;

typedef struct {
	state nextState;
	action actionToDo;
}  stateElement;


*/
typedef enum {
    M_CHDISPLAY=0,
    M_ALARM1,
    M_ALARM2,
    M_SETUP,
    M_WATER
} menuItems;    


#define UART_HEADER '%'
#define UART_PLENGTH 16

#define TM1_FOSC
#define TM1_PS 8
#define TM1_MAX_COUNT 65536

#define TM1_FREQ   (TM1_FOSC / TM1_PS)
#define TM1_CORR 0
#define TM1_50MS (65535 - 62500)
#define TM1_20MS (65536 - 25000)
#define TM1_25MS (65536 - 31250)
#define TM1_10MS (65536 - 12500)
#define TM1_5MS  (65536 - 6250)
#define TM1_1MS  (65536 - 1250)

#define TM1_RES_TICS TM1_25MS
#define TM1_50HZ 5

#define TM1_TICS_2_1S0  (1000/1)
#define TM1_TICS_2_0S5  (500/1)
#define TM1_TICS_2_0S25 (250/1)


#define TM1_TICS_2_0S02 (20/1)
#define TM1_TICS_2_0S015 (15/1)
#define TM1_TICS_2_0S010 (10/1)
#define TM1_TICS_2_0S005 (5/1)

#define WLIMIT_5MIN 600
#define WLIMIT_4MIN 480
#define WLIMIT_3MIN 360
#define WLIMIT_2MIN 240
#define WLIMIT_1P5MIN 120
#define LIGHTLIMIT 60
#define WSHORTLIMIT 120

#endif //SYSTEM_H

