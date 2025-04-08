#include <LoRa.h>
#include <ArduinoJson.h>

#define hod_svetl 3
#define sirena 4
#define zadna_P A0
#define predna_p A1
#define osvob_rudan 5
#define rudan 6
#define kot_svetl 7

const int inputPins[] = {
  hod_svetl,
  sirena,
  zadna_P,
  predna_p,
  osvob_rudan,
  rudan,
  kot_svetl
};

bool lastStates[sizeof(inputPins) / sizeof(inputPins[0])] = {0};


void setup() {
  Serial.begin(112500);
  
  for (int i = 0; i < sizeof(inputPins) / sizeof(inputPins[0]); i++) {
    pinMode(inputPins[i], INPUT);
  }

  if(!LoRa.begin(433000000)){
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
          data_handler("hod_svetl", currentState);
          break;
        case sirena:
          data_handler("sirena", currentState);
          break;
        case zadna_P:
          data_handler("zadna_P", currentState);
          break;
        case predna_p:
          data_handler("predna_p", currentState);
          break;
        case osvob_rudan:
          data_handler("osvob_rudan", currentState);
          break;
        case rudan:
          data_handler("rudan", currentState);
          break;
        case kot_svetl:
          data_handler("kot_svetl", currentState);
          break;
      }
    }
  }
}
void data_handler(String my_component, bool my_command){
  String my_json = "{";
  my_json += "\"my_ID\":" + "KM_0x2C7" + ",";
  my_json += "\" my_component\":" + my_name + ",";
  my_json += "\"command\":" + String(my_command);
  my_json += "}";

  LoRa.beginPacket();      
  LoRa.print(my_json);  
  LoRa.endPacket();   
}
