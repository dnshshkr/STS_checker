bool factoryReset() {
  Serial.print("[SETTINGS/Factory Reset]\nProceed? (Y): ");
  int8_t cd = 11;
  char choice;
  unsigned long prevmillis;
  do {
    if (millis() - prevmillis >= 1000)
    {
      cd--;
      cd >= 0 ? Serial.print(cd), Serial.print(" ") : 0;
      prevmillis = millis();
    }
  } while (!Serial.available() && cd >= 0);
  if (Serial.available())
    //    choice = toupper(Serial.readStringUntil('\r\n').charAt(0));
    choice = toupper(Serial.readStringUntil('\n').charAt(0));
  else
    choice = 'N';
  Serial.println();
  if (choice != 'Y') {
    Serial.println("Aborted");
    return false;
  }
  Serial.print("Resetting "), Serial.print(EEPROM.length()), Serial.println(" bytes...");
  for (uint32_t i = 0; i < EEPROM.length(); i++)
    EEPROM.update(i, 255);
  Serial.println("Done");
  return true;
}
