#include "modulate.h"

//---------------------------------------Banda Base-----------------------------------

//************************************** NRZ--OOK ************************************

void startBit_OOK(){
  digitalWrite (pin_OnOff,LOW);
  OOK_2mod(LOW);
}

void stopBit_OOK(){
  OOK_2mod(HIGH);
  OOK_2mod(HIGH);
  digitalWrite (pin_OnOff,HIGH);
}

size_t UART_OOK(int size, int8_t *buffer){
	startBit_OOK();
	while (size--) { 
		// Data bit transmission
		for(uint8_t mask = 1; mask; mask <<= 1){
			if(buffer[size] & mask){
				OOK_2mod(HIGH);
        #if DEBUG
				Serial.print('1');
        #endif
			}
			else{
				OOK_2mod(LOW);
				#if DEBUG
				  Serial.print('0');
        #endif
			}
		}
   #if DEBUG
    Serial.println();
   #endif
	}
	// Stop bit transmission
	stopBit_OOK();
	return size;
}

void OOK_2mod(int8_t b) {
  unsigned long currentMillis = millis();
  while (currentMillis + interval > millis())  //"espera ocupada"
    digitalWrite(pin_data,b);
}


unsigned long previousMillis = 0;
void ook_2mod(int state) {
  while (1) {
    unsigned long currentMillis = micros();
    digitalWrite(pin_data, state);
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      digitalWrite(pin_data, state - state);
      break;
    }

  }

}

////*************************************** MANCHESTER ************************************

void startBit_MAN(){
  digitalWrite (pin_OnOff,LOW);
  sendZero();
}

void stopBit_MAN(){
  sendOne();
  sendOne();
  digitalWrite (pin_OnOff,HIGH);
}

void sendZero(){
  delayMicroseconds(delay1);
  digitalWrite(pin_data, HIGH);

  delayMicroseconds(delay2);
  digitalWrite(pin_data, LOW);
}

void sendOne(){
  delayMicroseconds(delay1);
  digitalWrite(pin_data, LOW);

  delayMicroseconds(delay2);
  digitalWrite(pin_data, HIGH);
}

void UART_Man(int size, int8_t *buffer){
  //start bit
  //startBit_MAN();
  startBit_OOK();
  //send data
  while (size--) {
    // Data bit transmission
    for(uint8_t mask = 1; mask; mask <<= 1){
      if(buffer[size] & mask){
        sendOne();
        //Serial.print('1');
      }
    else{
        sendZero();
        //Serial.print('0');
    }//end of byte
  }//end of data - terminate with zero to be a good neighbour
  //Serial.println();
  }
  //stopBit_MAN();
  stopBit_OOK();
  //stopbit
}
