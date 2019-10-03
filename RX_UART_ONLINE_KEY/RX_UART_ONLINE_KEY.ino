#include <Wire.h>
#include "Adafruit_ADS1015.h"
#define DEBUG 1
#define WAVE 1
Adafruit_ADS1115 ads ;

float v[3];

int keyLen = 64;
int startBit;
int frameSize = keyLen + 3;//bits
int sframeSize = frameSize - 2;
uint32_t skey[4];

int delayTime = 50;
const long interval = 5903;//6000
String packet,data, subS;
float analogValue = 0.0;
float analogValue_i = 0.0;

void setup() {
  #if DEBUG
    Serial.begin(9600);
  #endif
  ads.begin();
}

void loop() {
  analogValue_i = ads.readADC_Differential_0_1()* 0.1875 /1000;
  #if WAVE
  while(true){
    analogValue_i = ads.readADC_Differential_0_1()* 0.1875 /1000; //time to read 1884us
    Serial.println(analogValue_i);    
  }
  #endif

  while(analogValue_i < 3.5 && analogValue_i > -3.5){
    delayMicroseconds(10);
    analogValue_i = ads.readADC_Differential_0_1()* 0.1875 /1000;
  }
    demodulation();
    delay(delayTime);
    analogValue_i = 0.0;
}

void demodulation() {
  int count = 0; 
  packet = "";
  while(count < frameSize){
    analogValue = ads.readADC_Differential_0_1()* 0.1875 /1000; // medida em Volts
    if (analogValue > 2.5) {
      packet = packet + '1';
      count ++;
      #if DEBUG
        Serial.print("1");
      #endif
      delayMicroseconds(interval);
    }
    else {
      packet = packet + '0';
      count ++;
      #if DEBUG
        Serial.print("0");
      #endif
      delayMicroseconds(interval);
    }
  }
  #if DEBUG
    Serial.println();
  #endif
  mountPackage(packet, keyLen);
}

void mountPackage(String packet, int keyLen){
  if(keyLen == 32){
    data = rotateBit(packet.substring(1,sframeSize), packet.substring(1,sframeSize).length());
    skey[0]= bitArrayToInt32(data, data.length());
    Serial.print(F(" 0x"));
    Serial.println(skey[0],HEX);
  }

    else if(keyLen == 64){
    data = rotateBit(packet.substring(sframeSize - 32,sframeSize), packet.substring(sframeSize - 32,sframeSize).length());
    skey[0]= bitArrayToInt32(data, data.length());
    data = rotateBit(packet.substring(sframeSize - 64,sframeSize - 32), packet.substring(sframeSize - 64,sframeSize - 32).length());
    skey[1]= bitArrayToInt32(data, data.length());
    Serial.print(F(" 0x"));
    Serial.println(skey[0],HEX);
    Serial.print(F(" 0x"));
    Serial.println(skey[1],HEX);
  }
  
  else if(keyLen == 128){
    data = rotateBit(packet.substring(sframeSize - 32,sframeSize), packet.substring(sframeSize - 32,sframeSize).length());
    skey[0]= bitArrayToInt32(data, data.length());
    data = rotateBit(packet.substring(sframeSize - 64,sframeSize - 32), packet.substring(sframeSize - 64,sframeSize - 32).length());
    skey[1]= bitArrayToInt32(data, data.length());
    data = rotateBit(packet.substring(sframeSize - 96,sframeSize - 64), packet.substring(sframeSize - 96,sframeSize - 64).length());
    skey[2]= bitArrayToInt32(data, data.length());
    data = rotateBit(packet.substring(sframeSize -128,sframeSize - 96), packet.substring(sframeSize -128,sframeSize - 96).length());
    skey[3]= bitArrayToInt32(data, data.length());
    Serial.print(F(" 0x"));
    Serial.println(skey[0],HEX);
    Serial.print(F(" 0x"));
    Serial.println(skey[1],HEX);
    Serial.print(F(" 0x"));
    Serial.println(skey[2],HEX);
    Serial.print(F(" 0x"));
    Serial.println(skey[3],HEX);
  }
}

String rotateBit(String packet, int size){
  String data;
  while (size--)
    data = data + packet[size];
  return data;
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
       if(arr[i] == '1')
          p1 |= 0x01 << (count/2 - i - 1);
       else
          p1 |= 0x00 << (count/2 - i - 1);
    }
    for (int i = count/2; i < count; i++) {
      if(arr[i] == '1')
        p2 |= 0x01 << (count - i - 1);
     else
        p2 |= 0x00 << (count - i - 1);
    }
    ret |= p1;
    ret = ret << 16;
    ret |= p2;
    return ret;
}



//High pass bessel filter order=1 alpha1=0.01  http://www.schwietering.com/jayduino/filtuino/index.php?characteristic=be&passmode=hp&order=1&alphalow=0.01&noteLow=&noteHigh=&pw=pw&calctype=float&run=Send
float FilterBeHp1(float x){ //class II {
v[0] = v[1];
      v[1] = (9.695312529087461995e-1 * x)
         + (0.93906250581749239892 * v[0]);
      return 
         (v[1] - v[0]);
}
