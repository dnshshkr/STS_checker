#define VERSION "3.4"
#include <EEPROM.h>
#include "Adafruit_TCS34725.h"
#define ylwPin 5
#define runPin 6
#define chPin 7
#define chSize 6
#define relayTypeAddr 0
#define baudRateAddr 1
#define integTimeAddr 2
#define gainAddr 3
Adafruit_TCS34725 tcs = Adafruit_TCS34725();
const uint8_t ylwAddrsCh1[chSize] = {4, 5, 6, 7, 8, 9}, ylwAddrsCh2[chSize] = {10, 11, 12, 13, 14, 15};
bool relayType;
uint8_t r, g, b, ylwCh1[chSize], ylwCh2[chSize];
float fr, fg, fb;
void (*resMcu)() = 0;
void setup() {
  pinMode(runPin, OUTPUT);
  pinMode(ylwPin, OUTPUT);
  pinMode(chPin, INPUT_PULLUP);
  getVals();
  relayType = EEPROM.read(relayTypeAddr);
  Serial.begin(getBaudRate(EEPROM.read(baudRateAddr)));
  printInfo();
  setIntegTime(EEPROM.read(integTimeAddr));
  setGain(EEPROM.read(gainAddr));
  //  float integTime = getIntegTime(EEPROM.read(integTimeAddr));
  //  uint8_t gain = getGain(EEPROM.read(gainAddr));
  //  Serial.print("Integration Time: "), Serial.print(integTime), Serial.print(" ms, Gain: "), Serial.print(gain), Serial.println("x");
  initSensor();
}
void loop() {
  //  checks for user interruption
  if (Serial.available()) {
    char cmd = toupper(Serial.readStringUntil('\n').charAt(0));
    if (cmd == 'S') {
      turnOffOutputs();
      settings(true);
    }
  }
  //  checks if connection to sensor exists
  if (!checkConnection()) {
    relayType ? digitalWrite(runPin, LOW) : digitalWrite(runPin, HIGH);
    initSensor();
  }
  relayType ? digitalWrite(runPin, HIGH) : digitalWrite(runPin, LOW);
  //  reads RGB data from sensor
  tcs.getRGB(&fr, &fg, &fb);
  r = round(fr), g = round(fg), b = round(fb);
  //  reads channel (LOW for channel 1, HIGH for channel 2)
  bool ch = digitalRead(chPin);
  Serial.print("Channel ");
  ch ? Serial.println(1) : Serial.println(2);
  Serial.print("R: "), Serial.print(r), Serial.print("\tG: "), Serial.print(g), Serial.print("\tB: "), Serial.print(b), Serial.println();
  //  matches read RGB data with stored RGB data
  if ((ch == HIGH && r >= ylwCh1[0] && r <= ylwCh1[1] && g >= ylwCh1[2] && g <= ylwCh1[3] && b >= ylwCh1[4] && b <= ylwCh1[5]) ||
      (ch == LOW && r >= ylwCh2[0] && r <= ylwCh2[1] && g >= ylwCh2[2] && g <= ylwCh2[3] && b >= ylwCh2[4] && b <= ylwCh2[5])) {
    Serial.println("Yellow");
    relayType ? digitalWrite(ylwPin, HIGH) : digitalWrite(ylwPin, LOW);
  }
  else {
    Serial.println("None");
    relayType ? digitalWrite(ylwPin, LOW) : digitalWrite(ylwPin, HIGH);
  }
  Serial.println();
}
bool checkConnection() {
  Wire.beginTransmission(TCS34725_ADDRESS);
  bool res = (Wire.endTransmission() == 0);
  if (res)
    return true;
  else {
    Serial.println("Sensor disconnected");
    return false;
  }
}
void getVals() {
  for (uint8_t i = 0; i < chSize; i++) {
    ylwCh1[i] = EEPROM.read(ylwAddrsCh1[i]);
    ylwCh2[i] = EEPROM.read(ylwAddrsCh2[i]);
  }
}
void printInfo() {
  Serial.print("STS Checker Ver. "), Serial.println(VERSION);
  Serial.println("Copyright(C) Delloyd R&D (M) Sdn Bhd");
}
void reboot() {
  Serial.println("Rebooting...");
  delay(100);
  resMcu();
}
void turnOffOutputs() {
  digitalWrite(LED_BUILTIN, LOW);
  if (relayType) {
    digitalWrite(runPin, LOW);
    digitalWrite(ylwPin, LOW);
  }
  else {
    digitalWrite(runPin, HIGH);
    digitalWrite(ylwPin, HIGH);
  }
}
