int pin_data  = 13;              // saída do PWM (eletrodo)
int pin_OnOff = 12;
int delayTime = 500;
const uint16_t interval = 8; // interval at which to stop PWM (milliseconds)
unsigned long previousMillis = 0;
String control = "00";

void setup() {
  Serial.begin(9600);
  pinMode      (pin_data, OUTPUT); // pin 13
  pinMode      (pin_OnOff,OUTPUT);
  digitalWrite (pin_data, LOW );
  digitalWrite (pin_OnOff,HIGH);
}

void loop() {
  String data, packet;
  boolean shouldSend;
  uint32_t skey[4] = {0x9474B8E8, 0xC73BCA7D, 0x53239142, 0xF3C3121A};
  for(int i = 0; i < 4; i++){         
    data = String(skey[i], BIN); // pacote de dados
    if(32 - data.length())
      data = complete_zeros(32 - data.length()) + data;
    
    if (data != "") { // montagem do pacote   
      packet = complete_zeros(3 - String(i, BIN).length()) + String(i, BIN); //ID do pacote   
      packet += data;
      shouldSend = true;
    }
    
    if (shouldSend) {
      shouldSend = false;
      //unsigned long currentMillis = millis();
      modulation(packet);                // envio do pacote 35bits em 1050ms
      digitalWrite (pin_OnOff,HIGH);
      //Serial.print("Time: ");
      //Serial.println(millis() - currentMillis);
      delay(delayTime);
    }
    // Reset variables
    packet = data = ""; 
  }
  delay(delayTime*3);
}

void OOK_2mod(int state) {
  unsigned long currentMillis = millis();
  while (currentMillis + interval > millis())  //"espera ocupada"
    digitalWrite(pin_data,state);
}

void modulation(String packet){
  digitalWrite (pin_OnOff,LOW);
  //OOK_2mod(0);//startbit
  for (int i = 0; i < packet.length(); i++) {
    //Serial.print(packet.charAt(i));
    if (packet.charAt(i) == '0'){
      OOK_2mod(0);
    }
    else if (packet.charAt(i) == '1')
      OOK_2mod(1);
  }
  digitalWrite (pin_OnOff,HIGH);
  //digitalWrite(pin_data,LOW);
  //Serial.println();
}

String complete_zeros(int len){
  String zeros;
  for (int i = 0; i < len; i++)
    zeros = zeros + "0";
  return zeros;
}
