#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

#define hod_svetl 3
#define sirena 4
#define zadna_p A0
#define kot_svetl A1


const int inputPins[] = {
  hod_svetl,
  sirena,
  zadna_p,
  kot_svetl
};

#define KM_ID "SZ1265"
#define code_on "0xABC"
#define code_off "0xABD"

void setup() {
  Serial.begin(115200);
  while(!Serial);

  for (int i = 0; i < sizeof(inputPins) / sizeof(inputPins[0]); i++) {
    pinMode(inputPins[i], OUTPUT);
  }

  while(!LoRa.begin(433E6)){
    Serial.print("LoRa could not start");
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();
}

void loop() {
    handle_recieve();
}

int handle_recieve(){
  int packet_size = LoRa.parsePacket();
  
  if (packet_size) {
    String my_data;
    while (LoRa.available()) {
      my_data = LoRa.readString();
    }
    
    if(my_data.length()<8) return 1;
    
    String km_id;
    String command;
    for(int i=0; i<=5; i++){
      km_id+=my_data[i];
    }
    
    if(km_id != KM_ID) return 1;
    
    int j=6;
    while(my_data[j]!='\0'){
      command+=my_data[j];
      j++;
    }

    Serial.println(command);

    if (command == "hod_svetl0xABC") {
      digitalWrite(hod_svetl, HIGH);
    } else if (command == "hod_svetl0xABD") {
      digitalWrite(hod_svetl, LOW);
    }
  
    else if (command == "sirena0xABC") {
      digitalWrite(sirena, HIGH);
    } else if (command == "sirena0xABD") {
      digitalWrite(sirena, LOW);
    }
  
    else if (command == "zadna_p0xABC") {
      digitalWrite(zadna_p, HIGH);
    } else if (command == "zadna_p0xABD") {
      digitalWrite(zadna_p, LOW);
    }
  
    else if (command == "kot_svetl0xABC") {
      digitalWrite(kot_svetl, HIGH);
    } else if (command == "kot_svetl0xABD") {
      digitalWrite(kot_svetl, LOW);
    }
    else {
      return 1;
    }

  }

  return 0;
}
