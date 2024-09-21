void settings() {
  char cmd;
  flushSerial();
begin_settings:
  printInfo();
  Serial.println("[SETTINGS]");
  Serial.println("1: Yellow Ch1");
  Serial.println("2: Yellow Ch2");
  Serial.print("3: Relay Type: Active "), relayType ? Serial.println("High") : Serial.println("Low");
  Serial.print("4: Baud Rate: "), Serial.println(getBaudRate(EEPROM.read(baudRate_addr)));
  Serial.print("5: Integration Time: "), Serial.print(getIntegTime(EEPROM.read(integTime_addr))), Serial.println(" ms");
  Serial.print("6: Gain: "), Serial.print(getGain(EEPROM.read(gain_addr))), Serial.println("x");
  Serial.println("7: Reboot");
  Serial.println("8: Factory Reset");
  Serial.println("S: Exit");
waitCmd_settings:
  while (!Serial.available());
  //  cmd = toupper(Serial.readStringUntil('\r\n').charAt(0));
  cmd = toupper(Serial.readStringUntil('\n').charAt(0));
  switch (cmd) {
    case '1': case '2': {
        setRGB(cmd);
        goto begin_settings;
      }
    case '3': {
        relayType = !relayType;
        EEPROM.update(relayType_addr, relayType);
        goto begin_settings;
      }
    case '4': {
        if (baudRateSetting())
          reboot();
        else
          goto begin_settings;
      }
    case '5': {
        integrationTimeSetting();
        goto begin_settings;
      }
    case '6': {
        gainSetting();
        goto begin_settings;
      }
    case '7':
      reboot();
    case '8': {
        if (factoryReset())
          reboot();
        else
          goto begin_settings;
      }
    case 'S':
      return;
    default: {
        Serial.println("Invalid command");
        goto waitCmd_settings;
      }
  }
}
