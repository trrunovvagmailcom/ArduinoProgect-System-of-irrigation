void SerialPortListener() {
  if (swSer.available() > 0) {
    String recievedFromArduino = swSer.readString();
    String arduinoSerialPort = "A:" + recievedFromArduino + "\0";
    webSocket.broadcastTXT(recievedFromArduino);
    Serial.println(arduinoSerialPort);
    if (recievedFromArduino[0] == '{' && recievedFromArduino[2] == 'S') {
      //deserializeJson(JSONdocument, recievedFromArduino);
      recievedFromArduino = recievedFromArduino + "\0";
      webSocket.broadcastTXT(recievedFromArduino);
      //Serial.println (broadcast1);
    };
  };
};
void GetSensorsData(){
  String Request = "{\"com\":3}";
  swSer.println(Request);
};
void PsevdoSendToServer() {
  uint8_t Ss = 0 + rand() % 100;
  uint8_t Gs = 0 + rand() % 100;
  uint8_t Hs = 0 + rand() % 100;
  uint8_t Ts = 0 + rand() % 100;
  String DataSensor = String("{\"S\":") + Ss + ",\"G\":" + Gs + ",\"H\":" + Hs + ",\"T\":" + Ts + "}\n";
  Serial.print(DataSensor);
  webSocket.broadcastTXT(DataSensor);
};
