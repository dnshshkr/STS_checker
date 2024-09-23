void initSensor() {
  bool bl = true;
  unsigned long prevmillis;
  Serial.println("Setting up sensor...");
  bool sensorInit = tcs.begin();
  while (!sensorInit) {
    relayType ? digitalWrite(ylwPin, LOW) : digitalWrite(ylwPin, HIGH);
    if (Serial.available()) {
      char cmd = Serial.read();
      flushSerial();
      if (toupper(cmd) == 'S') {
        turnOffOutputs();
        settings();
      }
    }
    //    blinks LED_BUILTIN and runPin
    if (prevmillis == 0 || millis() - prevmillis >= 500) {
      bl = !bl;
      Serial.println("Sensor error (Send 'S' to enter settings)");
      relayType ? digitalWrite(runPin, bl) : digitalWrite(runPin, !bl);
      digitalWrite(LED_BUILTIN, bl);
      sensorInit = tcs.begin();
      prevmillis = millis();
    }
  }
  Serial.println("Sensor ready");
  relayType ? digitalWrite(runPin, HIGH) : digitalWrite(runPin, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
}
