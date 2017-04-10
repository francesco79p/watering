#include "i2cmod.h"




/**
 * Funzione che genera un delay
 * Il delay generato dipende dalla frequenza della CPU. Viene fornito come
 * parametro un intero positivo: quando raggionge 0 la funzione esce.
 * @param cnt indica quante volte si esegue il loop di wait:
 * a 80MHz il dt=12.5ns quindi il delay T=12.5ns * 10 * cnt
 */
void i2c_wait(unsigned int cnt)
{
    while(--cnt)
    {
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
    }
}




/**
 * Legge un byte dall'i2c
 * La funzione legge un byte dal bus i2c. La funzione è attualmente dedicata al
 * gyro in quanto non è possibile al momento parlare con l'accelerometro. per 
 * questo motivo l'indirizzo dello slave è fissato a quello del gyro.
 * La funzione mette il bus in idle state solo dopo l'apertura del bus e non
 * anche dopo la scrittura di un bus. Non viene utilizzata la funzione i2c wait.
 * @param Reg è il registro da cui leggere
 * @return la funzione  ritorna il byte letto.
 *
 * i2cData[3] contiene l'indirizzo slave e il registro da cui leggere. ci sono
 * 3 bytes perchè l'indirizzo dello slave una volta è in modalità lettura e una
 * volta in modalità scrittura.
 */


unsigned char i2c_WriteMany2(unsigned char Reg, unsigned char device, unsigned char *val, unsigned char n){

    char i2cData[3];
    char DataSz;
    char idx;
    char recv;
  
    recv=0;
    i2cData[0]=device;
    i2cData[1]=Reg;
    //i2cData[2]=val;
    DataSz = 3;
    idx=0;




    StartI2C();
    IdleI2C();
    WriteI2C(i2cData[0]);
    i2c_wait(250);
    WriteI2C(i2cData[1]);
    i2c_wait(250);
    
    while(idx<n){
        
        WriteI2C(val[idx]);
        i2c_wait(350);
        idx++;
    }

  

    IdleI2C();
    StopI2C();

    
    return 1;


}//END: i2c_WriteOne2




unsigned char i2c_WriteOne2(unsigned char Reg, unsigned char device, unsigned char val){

    char i2cData[3];
    char DataSz;
    char idx;
    char recv;

    recv=0;
    i2cData[0]=device;
    i2cData[1]=Reg;
    i2cData[2]=val;
    DataSz = 3;
    idx=0;




    StartI2C();
    IdleI2C();
    WriteI2C(i2cData[0]);
    i2c_wait(250);
    WriteI2C(i2cData[1]);
    i2c_wait(250);
    WriteI2C(i2cData[2]);

    IdleI2C();
    StopI2C();

    return recv;



    return 1;


}//END: i2c_WriteOne2


unsigned char i2c_ReadMany2(unsigned char Base,
                           unsigned char device,
                           unsigned char mask,
                           unsigned char offset,
                           unsigned char *buf){

    unsigned char i2cData[3];
    unsigned char len,ptr;

    i2cData[0]=device;
    i2cData[1]=Base;
    i2cData[2]=device | mask;
    ptr=0;
    len=7;


    StartI2C();
    IdleI2C();
    
    WriteI2C(i2cData[0]);
    i2c_wait(50);

    WriteI2C(i2cData[1]);
    i2c_wait(50);

	RestartI2C();
    IdleI2C();
    WriteI2C(i2cData[2]);
    i2c_wait(50);
    
    for(ptr=0;ptr<(len-1);ptr++)
    {
        
        buf[ptr]=ReadI2C();
        AckI2C();
        i2c_wait(50);

        
    }

    buf[ptr]=ReadI2C();
    NotAckI2C();
    IdleI2C();
    StopI2C();
   
    return ptr;


}// END: i2c_ReadMany






unsigned char i2c_ReadOne2(unsigned char Reg, unsigned char device, unsigned char mask){
/*
    char i2cData[3];
    char recv;

    recv=0;
    i2cData[0]=device;
    i2cData[1]=Reg;
    i2cData[2]=device | mask;


    StartI2C();
    IdleI2C();
    WriteI2C(i2cData[0]);
    i2c_wait(50);
    WriteI2C(i2cData[1]);
    
    RestartI2C1();
    IdleI2C1();
    MasterWriteI2C1(i2cData[2]);
    recv = MasterReadI2C1();

    NotAckI2C1();
    IdleI2C1();
    StopI2C1();


	 i2c_wait(50);
i2c_wait(50);
    return recv;


*/
}// END: i2c_ReadOne2

