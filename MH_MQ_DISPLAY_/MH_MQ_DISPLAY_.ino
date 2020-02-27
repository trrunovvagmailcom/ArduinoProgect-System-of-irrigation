#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const PROGMEM int8_t PIN_MH_Moisture_GROUND = 13, PIN_MH_Moisture_VCC = 12, PIN_MH_Moisture_Sensor = A0; // inckude power for MQ135 Semiconductor sensor// inckude MH sensor for ground moisture
const PROGMEM int8_t PIN_MQ135_Gaz_GROUND = 11, PIN_MQ135_Gaz_VCC = 10, PIN_MQ135_Gaz_Sensor = A1; // inckude power for MQ135 Semiconductor sensor


class MainClass {
  public:
    void message();
    void printLCD (const __FlashStringHelper *value, int num_column, int num_line);
    void printLCD (int value, int num_column, int num_line);
    void powerSensor(int pin_GROUND, int pin_VCC);
    
  private:
};

MainClass groundSensor;
MainClass gazSensor;

void setup() {
  groundSensor.powerSensor(PIN_MH_Moisture_GROUND, PIN_MH_Moisture_VCC);
  gazSensor.powerSensor(PIN_MQ135_Gaz_GROUND, PIN_MQ135_Gaz_VCC);
  initLCD();
  Serial.begin (9600);
}

void loop() {
  int valueMHSensor = map(analogRead(PIN_MH_Moisture_Sensor), 1023, 0, 0, 100);
  groundSensor.printLCD(F("M:"), 0, 0);
  groundSensor.printLCD(valueMHSensor, 2, 0);

  int valueMQSensor = map(analogRead(PIN_MQ135_Gaz_Sensor), 0, 1023, 0, 100);
  gazSensor.printLCD(F("G:"), 0, 1);
  gazSensor.printLCD(valueMQSensor, 2, 1);

  delay(2000);
}

void MainClass::powerSensor(int pin_GROUND, int pin_VCC) {
  pinMode(pin_GROUND, OUTPUT);
  pinMode(pin_VCC, OUTPUT);

  digitalWrite(pin_GROUND, LOW);
  digitalWrite(pin_VCC, HIGH);
}
void MainClass::printLCD (const __FlashStringHelper *value, int num_column, int num_line) {
  lcd.setCursor(num_column, num_line);    // ставим курсор на 1 символ первой строки
  cleanLCD();
  lcd.setCursor(num_column, num_line);
  lcd.print(value);
}
void MainClass::printLCD (int value, int num_column, int num_line) {
  lcd.setCursor(num_column, num_line);    // ставим курсор на 1 символ первой строки
  cleanLCD();
  lcd.setCursor(num_column, num_line);    // ставим курсор на 1 символ первой строки
  lcd.print(value);
}
void initLCD() {
  lcd.init(); // инициализация LCD дисплея
  lcd.backlight(); // включение подсветки дисплея
}
void cleanLCD(){
     lcd.print(F("       "));
}
void MainClass::message() {
  lcd.setCursor(7, 0);
  lcd.print("12345678");
};
