void settings() {
  //  char cmd;
begin_settings:
  //  flushSerial();
  printInfo();
  Serial.println("[Settings]");
  Serial.println("1: Channel 1");
  Serial.println("2: Channel 2");
  Serial.print("3: Relay Type: Active "), relayType ? Serial.println("high") : Serial.println("low");
  Serial.print("4: Baud Rate: "), Serial.println(getBaudRate(EEPROM.read(baudRateAddr)));
  Serial.print("5: Integration Time: "), Serial.print(getIntegTime(EEPROM.read(integTimeAddr))), Serial.println(" ms");
  Serial.print("6: Gain: "), Serial.print(getGain(EEPROM.read(gainAddr))), Serial.println("x");
  Serial.println("7: Reboot");
  Serial.println("8: Factory Reset");
  Serial.println("S: Exit");
waitCmd_settings:
  Serial.print("Selection: ");
  while (!Serial.available());
  char cmd = Serial.readStringUntil('\n').charAt(0);
  Serial.println(cmd);
  cmd = toupper(cmd);
  //  char cmd = Serial.read();
  //  flushSerial();
  switch (cmd) {
    case '1': case '2': {
        setRGB(cmd);
        //        goto begin_settings;
        break;
      }
    case '3': {
        relayType = !relayType;
        EEPROM.update(relayTypeAddr, relayType);
        //        goto begin_settings;
        break;
      }
    case '4': {
        if (baudRateSetting())
          reboot();
        else
          //          goto begin_settings;
          break;
      }
    case '5': {
        integrationTimeSetting();
        //        goto begin_settings;
        break;
      }
    case '6': {
        gainSetting();
        //        goto begin_settings;
        break;
      }
    case '7':
      reboot();
    case '8': {
        if (factoryReset())
          reboot();
        else
          //          goto begin_settings;
          break;
      }
    case 'S': case's':
      return;
    default: {
        Serial.println("Invalid");
        goto waitCmd_settings;
      }
  }
  goto begin_settings;
}
