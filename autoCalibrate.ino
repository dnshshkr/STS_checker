void autoCalibrate(uint8_t* valAddrs) {
  Serial.print("Make sure to turn on STS first before continuing\nEnter tolerance (0-255): ");
  uint8_t readCount = 128;
  uint16_t totalR = 0, totalG = 0, totalB = 0;
  int16_t vals[chSize];
  //  Serial.print("Enter tolerance (%): ");
  while (!Serial.available()) {}
  uint8_t tol = Serial.readStringUntil('\n').toInt();
  Serial.println(tol);
  //  tol /= 100.0;
  //  float tol = 0.15;
  Serial.println("Calibrating...");
  for (uint8_t i = 0; i < readCount; i++) {
    if (!checkConnection()) {
      Serial.println("Calibration failed");
      return;
    }
    tcs.getRGB(&fr, &fg, &fb);
    r = round(fr), g = round(fg), b = round(fb);
    totalR += r, totalG += g, totalB += b;
  }
  r = round((float)totalR / readCount);
  //  vals[0] = round((float)r * (1.0 - tol));
  vals[0] = r - tol;
  //  vals[1] = round((float)r * (1.0 + tol));
  vals[1] = r + tol;
  g = round((float)totalG / readCount);
  //  vals[2] = round((float)g * (1.0 - tol));
  vals[2] = g - tol;
  //  vals[3] = round((float)g * (1.0 + tol));
  vals[3] = g + tol;
  b = round((float)totalB / readCount);
  //  vals[4] = round((float)b * (1.0 - tol));
  vals[4] = b - tol;
  //  vals[5] = round((float)b * (1.0 + tol));
  vals[5] = b + tol;
  Serial.println("Average\nR: " + String(r) + "\tG: " + String(g) + "\tB: " + String(b));
  for (uint8_t i = 0; i < chSize; i += 2) {
    if (vals[i] < 0)
      vals[i] = 0;
  }
  for (uint8_t i = 1; i < chSize; i += 2) {
    if (vals[i] > 255)
      vals[i] = 255;
  }
  for (uint8_t i = 0; i < chSize; i++)
    EEPROM.update(valAddrs[i], vals[i]);
  Serial.println("Calibration done");
}
