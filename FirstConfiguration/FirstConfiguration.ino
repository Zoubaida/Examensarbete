/*
  First Configuration
  This sketch demonstrates the usage of MKR WAN 1300 LoRa module.
  This example code is in the public domain.
*/

#include <MKRWAN.h>
#include <SPI.h>
#include <LoRa.h>

LoRaModem modem;

// Uncomment if using the Murata chip as a module
// LoRaModem modem(Serial1);

String appEui = "70B3D57ED000A222";
String appKey = "3F4A3415A37402C5EF98377017FCDE74";
String devAddr = "26012A38";
String nwkSKey = "7B47C6B8EE7E6BF87695C567ADED6455";
String appSKey = "4B282393437E21B0D0F2506DEC0B7E57";
int echoPin = 6;
int trigPin = 7;
long duration, distance;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  while (!Serial);
  Serial.println("Welcome to MKRWAN1300 first configuration sketch");
  Serial.println("Register to your favourite LoRa network and we are ready to go!");
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
  int connected;
  connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }
  // Set poll interval to 60 secs.
  //  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
 LoRa.setSpreadingFactor(8);  
}

void loop() {
  int err;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  Serial.println(distance);
  byte payload[2];
  payload[0] = highByte(distance);
  payload[1] = lowByte(distance);
  modem.beginPacket();
  modem.write(payload, 2);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }

  delay(30000);
}
