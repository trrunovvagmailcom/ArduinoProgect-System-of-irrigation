#include <Wire.h>
#include <iarduino_RTC.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>
#include <ArduinoJson.h>
//-------------------------------------DHT_Libraries----------------------------
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

//----------------------------------Parameters and Pins-------------------------
const PROGMEM uint8_t Switch_PIN_LCD = 9;
const PROGMEM uint8_t PIN_MH_Moisture_Sensor = A1, Switch_PIN_MH_Moisture_Sensor = 10;
const PROGMEM uint8_t PIN_MQ135_Gaz_Sensor   = A2, Switch_PIN_MQ135_Gaz_Sensor   = 11;
const PROGMEM uint8_t PIN_DHT_Sensor         = A3, Switch_PIN_DHT_Sensor         = 12;

const PROGMEM uint8_t Switch_PIN_DC_Motor = 5;
const PROGMEM uint8_t Alarm_Value_Sensor  = 60;

//------------------------------------Using libraries---------------------------
StaticJsonDocument<200> JSONdocument;
DHT DHTsensor(PIN_DHT_Sensor, DHT11);
iarduino_RTC DataTime(RTC_DS1302,4,2,3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//-------------------------------------PERIOD_LCD-------------------------------
uint16_t PERIOD_PRINT_LCD = 2000;
uint32_t timer_print_lcd = 0;

uint16_t PERIOD_PRINT_LCD_self = 2000;
uint32_t timer_print_lcd_self = 0;
//-----------------------------------PERIOD_SENSORS-----------------------------
uint16_t PERIOD_SELF_CHECK = 3600*1000;
uint32_t timer_self_check = 0;

uint16_t PERIOD_SEND_DATA = 60000;
uint32_t timer_send_data = 0;
//------------------------------------PERIOD_SENDING----------------------------
bool DHT_SENSOR_SWITCHED_ON = true;
bool MOISTURE_SENSOR_SWITCHED_ON = true;
bool GAZ_SENSOR_SWITCHED_ON = true;

bool RTC_updateOn = false;
bool UserPompSetOn = false;

bool RequestModeOn = false;
bool RequestModeProcess = false;


//-----------------------------------------POMP---------------------------------
uint8_t PompTimerDay = 30;// once every 30 days //DEFALT SETTINGS
uint8_t PompTimerHour = 60;
uint8_t PompTimerMinute = 60;

uint16_t PERIOD_POMP = 1000;//active time
uint32_t timer_popm_start = 0;
bool pompState = HIGH;//1 is off, 0 is on

//-------------------------------_________MAIN___________-----------------------

void setup() {
  Serial.begin(9600);
  DHTsensor.begin();
  DataTime.begin();
  //DataTime.settime(10,45,12,30,4,20,4);

//----------------------------POWER_ON_SENSORS,LCD,MOTOR------------------------
  Pin_SetUp(5,12);

  digitalWrite(Switch_PIN_LCD, LOW);
  digitalWrite(Switch_PIN_DHT_Sensor, LOW);
  digitalWrite(Switch_PIN_MH_Moisture_Sensor, LOW);
  digitalWrite(Switch_PIN_MQ135_Gaz_Sensor, LOW);
  delay(100);
  LCD_SetUp();
  Serial.println("Arduino has started!");
};

void loop() {
  if(millis() - timer_print_lcd >= PERIOD_PRINT_LCD && !RequestModeOn) {
    sensorsValuesPrintLCD();
    Serial.println(SensorDataJSON(GetValueSensor(1), GetValueSensor(2), GetValueSensor(3), GetValueSensor(4)));
    checkOverflow(timer_print_lcd, PERIOD_PRINT_LCD);
  };
  SerialPortListener();
  ModControl();
};

void ModControl(){
  if(RequestModeOn){
    if(millis() - timer_self_check >= PERIOD_SELF_CHECK) {
      RequestMode(true);
      checkOverflow(timer_self_check, PERIOD_SELF_CHECK);
    };//self-check
  };
  if(UserPompSetOn){
    TimerPompSwitch();
  };
};

uint8_t GetValueSensor(uint8_t __NumOfSensor){
  switch (__NumOfSensor) {
    case 1:return map(analogRead(PIN_MH_Moisture_Sensor), 1023, 0, 0, 100);
    case 2:return map(analogRead(PIN_MQ135_Gaz_Sensor),   0, 1023, 0, 100);
    case 3:return (uint8_t)DHTsensor.readHumidity();
    case 4:return (uint8_t)DHTsensor.readTemperature();
    default:return 0;
  };
};
void(* resetFunc) (void) = 0; // refresh reset
void checkOverflow(uint32_t &__timer, uint16_t __period) {
  do {__timer += __period;
    if (__timer < __period) break;
  } while (__timer < millis() - __period);
};
