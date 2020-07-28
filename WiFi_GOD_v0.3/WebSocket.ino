void WebSoket_SetUp() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println(F("WebSocket is working"));
};
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  String WebSocketNotification;
  switch (type) {
    case WStype_DISCONNECTED:
      WebSocketNotification = String("Disconnected! Clien number - ") + num + "\n";
      break;
    case WStype_CONNECTED:{
      IPAddress ip = webSocket.remoteIP(num);
      WebSocketNotification = "Сonnection " + String(num) + " from: " + ip.toString() + "\n";
      webSocket.sendTXT(num, "Accepted WebSocketESP\n");
      break;
    }
    case WStype_TEXT:{
      char *CharPayload = (char*)payload;
      WebSocketNotification = String(num) + " text:" + CharPayload + " ";
      //webSocket.sendTXT(num, "Request is recieved.");
      RequestManager(num, payload);
      break;
    }
    case WStype_BIN:
      WebSocketNotification = String(num) + " - get binary length:" + length + "\n";
      hexdump(payload, length);
      break;
  };
  Serial.println(WebSocketNotification);
};

const char* Commands[] = {"data","ip","set time","set pomp","restart","Arestart","get commands"};

void RequestManager(uint8_t __num, uint8_t * __UserRequest) {
  char *CharUserRequest = (char*)__UserRequest;

  if (__UserRequest[0]==62){//62 == '>' in char type
    //uint8_t FromCharToInt = (uint8_t)__UserRequest[1] - 48;//number in table figures starts with 0(48) + the following
    switch(__UserRequest[1]){
      case '0':
        Serial.print("Command 0:data");
        GetSensorsData();
        realDataOn = true;
        Serial.println(realDataOn);
        webSocket.sendTXT(__num, "Data");
        break;
      case '1':{
        Serial.print("Command 1:ip");
        String IPmod = "{\"ServerIP\":\"" + getIP() + "\"}";
        Serial.println(IPmod);
        webSocket.sendTXT(__num, IPmod);
        break;
      };
      case '2':
        Serial.print("Command 2:set time");
        swSer.print(CharUserRequest);
        webSocket.sendTXT(__num, "Send to Arduino");
        //webSocket.sendTXT(__num, JSONdocument);
        break;
      case '3':
        Serial.print("Command 3:set pomp");
        swSer.print(CharUserRequest);
        webSocket.sendTXT(__num, "Send to Arduino");
        break;
      case '4':
        Serial.print("Command 4:restart");
        webSocket.sendTXT(__num, "Command 4:restart");
        ESP.restart();
        break;
      case '5':
        Serial.print("Command 5:Arduino restart");
        webSocket.sendTXT(__num, "Command 5:Arduino restart");
        ESP.restart();
        break;
      case '6':{
        Serial.print("Command 6:command_list");
        String commands = CreateJSONCommandStack();
        webSocket.sendTXT(__num, commands);
        break;
      };
    };
  };
};
void CropMassive(char *Mass, uint8_t deletElenents){
  uint8_t lengthOfArray = (sizeof(Mass) / sizeof(char *));
  for(uint8_t i = 0 ; i < lengthOfArray - deletElenents; i++){
    Mass[i] = Mass[i + deletElenents];
  };
};

String getIP() {
  return (WiFi.localIP()) ? WiFi.localIP().toString():"192.168.0.8";
};

String CreateJSONCommandStack(){
  uint8_t lengthOfCommandsArray = (sizeof(Commands) / sizeof(char *));
  String JSONstringCommandsArray = "{";
  for(uint8_t i = 0 ; i < lengthOfCommandsArray-1; i++){
    JSONstringCommandsArray += String("\"") + Commands[i] + "\":" + i+ ",";
  };
  JSONstringCommandsArray += String("\"") + Commands[lengthOfCommandsArray-1] + "\":" + (lengthOfCommandsArray-1) + "}";
  Serial.print(JSONstringCommandsArray);
  return JSONstringCommandsArray;
};
