#ifndef modulate_h
#define modulate_h
#include <Arduino.h>

#define pin_data  (13)              // saída do PWM (eletrodo)
#define pin_OnOff (12)
#define DEBUG 0

//------------------------------------software serial---------------------------------
//---------------------------------------Banda Base-----------------------------------

//**************************************** NRZ OOK ************************************
#define delayTime 200//200
#define interval  8 //interval at which to stop PWM (milliseconds)

void startBit_OOK();
void stopBit_OOK ();
size_t UART_OOK(int size, int8_t *buffer);
void OOK_2mod(int8_t b);

////*************************************** MANCHESTER ************************************

//define our manchester encoding pulse widths. Values below equate to 1200 baud (1/2 chip rate)
#define delay1 interval
#define delay2 interval
void UART_Man(int size, int8_t *buffer);
void startBit_MAN();
void stopBit_MAN();
void sendZero();
void sendOne();

#endif
