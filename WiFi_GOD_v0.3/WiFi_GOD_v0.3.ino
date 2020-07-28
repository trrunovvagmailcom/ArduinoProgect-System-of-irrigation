//----Izotov_Vladislav_ESP-01-----------------------------------------------------
#include <ESP8266WiFiMulti.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include <ESP8266FtpServer.h>
#include <avr/pgmspace.h>
#include <FS.h>

ESP8266WiFiMulti wifiMulti;
IPAddress ap_IP(192, 168, 0, 8);

FtpServer FTP_Server;
WebSocketsServer webSocket = WebSocketsServer(81);

ESP8266WebServer HTTP(80);
StaticJsonDocument<200> JSONdocument;

SoftwareSerial swSer(0,2);

#define SPEED_PORT 9600
#define PERIOD_SEND_DATA 5000
uint32_t timer_send_data = 0;

uint32_t Real_PERIOD_SEND_DATA = 120000;
uint32_t Real_timer_send_data = 0;

//Thread* thread;
bool realDataOn = false;
//-------------------------------------------------------*____Main_Function____*-------------------------------------------------------------------------------------
void setup() {
  Serial.begin(SPEED_PORT);
  swSer.begin(SPEED_PORT);
  WiFi_SetUp();
  WebSoket_SetUp();
  HTTP_SetUp();
  FTP_SetUp();
  SSDP_SetUp();

};

void loop() {
  WiFi_SetUp();
  HTTP.handleClient();
  webSocket.loop();
  FTP_Server.handleFTP();
  SerialPortListener();
  PsevdoOrRealData();

};
//------------------------------------------------------------------------------------------
void PsevdoOrRealData(){
  if(realDataOn && millis() - Real_timer_send_data >= Real_PERIOD_SEND_DATA ){
    GetSensorsData();
    checkOverflow(Real_timer_send_data, Real_PERIOD_SEND_DATA);
  };
  if(!realDataOn && millis() - timer_send_data >= PERIOD_SEND_DATA){
    PsevdoSendToServer();
    checkOverflow(timer_send_data, PERIOD_SEND_DATA);
  };
};

void printLN_to_SerialPort(const __FlashStringHelper *value){
  Serial.println(value);
};
void print_to_SerialPort(const __FlashStringHelper *value){
  Serial.print(value);
};

void checkOverflow(uint32_t &__timer, uint16_t __period) {
  do {__timer += __period;
    if (__timer < __period) break;  // переполнение uint32_t
  } while (__timer < millis() - __period); // защита от пропуска шага
};
