static const uint32_t baudRates[] = {9600, 19200, 38400, 57600, 74880, 115200};
bool baudRateSetting() {
  Serial.println("[SETTINGS/Baud Rate]");
  for (uint8_t i = 0; i < sizeof(baudRates) / sizeof(baudRates[0]); i++)
    Serial.print(i + 1), Serial.print(": "), Serial.println(baudRates[i]);
  Serial.println("S: Back");
waitCmd_baudRateSetting:
  Serial.print("Selection: ");
  while (!Serial.available());
  char choice = toupper(Serial.readStringUntil('\n').charAt(0));
  Serial.println(choice);
  if (choice == 'S')
    return false;
  if (choice - '0' < 1 || choice - '0' > 6) {
    Serial.println("Invalid input");
    goto waitCmd_baudRateSetting;
  }
  uint32_t baudRate = getBaudRate(choice - '0' - 1);
  Serial.println("Baud rate set to " + String(baudRate));
  return true;
}
uint32_t getBaudRate(uint8_t index) {
  uint32_t baudRate;
  if (index >= 0 && index < sizeof(baudRates) / sizeof(baudRates[0]))
    baudRate = baudRates[index];
  else {
    baudRate = 9600;
    index = 0;
  }
  EEPROM.update(baudRateAddr, index);
  return baudRate;
}
