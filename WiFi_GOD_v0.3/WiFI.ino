const PROGMEM char *first_WiFi_ssid  = "VOSBS",       *first_WiFi_password  = "Privet123";
const PROGMEM char *second_WiFi_ssid = "TP-LINK_707E",*second_WiFi_password = "12345678";
const PROGMEM char *third_WiFi_ssid  = "Z10",         *third_WiFi_password  = "12345678";
const PROGMEM char *fourth_WiFi_ssid = "LAPTOP-Q0ULH7NJ 2043",*fourth_WiFi_password  = "12345678910";

bool condition_WiFi_AP_switch_OFF = true;
const PROGMEM char *own_WiFi_ssid = "ESP_WiFi_Point", *own_WiFi_password = "12345678";

//--------------------------------------------------WiFi_functions--------------------------------------------------------------------
void add_WiFi_Points(){
    wifiMulti.addAP(first_WiFi_ssid, first_WiFi_password);
    wifiMulti.addAP(second_WiFi_ssid , second_WiFi_password);
    wifiMulti.addAP(third_WiFi_ssid , third_WiFi_password);
    wifiMulti.addAP(fourth_WiFi_ssid , fourth_WiFi_password);
    //add point here
};
void WiFi_SetUp(){
  if(condition_WiFi_AP_switch_OFF && WiFi.status()!= WL_CONNECTED){
    WiFi_Monitor();
    delay(500);
  };
};
void WiFi_Monitor(){
  add_WiFi_Points();
  WiFi.mode(WIFI_STA);
  int8_t attemts = 20;
  Serial.print(F("\nConnecting"));

  while(--attemts && wifiMulti.run() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  };
  if(wifiMulti.run()!= WL_CONNECTED){
    Serial.println(F("\nThe connetion was failed, start AP mode"));
    condition_WiFi_AP_switch_OFF = false;
    use_WiFi_AP();
  }else{
    Serial.print(F("\nWiFi is connected: "));
    Serial.print(WiFi.SSID());
    Serial.print(F(" - "));
    Serial.println(WiFi.localIP());
  };
};
bool use_WiFi_AP(){
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_IP, ap_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(own_WiFi_ssid,own_WiFi_password);
    Serial.println(F("\nWiFi AP is started: "));
    Serial.print(own_WiFi_ssid);
    Serial.println(F(" - 192.168.0.8")); //Serial.print(WiFi.softAPIP());
  return true;
};
