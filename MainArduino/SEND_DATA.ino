String SensorDataJSON(int8_t __valueSoil, int8_t __valueGaz, int8_t __valueHumidity, int8_t __valueTemperature){
  String DataSensors = String("{\"S\":)" + __valueSoil + ",\""
                                 "G\":"  + __valueGaz  + ",\""
                                 "H\":"  + __valueHumidity    +" ,\""
                                 "T\":"  + __valueTemperature + "}\0";
  return DataSensors;
};
uint8_t SerialPortListener() {
  if (Serial.available() > 0) {
    String recievedFromESP = Serial.readString();
    if (recievedFromESP[0] == '>' && recievedFromESP[2] == '{') {
      Serial.println(recievedFromESP.substring(2));
      deserializeJson(JSONdocument, recievedFromESP.substring(2));
      int dd = JSONdocument["d"];
      Serial.print(dd);
      switch (recievedFromESP[1]) {
        case '0'://set data time
          Serial.println(F("Command 1: set data time!"));
          if(RTC_updateOn){
            Serial.println(F("The RTC was set earlier!"));
          }else{
            UpdateRTC(JSONdocument["s"],JSONdocument["m"],JSONdocument["h"],JSONdocument["d"],JSONdocument["mn"],JSONdocument["y"],JSONdocument["wd"]);
          };
          break;
        case '1'://set data time
          Serial.println(F("Command 1: set data time!"));
          if(RTC_updateOn){
            Serial.println(F("The RTC was set earlier!"));
          }else{
            UpdateRTC(JSONdocument["s"],JSONdocument["m"],JSONdocument["h"],JSONdocument["d"],JSONdocument["mn"],JSONdocument["y"],JSONdocument["wd"]);
          };
          break;
        case '2'://pomp timer
          Serial.println(F("Command 2: Pomp user settings!"));
          SetPompTimerParameters(JSONdocument["d"],JSONdocument["h"],JSONdocument["m"],JSONdocument["dur"]);
          break;
        case '3'://user setting mode, timers for modules, action on request//send data
          Serial.println(F("Command 3: User modules and sensors settings!"));
          if(millis() > timer_send_data + PERIOD_SEND_DATA){
            RequestMode(false);
          }else{
            Serial.print(F("Offen requests or self-check mode, please try again in "));
            uint32_t waitTime = (timer_send_data + PERIOD_SEND_DATA) - millis();
            Serial.println(waitTime/1000);
          };
          break;
        case '4':{//get current RTC,module time
          Serial.println(F("Command 4: get current RTC,module time!"));
          Serial.println(DataTime.gettime("d-m-Y,H:i:s"));
          uint32_t time = millis()/1000;
          String ModuleTime = formatTime(time/60/60) + ":" + formatTime(time/60%60) + ":" + formatTime(time%60);
          Serial.println(ModuleTime);
          break;
        };
        case '5':
          Serial.println(F("Command 5: restart!"));
          resetFunc();
          break;
        default:
          Serial.println(F("The command is not found!"));
      };
      recievedFromESP = "<" + recievedFromESP;
      Serial.println(recievedFromESP);
    };
  };
};

void UpdateRTC(uint8_t second,uint8_t minute,uint8_t hour,uint8_t day,uint8_t month, uint8_t year,uint8_t weekday){
  if(year != DataTime.year || month != DataTime.month || day != DataTime.day || hour != DataTime.Hours){
    Serial.print(F("Old time is "));
    Serial.println(DataTime.gettime("d-m-Y,H:i:s"));
    DataTime.settime(second,minute,hour,day,month,year,weekday);//set RTC time
    Serial.print(F("RTC time is udated! New time is "));
    Serial.println(DataTime.gettime("d-m-Y,H:i:s"));
  }else{
    Serial.print(F("The current RTC time of Arduino is actual! "));
    Serial.println(DataTime.gettime("d-m-Y,H:i:s"));
  };
  RTC_updateOn = true;
};

void SetPompTimerParameters(uint8_t perMonth, uint8_t perHours, uint8_t perMinutes, uint16_t duration){
  PompTimerDay = perMonth;
  PompTimerHour = perHours;
  PompTimerMinute = perMinutes;
  PERIOD_POMP = duration;
  Serial.println(PompTimerDay);
  Serial.println(PERIOD_POMP);
  UserPompSetOn = true;
  Serial.println(F("Pomp settings were set up!"));
};

uint8_t CheckSameMinute = 0;

void TimerPompSwitch(){
  if (pompState==HIGH && CheckSameMinute != DataTime.minutes && DataTime.day % PompTimerDay == 0 && DataTime.Hours % PompTimerHour == 0 && DataTime.minutes % PompTimerMinute == 0) {
    pompState = LOW;
    Serial.println(F("pomp ON"));
    timer_popm_start = millis();
    CheckSameMinute = DataTime.minutes;
    digitalWrite(Switch_PIN_DC_Motor,pompState);
  }else if(!pompState && millis() - timer_popm_start >= PERIOD_POMP){
    pompState = HIGH;
    Serial.println(F("pomp OFF"));
    digitalWrite(Switch_PIN_DC_Motor,pompState);
  };
};

void RequestMode(bool OnlyCheckSensors){
  if(!RequestModeProcess){
    RequestModeProcess = true;
    if(!RequestModeOn) RequestModeOn = true;
    if(!MOISTURE_SENSOR_SWITCHED_ON || !GAZ_SENSOR_SWITCHED_ON || !DHT_SENSOR_SWITCHED_ON){
      SwitchOnOffModule(true,Switch_PIN_MH_Moisture_Sensor,MOISTURE_SENSOR_SWITCHED_ON);
      SwitchOnOffModule(true,Switch_PIN_MQ135_Gaz_Sensor,GAZ_SENSOR_SWITCHED_ON);
      SwitchOnOffModule(true,Switch_PIN_DHT_Sensor,DHT_SENSOR_SWITCHED_ON);
    };
    uint32_t preparationSensorsTimer = millis();
    Serial.println(F("Preparation of the sensors!"));
    LCD_Switch(true);
    while(millis() < preparationSensorsTimer + 15000){
      if(millis() > preparationSensorsTimer + 10000 && GetValueSensor(2) > 20){
        if(OnlyCheckSensors){
          CheckSensorsValues();
        }else{
          Serial.println(SensorDataJSON(GetValueSensor(1), GetValueSensor(2), GetValueSensor(3), GetValueSensor(4)));
          Serial.println(F("Data is done. Turning off the modules and sensors!"));
          timer_send_data = millis();
        };
        if(MOISTURE_SENSOR_SWITCHED_ON || GAZ_SENSOR_SWITCHED_ON || DHT_SENSOR_SWITCHED_ON){
          SwitchOnOffModule(false,Switch_PIN_MH_Moisture_Sensor,MOISTURE_SENSOR_SWITCHED_ON);
          SwitchOnOffModule(false,Switch_PIN_MQ135_Gaz_Sensor,GAZ_SENSOR_SWITCHED_ON);
          SwitchOnOffModule(false,Switch_PIN_DHT_Sensor,DHT_SENSOR_SWITCHED_ON);
          LCD_Switch(false);
        };
        break;
      } else if(millis() - timer_print_lcd_self >= PERIOD_PRINT_LCD_self){
        sensorsValuesPrintLCD();
        checkOverflow(timer_print_lcd_self, PERIOD_PRINT_LCD_self);
      };
    };
    RequestModeProcess = false;
  };
};

void CheckSensorsValues(){
  if(GetValueSensor(1) >= 80){
    Serial.print(F("!!Critical value the soil sensor is "));
    Serial.println(GetValueSensor(1));
    return;
  };
  if(GetValueSensor(2) >= 60){
    Serial.print(F("!!Critical value the gaz sensor is "));
    Serial.println(GetValueSensor(2));
    return;
  };
  if(GetValueSensor(3) >= 80 || GetValueSensor(3) <= 20){
    Serial.print(F("!!Critical value the humidity sensor is "));
    Serial.println(GetValueSensor(3));
    return;
  };
  if(GetValueSensor(4) >= 40 || GetValueSensor(4) <= 20 ){
    Serial.print(F("!!Critical value the temperature sensor is "));
    Serial.println(GetValueSensor(4));
    return;
  };
  Serial.println(F("!!The sensors are correct!"));
};
