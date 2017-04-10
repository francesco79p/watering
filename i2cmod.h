/* 
 * File:   i2cmod.h
 * Author: Francesco
 *
 * Created on 21 May 2013, 19:14
 *
 * In questa sezione sono definite le costanti del giroscopio ST L3G400D
 * The L3G4200D is a low-power three-axis angular rate sensor able to provide
 * unprecedented stablility of zero rate level and sensitivity over temperature
 * and time.
 * Ha tre range di funzionamento a 250/500/2000 degree per second.
 * L'output è in complemento a 2 a 16bit quindi il massimo (positivo o negativo)
 * corrisponde a 32k.
 * Per il momento lo uso in modalità BYPASS. In questo modo nei registri c'è
 * sempre il valore corrente letto sui tre assi. Ci sono anche la modalità FIFO
 * e la modalità straming.
 *
 */

#ifndef I2CMOD_H
#define	I2CMOD_H



#include <p18cxxx.h>
#include "HardwareProfile.h"
#include <i2c.h>

#define Fsck        100000


/// Valore del registro BRG che determina la velocità del bus 100kHz per questo Gyro
#define BRG_VAL     (GetPeripheralClock()/2/Fsck) 





void i2c_wait(unsigned int cnt);
unsigned char i2c_ReadOne2(unsigned char Reg, unsigned char device, unsigned char mask);
unsigned char i2c_ReadMany2(unsigned char Base, unsigned char device, unsigned char mask, unsigned char offset, unsigned char *buf);
unsigned char i2c_WriteOne2(unsigned char Reg, unsigned char device, unsigned char val);
unsigned char i2c_WriteMany2(unsigned char Reg, unsigned char device, unsigned char *val, unsigned char n);



#endif	/* I2CMOD_H */
