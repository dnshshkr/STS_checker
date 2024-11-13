const uint32_t baudRates[] = {9600, 19200, 38400, 57600, 74880, 115200};
const uint8_t baudRatesLen = sizeof(baudRates) / sizeof(baudRates[0]);
bool baudRateSetting() {
  Serial.println(F("[Settings/Baud Rate]"));
  for (uint8_t i = 0; i < baudRatesLen; i++)
    Serial.print(i + 1), Serial.print(F(": ")), Serial.println(baudRates[i]);
  Serial.println(F("S: Back"));
waitCmd_baudRateSetting:
  Serial.print(F("Selection: "));
  while (!Serial.available());
  char choice = toupper(Serial.readStringUntil('\n').charAt(0));
  Serial.println(choice);
  if (choice == 'S')
    return false;
  if (choice - '0' < 1 || choice - '0' > baudRatesLen) {
    Serial.println(F("Invalid"));
    goto waitCmd_baudRateSetting;
  }
  uint32_t baudRate = getBaudRate(choice - '0' - 1);
  Serial.print(F("Baud rate set to ")), Serial.println(baudRate);
  return true;
}
uint32_t getBaudRate(uint8_t index) {
  uint32_t baudRate;
  if (index >= 0 && index < baudRatesLen)
    baudRate = baudRates[index];
  else {
    baudRate = 9600;
    index = 0;
  }
  EEPROM.update(baudRateAddr, index);
  return baudRate;
}
