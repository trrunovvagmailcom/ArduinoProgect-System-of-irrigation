void sensorsValuesPrintLCD(){
  printLCDnoClear(F("S"), 0, 0);
  printLCD(GetValueSensor(1), 1, 0);
  printLCDnoClear(F("G"), 0, 1);
  printLCD(GetValueSensor(2), 1, 1);
  printLCDnoClear(F("H"), 4, 0);
  printLCD(GetValueSensor(3), 5, 0);
  printLCDnoClear(F("T"), 4, 1);
  printLCD(GetValueSensor(4), 5, 1);

  uint32_t time = millis()/1000;
  String ModuleTime = formatTime(time/60/60) + ":" + formatTime(time/60%60) + ":" + formatTime(time%60);
  lcd.setCursor(8, 0); lcd.print(ModuleTime);
  DataTime.gettime("d-m-Y,H:i:s"); 
  ModuleTime = formatTime(DataTime.Hours) + ":" + formatTime(DataTime.minutes) + ":" + formatTime(DataTime.seconds);
  lcd.setCursor(8, 1); lcd.print(ModuleTime);
};

String formatTime(int __printParameter){
  if(__printParameter < 10) return "0" + String(__printParameter);
  return String(__printParameter);
};

void printLCD (const __FlashStringHelper *value, uint8_t num_column, uint8_t num_line) {
  lcd.setCursor(num_column, num_line);cleanMinLCD(num_column, num_line);
  lcd.setCursor(num_column, num_line);lcd.print(value);
};

void printLCD (int value, uint8_t num_column, uint8_t num_line) {
  lcd.setCursor(num_column, num_line);cleanMinLCD(num_column, num_line);
  lcd.setCursor(num_column, num_line);lcd.print(value);
};

void printLCDnoClear (int value, uint8_t num_column, uint8_t num_line) {
  lcd.setCursor(num_column, num_line);lcd.print(value);
};
void printLCDnoClear (const __FlashStringHelper *value, uint8_t num_column, uint8_t num_line) {
  lcd.setCursor(num_column, num_line);lcd.print(value);
};

void cleanMinLCD(uint8_t num_column,uint8_t num_line){
  lcd.setCursor(num_column, num_line);
  lcd.print(F("   "));
};

void LCD_Switch(bool __Switch_Parameter){
  if(__Switch_Parameter){
    lcd.display();
    lcd.init();
  }else{
    lcd.noDisplay();
  };
  lcd.setBacklight(__Switch_Parameter);
};

void LCD_SetUp(){
  lcd.init();
  lcd.backlight();
};
