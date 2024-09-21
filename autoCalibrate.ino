void autoCalibrate(char ch) {
  uint8_t readCount = 10;
  uint16_t totalR, totalG, totalB;
  int32_t lr, hr, lg, hg, lb, hb;
  float tolerance = 0.1;
  Serial.println("Auto calibrating...");
  for (uint8_t i = 0; i < readCount; i++) {
    tcs.getRGB(&fred, &fgreen, &fblue);
    red = round(fred), green = round(fgreen), blue = round(fblue);
    totalR += red;
    totalG += green;
    totalB += blue;
  }
  red = round((float)totalR / readCount);
  lr = round((float)red * (1.0 - tolerance));
  if (lr < 0)
    lr = 0;
  hr = round((float)red * (1.0 + tolerance));
  if (hr > 255)
    hr = 255;
  green = round((float)totalG / readCount);
  lg = round((float)green * (1.0 - tolerance));
  if (lg < 0)
    lg = 0;
  hg = round((float)green * (1.0 + tolerance));
  if (hg > 255)
    hg = 255;
  blue = round((float)totalB / readCount);
  lb = round((float)blue * (1.0 - tolerance));
  if (lb < 0)
    lb = 0;
  hb = round((float)blue * (1.0 + tolerance));
  if (hb > 255)
    hb = 255;
  switch (ch) {
    case '1': {
        EEPROM.update(ylw_LR_addr_ch1, lr);
        EEPROM.update(ylw_HR_addr_ch1, hr);
        EEPROM.update(ylw_LG_addr_ch1, lg);
        EEPROM.update(ylw_HG_addr_ch1, hg);
        EEPROM.update(ylw_LB_addr_ch1, lb);
        EEPROM.update(ylw_HB_addr_ch1, hb);
        break;
      }
    case '2': {
        EEPROM.update(ylw_LR_addr_ch2, lr);
        EEPROM.update(ylw_HR_addr_ch2, hr);
        EEPROM.update(ylw_LG_addr_ch2, lg);
        EEPROM.update(ylw_HG_addr_ch2, hg);
        EEPROM.update(ylw_LB_addr_ch2, lb);
        EEPROM.update(ylw_HB_addr_ch2, hb);
      }
  }
  Serial.println("Auto calibration completed");
}
