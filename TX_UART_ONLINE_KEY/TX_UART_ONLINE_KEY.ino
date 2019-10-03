#include "modulate.h"

void setup() {
  #if DEBUG
    Serial.begin(9600);
  #endif
  pinMode      (pin_data ,OUTPUT);
  pinMode      (pin_OnOff,OUTPUT);
  digitalWrite (pin_data ,LOW   );
  digitalWrite (pin_OnOff,HIGH  );
}

void loop() {
  int8_t data [] = {-108, 116, -72, -24, -57, 59, -54, 125};//, 83, 35, -111, 66, -13, -61, 18, 26};//Key 128 bits
  int8_t data1 [] = {83, 35, -111, 66, -13, -61, 18, 26};//Key 64 bits

  #if DEBUG
  unsigned long currentMillis = millis();
  #endif
  
  UART_OOK(sizeof(data) ,data); //1048 ms to send 128 bits key, StartbBit and StopBit 
  delay(delayTime);
  UART_OOK(sizeof(data1) ,data1);
  #if DEBUG
    Serial.print("Time: ");
    Serial.println(millis() - currentMillis);
  #endif

  delay(delayTime*3);  
}
