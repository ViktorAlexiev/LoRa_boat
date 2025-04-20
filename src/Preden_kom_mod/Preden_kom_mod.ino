#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

#define predna_P 3
#define rudan_R A0
#define rudan_B 4

const int inputPins[] = {
  predna_P,
  rudan_R,
  rudan_B,
};

#define KM_ID "SZ1265"
#define code_on "on1947"
#define code_off "off1947"

void setup() {
  Serial.begin(115200);
  while(!Serial);

  for (int i = 0; i < sizeof(inputPins) / sizeof(inputPins[0]); i++) {
    pinMode(inputPins[i], OUTPUT);
  }

  while(!LoRa.begin(433E6)) {
    Serial.print("LoRa could not start");
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();
}

void loop() {
    handle_receive();
}

int handle_receive() {
  int packet_size = LoRa.parsePacket();

  if (packet_size) {
    String my_data;
    while (LoRa.available()) {
      my_data = LoRa.readString();
    }

    if (my_data.length() < 8) return 1;

    String km_id;
    String command;
    for (int i = 0; i <= 5; i++) {
      km_id += my_data[i];
    }

    if (km_id != KM_ID) return 1;

    int j = 6;
    while (my_data[j] != '\0') {
      command += my_data[j];
      j++;
    }

    Serial.println(command);
    
    if (command == "rudanon1947") {    
        digitalWrite(rudan_R, HIGH);
        digitalWrite(rudan_B, HIGH);
    } else if (command == "rudanoff1947") {
        digitalWrite(rudan_R, LOW);
        digitalWrite(rudan_B, LOW);
    }

    else if (command == "predna_pon1947") {
      digitalWrite(predna_P, HIGH);
    } else if (command == "predna_poff1947") {
      digitalWrite(predna_P, LOW);
    }
    else {
      return 1;
    }
  }

  return 0;
}
