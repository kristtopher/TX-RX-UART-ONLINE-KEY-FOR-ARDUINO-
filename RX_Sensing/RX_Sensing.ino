#include <Wire.h>
#include "Adafruit_ADS1015.h"
Adafruit_ADS1115 ads ;

int frameSize = 35;             // tamanho do pacote (***a alterar para dinamico *** conforme necessidade)
int delayTime = 30;
const long interval = 6;      //150ms interval at which to stop PWM (milliseconds)
String packet,data;
uint32_t skey[4];
int id;
float analogValue = 0.0;
float analogValue_i = 0.0;

void setup() {
  Serial.begin(9600);
  ads.begin();
}

void loop() {
  analogValue_i = ads.readADC_Differential_0_1()* 0.1875 /1000;
  
  while(true){
    //unsigned long currentMillis = micros();
    analogValue_i = ads.readADC_Differential_0_1()* 0.1875 /1000;
    //Serial.println(micros() - currentMillis); //1884
    Serial.println(analogValue_i);    
  }
  while(analogValue_i < 3.5 && analogValue_i > -3.5){
    delayMicroseconds(10);
    analogValue_i = ads.readADC_Differential_0_1()* 0.1875 /1000;
  }
//    delay(interval);
    demodulation();
    delay(460);
    analogValue_i = 0.0;
}

void demodulation() {
  int count = 0; 
  packet = "";
  while(count < frameSize){
    analogValue = ads.readADC_Differential_0_1()* 0.1875 /1000; // medida em Volts
    //Serial.println(analogValue);
    if (analogValue > 2.5) {
      packet = packet + '1';
      count ++;
      Serial.print("1");
      delay(interval);
    }
    else {//if(analogValue < -1.8){
      packet = packet + '0';
      count ++;
      Serial.print("0");
      delay(interval);
    }
  }
  Serial.println();
  id   = binToInt(packet.substring(0,3));
  data = packet.substring(3,35);
  skey[id] = bitArrayToInt32(data, data.length());
  Serial.print(id);
  Serial.print(F(" 0x"));
  Serial.println(skey[id],HEX);
}

int binToInt(String binString){
  if      (binString == "000") return 0;
  else if (binString == "001") return 1;
  else if (binString == "010") return 2;
  else if (binString == "011") return 3;
  else if (binString == "100") return 4;
  else if (binString == "101") return 5;
  else if (binString == "110") return 6;
  else if (binString == "111") return 7;
  // Error
  else return -1;
}

uint32_t bitArrayToInt32(String arr, int count){
    uint32_t ret = 0x0000;
    unsigned int p1 = 0x0000;
    unsigned int p2 = 0x0000;
    for (int i = 0; i < count/2; i++) {
        if(arr[i] == '1'){
          p1 |= 0x01 << (count/2 - i - 1);
        }
       else{
          p1 |= 0x00 << (count/2 - i - 1);
        }
    }
    for (int i = count/2; i < count; i++) {
      if(arr[i] == '1'){
        p2 |= 0x01 << (count - i - 1);
      }
     else{
        p2 |= 0x00 << (count - i - 1);
      }
    }
    ret |= p1;
    ret = ret << 16;
    ret |= p2;
    return ret;
}
