void autoCalibrate(uint8_t* valAddrs) {
  uint8_t readCount = 100;
  uint16_t totalR, totalG, totalB;
  int16_t vals[chSize];
  float tol = 0.1;
  Serial.println("Calibrating...");
  for (uint8_t i = 0; i < readCount; i++) {
    tcs.getRGB(&fr, &fg, &fb);
    r = round(fr), g = round(fg), b = round(fb);
    totalR += r, totalG += g, totalB += b;
  }
  r = round((float)totalR / readCount);
  vals[0] = round((float)r * (1.0 - tol));
  vals[1] = round((float)r * (1.0 + tol));
  g = round((float)totalG / readCount);
  vals[2] = round((float)g * (1.0 - tol));
  vals[3] = round((float)g * (1.0 + tol));
  b = round((float)totalB / readCount);
  vals[4] = round((float)b * (1.0 - tol));
  vals[5] = round((float)b * (1.0 + tol));
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
  Serial.println("Done");
}
