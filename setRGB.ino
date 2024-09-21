void setRGB(char cmd) {
  const String validCmd[] = {"S", "1", "LR", "HR", "LG", "HG", "LB", "HB"};
begin_setRGB:
  getVals();
  Serial.print("[SETTINGS/Yellow Ch"), Serial.print(cmd), Serial.println("]");
  switch (cmd) {
    case '1': {
        Serial.print("LR: "), Serial.println(ylw_LR_ch1);
        Serial.print("HR: "), Serial.println(ylw_HR_ch1);
        Serial.print("LG: "), Serial.println(ylw_LG_ch1);
        Serial.print("HG: "), Serial.println(ylw_HG_ch1);
        Serial.print("LB: "), Serial.println(ylw_LB_ch1);
        Serial.print("HB: "), Serial.println(ylw_HB_ch1);
        printAutoCalibrate();
        break;
      }
    case '2': {
        Serial.print("LR: "), Serial.println(ylw_LR_ch2);
        Serial.print("HR: "), Serial.println(ylw_HR_ch2);
        Serial.print("LG: "), Serial.println(ylw_LG_ch2);
        Serial.print("HG: "), Serial.println(ylw_HG_ch2);
        Serial.print("LB: "), Serial.println(ylw_LB_ch2);
        Serial.print("HB: "), Serial.println(ylw_HB_ch2);
        printAutoCalibrate();
      }
  }
  printBack();
waitCmd_setRGB:
  while (!Serial.available());
  String color = Serial.readStringUntil('\n');
  color.trim();
  color.toUpperCase();
  bool isValid = false;
  for (uint8_t i = 0; i < sizeof(validCmd) / sizeof(validCmd[0]); i++) {
    if (color == "\0")
      goto waitCmd_setRGB;
    if (color == validCmd[i]) {
      isValid = true;
      break;
    }
  }
  if (!isValid) {
    Serial.println("Invalid command");
    goto waitCmd_setRGB;
  }
  if (color == validCmd[0])
    return;
  else if (color == validCmd[1]) {
    autoCalibrate(cmd);
    goto begin_setRGB;
  }
  uint8_t minVal, maxVal;
  getMinMax(cmd, color, minVal, maxVal);
  int val;
  bool outOfRange;
  do {
    Serial.print("Insert new value for " + color + " ("), Serial.print(minVal), Serial.print("-"), Serial.print(maxVal), Serial.print("): ");
    while (!Serial.available());
    val = Serial.readStringUntil('\n').toInt();
    Serial.println(val);
    if (val < minVal || val > maxVal) {
      outOfRange = true;
      Serial.println("Value out of range");
    }
    else
      outOfRange = false;
  } while (outOfRange);
  switch (cmd) {
    case '1': {
        if (color == "LR")
          EEPROM.update(ylw_LR_addr_ch1, val);
        else if (color == "HR")
          EEPROM.update(ylw_HR_addr_ch1, val);
        else if (color == "LG")
          EEPROM.update(ylw_LG_addr_ch1, val);
        else if (color == "HG")
          EEPROM.update(ylw_HG_addr_ch1, val);
        else if (color == "LB")
          EEPROM.update(ylw_LB_addr_ch1, val);
        else if (color == "HB")
          EEPROM.update(ylw_HB_addr_ch1, val);
        break;
      }
    case '2': {
        if (color == "LR")
          EEPROM.update(ylw_LR_addr_ch2, val);
        else if (color == "HR")
          EEPROM.update(ylw_HR_addr_ch2, val);
        else if (color == "LG")
          EEPROM.update(ylw_LG_addr_ch2, val);
        else if (color == "HG")
          EEPROM.update(ylw_HG_addr_ch2, val);
        else if (color == "LB")
          EEPROM.update(ylw_LB_addr_ch2, val);
        else if (color == "HB")
          EEPROM.update(ylw_HB_addr_ch2, val);
      }
  }
  goto begin_setRGB;
}
void getMinMax(char channel, String color, uint8_t &minVal, uint8_t &maxVal) {
  const uint8_t absMin = 0, absMax = 255;
  if (channel == '1') {
    if (color == "LR") {
      minVal = absMin;
      maxVal = ylw_HR_ch1;
    }
    else if (color == "HR") {
      minVal = ylw_LR_ch1;
      maxVal = absMax;
    }
    else if (color == "LG") {
      minVal = absMin;
      maxVal = ylw_HG_ch1;
    }
    else if (color == "HG") {
      minVal = ylw_LG_ch1;
      maxVal = absMax;
    }
    else if (color == "LB") {
      minVal = absMin;
      maxVal = ylw_HB_ch1;
    }
    else {
      minVal = ylw_LB_ch1;
      maxVal = absMax;
    }
  }
  else {
    if (color == "LR") {
      minVal = absMin;
      maxVal = ylw_HR_ch2;
    }
    else if (color == "HR") {
      minVal = ylw_LR_ch2;
      maxVal = absMax;
    }
    else if (color == "LG") {
      minVal = absMin;
      maxVal = ylw_HG_ch2;
    }
    else if (color == "HG") {
      minVal = ylw_LG_ch2;
      maxVal = absMax;
    }
    else if (color == "LB") {
      minVal = absMin;
      maxVal = ylw_HB_ch2;
    }
    else {
      minVal = ylw_LB_ch2;
      maxVal = absMax;
    }
  }
}
