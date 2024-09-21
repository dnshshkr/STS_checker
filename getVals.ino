void getVals() {
  for (uint8_t i = 0; i < chSize; i++) {
    ylwCh1[i] = EEPROM.read(ylwAddrsCh1[i]);
    ylwCh2[i] = EEPROM.read(ylwAddrsCh2[i]);
  }
}
