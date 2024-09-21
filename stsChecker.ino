#define VERSION "3.0"
#include <EEPROM.h>
#include "Adafruit_TCS34725.h"
#define runPin 6
#define ylwPin 5
#define chPin 7
#define relayType_addr 0
#define baudRate_addr 1
#define integTime_addr 2
#define gain_addr 3
#define ylw_LR_addr_ch1 4
#define ylw_HR_addr_ch1 5
#define ylw_LG_addr_ch1 6
#define ylw_HG_addr_ch1 7
#define ylw_LB_addr_ch1 8
#define ylw_HB_addr_ch1 9
#define ylw_LR_addr_ch2 10
#define ylw_HR_addr_ch2 11
#define ylw_LG_addr_ch2 12
#define ylw_HG_addr_ch2 13
#define ylw_LB_addr_ch2 14
#define ylw_HB_addr_ch2 15
Adafruit_TCS34725 tcs = Adafruit_TCS34725();
bool relayType;
uint8_t red, green, blue, ylw_LR_ch1, ylw_HR_ch1, ylw_LG_ch1, ylw_HG_ch1, ylw_LB_ch1, ylw_HB_ch1;
uint8_t ylw_LR_ch2, ylw_HR_ch2, ylw_LG_ch2, ylw_HG_ch2, ylw_LB_ch2, ylw_HB_ch2;
float fred, fgreen, fblue;
void (*resMcu)() = 0;
void reboot() {
  Serial.println("Rebooting...");
  delay(100);
  resMcu();
}
void setup() {
  pinMode(runPin, OUTPUT);
  pinMode(ylwPin, OUTPUT);
  pinMode(chPin, INPUT_PULLUP);
  getVals();
  relayType = EEPROM.read(relayType_addr);
  Serial.begin(getBaudRate(EEPROM.read(baudRate_addr)));
  printInfo();
  setIntegTime(EEPROM.read(integTime_addr));
  setGain(EEPROM.read(gain_addr));
  float integTime = getIntegTime(EEPROM.read(integTime_addr));
  uint8_t gain = getGain(EEPROM.read(gain_addr));
  Serial.print("Integration Time: "), Serial.print(integTime), Serial.print(" ms, Gain: "), Serial.print(gain), Serial.println("x");
  initSensor();
}
void loop() {
  char cmd;
  if (Serial.available()) {
    if (toupper((char)Serial.read()) == 'S') {
      turnOffOutputs();
      settings();
    }
  }
  if (!checkConnection()) {
    Serial.println("Sensor disconnected. Reconnecting...");
    relayType ? digitalWrite(runPin, LOW) : digitalWrite(runPin, HIGH);
    initSensor();
  }
  relayType ? digitalWrite(runPin, HIGH) : digitalWrite(runPin, LOW);
  tcs.getRGB(&fred, &fgreen, &fblue);
  red = round(fred), green = round(fgreen), blue = round(fblue);
  bool channel = digitalRead(chPin);
  Serial.print("Channel ");
  channel ? Serial.println(1) : Serial.println(2);
  Serial.print("R: "), Serial.print(red), Serial.print("\tG: "), Serial.print(green), Serial.print("\tB: "), Serial.print(blue), Serial.println(), Serial.print("Result: ");
  if ((channel == HIGH && red >= ylw_LR_ch1 && red <= ylw_HR_ch1 && green >= ylw_LG_ch1 && green <= ylw_HG_ch1 && blue >= ylw_LB_ch1 && blue <= ylw_HB_ch1) ||
      (channel == LOW && red >= ylw_LR_ch2 && red <= ylw_HR_ch2 && green >= ylw_LG_ch2 && green <= ylw_HG_ch2 && blue >= ylw_LB_ch2 && blue <= ylw_HB_ch2)) {
    Serial.println("Yellow\n");
    relayType ? digitalWrite(ylwPin, HIGH) : digitalWrite(ylwPin, LOW);
  }
  else {
    Serial.println("No reading\n");
    relayType ? digitalWrite(ylwPin, LOW) : digitalWrite(ylwPin, HIGH);
  }
}
bool checkConnection() {
  Wire.beginTransmission(TCS34725_ADDRESS);
  return (Wire.endTransmission() == 0);
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
void printInfo() {
  Serial.print("STS Checker Ver. "), Serial.println(VERSION);
  Serial.println("Copyright(C) Delloyd R&D (M) Sdn Bhd");
}
void flushSerial() {
  while (Serial.available())
    Serial.read();
}
void printBack() {
  Serial.println("S: Back");
}
void printAutoCalibrate() {
  Serial.println("1. Auto calibrate (this operation overwrites all RGB values)");
}
