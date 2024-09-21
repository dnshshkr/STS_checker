void getVals() {
  ylw_LR_ch1 = EEPROM.read(ylw_LR_addr_ch1);
  ylw_HR_ch1 = EEPROM.read(ylw_HR_addr_ch1);
  ylw_LG_ch1 = EEPROM.read(ylw_LG_addr_ch1);
  ylw_HG_ch1 = EEPROM.read(ylw_HG_addr_ch1);
  ylw_LB_ch1 = EEPROM.read(ylw_LB_addr_ch1);
  ylw_HB_ch1 = EEPROM.read(ylw_HB_addr_ch1);

  ylw_LR_ch2 = EEPROM.read(ylw_LR_addr_ch2);
  ylw_HR_ch2 = EEPROM.read(ylw_HR_addr_ch2);
  ylw_LG_ch2 = EEPROM.read(ylw_LG_addr_ch2);
  ylw_HG_ch2 = EEPROM.read(ylw_HG_addr_ch2);
  ylw_LB_ch2 = EEPROM.read(ylw_LB_addr_ch2);
  ylw_HB_ch2 = EEPROM.read(ylw_HB_addr_ch2);
}
