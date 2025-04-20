#include <LoRa.h>
#include <ArduinoJson.h>

#define hod_svetl 3
#define sirena 4
#define zadna_p A0
#define predna_p A1
#define rudan 6
#define rudan_allow 5
#define led_pin 7



#define KM_ID "SZ1265"
#define code_on "on1947"
#define code_off "off1947"

const int inputPins[] = {
  hod_svetl,
  sirena,
  zadna_p,
  predna_p,
  rudan,
  rudan_allow
};

bool lastStates[sizeof(inputPins) / sizeof(inputPins[0])] = {0};

void setup() {
  Serial.begin(112500);
  
  for (int i = 0; i < sizeof(inputPins) / sizeof(inputPins[0]); i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
  }

  pinMode(led_pin, OUTPUT);
    
  

  if(!LoRa.begin(433E6)){
      Serial.println("LoRa transmitter not working");
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();
  
}
void loop() {
  
  for (int i = 0; i < sizeof(inputPins) / sizeof(inputPins[0]); i++) {
    int pin = inputPins[i];
    bool currentState = digitalRead(pin);

    if (currentState != lastStates[i]) {
      lastStates[i] = currentState;

      switch (pin) {
        case hod_svetl:
          data_handler("hod_svetl", !currentState);
          break;
        case sirena:
          data_handler("sirena", !currentState);
          break;
        case zadna_p:
          data_handler("zadna_p", !currentState);
          break;
        case predna_p:
          data_handler("predna_p", !currentState);
          break;
        case rudan_allow:
          if(!digitalRead(rudan_allow) == 0){
            data_handler("rudan", 0);
            digitalWrite(led_pin, LOW);
          }else{
            digitalWrite(led_pin, HIGH);
          }
          break;
        case rudan:
          if(!digitalRead(rudan_allow) == 0){
            data_handler("rudan", 0);
          }else{
            data_handler("rudan", !currentState);
          }
          break;
        
        
      }
    }
  }
}
void data_handler(String my_component, bool my_state){
  String my_command = KM_ID+my_component;
  if(my_state == 1){
    my_command+=code_on;
  }else{
    my_command+=code_off;
  }
  LoRa.beginPacket();      
  LoRa.print(my_command);  
  LoRa.endPacket();   
}
