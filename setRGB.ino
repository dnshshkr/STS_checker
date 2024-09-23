void setRGB(char ch) {
  static const String validCmd[] = {"S", "C", "LR", "HR", "LG", "HG", "LB", "HB"};
  static const uint8_t validCmdLen = sizeof(validCmd) / sizeof(validCmd[0]);
  uint8_t vals[chSize], valAddrs[chSize];
begin_setRGB:
  //  flushSerial();
  getVals();
  if (ch == '1') {
    for (uint8_t i = 0; i < chSize; i++) {
      valAddrs[i] = ylwAddrsCh1[i];
      vals[i] = ylwCh1[i];
    }
  }
  else {
    for (uint8_t i = 0; i < chSize; i++) {
      valAddrs[i] = ylwAddrsCh2[i];
      vals[i] = ylwCh2[i];
    }
  }
  Serial.print("[Settings/Channel "), Serial.print(ch), Serial.println("]");
  Serial.print("LR: "), Serial.println(vals[0]);
  Serial.print("HR: "), Serial.println(vals[1]);
  Serial.print("LG: "), Serial.println(vals[2]);
  Serial.print("HG: "), Serial.println(vals[3]);
  Serial.print("LB: "), Serial.println(vals[4]);
  Serial.print("HB: "), Serial.println(vals[5]);
  Serial.println("C: Self-calibrate");
  Serial.println("S: Back");
waitCmd_setRGB:
  while (!Serial.available());
  String color = Serial.readStringUntil('\n');
  color.trim();
  color.toUpperCase();
  bool isValid = false;
  //  for (short i = 0; i < sizeof(color) / sizeof(color[0]); i++)
  //    Serial.print(color[i], HEX);
  //  Serial.println();
  for (uint8_t i = 0; i < validCmdLen; i++) {
    //    if (color == "\0")
    //      goto waitCmd_setRGB;
    if (color == validCmd[i]) {
      isValid = true;
      break;
    }
  }
  if (!isValid) {
    Serial.println("Invalid");
    goto waitCmd_setRGB;
  }
  if (color == validCmd[0])
    return;
  else if (color == validCmd[1]) {
    autoCalibrate(valAddrs);
    goto begin_setRGB;
  }
  uint8_t minVal, maxVal;
  getMinMax(vals, color, minVal, maxVal);
  int val;
  bool outOfRange;
  do {
    Serial.print("Insert value for " + color + " ("), Serial.print(minVal), Serial.print("-"), Serial.print(maxVal), Serial.print("): ");
    while (!Serial.available());
    val = Serial.readStringUntil('\n').toInt();
    flushSerial();
    Serial.println(val);
    if (val < minVal || val > maxVal) {
      outOfRange = true;
      Serial.println("Out of range");
    }
    else
      outOfRange = false;
  } while (outOfRange);
  if (color == "LR")
    EEPROM.update(valAddrs[0], val);
  else if (color == "HR")
    EEPROM.update(valAddrs[1], val);
  else if (color == "LG")
    EEPROM.update(valAddrs[2], val);
  else if (color == "HG")
    EEPROM.update(valAddrs[3], val);
  else if (color == "LB")
    EEPROM.update(valAddrs[4], val);
  else
    EEPROM.update(valAddrs[5], val);
  goto begin_setRGB;
}
void getMinMax(uint8_t* vals, String color, uint8_t &minVal, uint8_t &maxVal) {
  const uint8_t absMin = 0, absMax = 255;
  if (color == "LR") {
    minVal = absMin;
    maxVal = vals[1];
  }
  else if (color == "HR") {
    minVal = vals[0];
    maxVal = absMax;
  }
  else if (color == "LG") {
    minVal = absMin;
    maxVal = vals[3];
  }
  else if (color == "HG") {
    minVal = vals[2];
    maxVal = absMax;
  }
  else if (color == "LB") {
    minVal = absMin;
    maxVal = vals[5];
  }
  else {
    minVal = vals[4];
    maxVal = absMax;
  }
}
