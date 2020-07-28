void HTTP_SetUp(){
//-------------------HTTP_Events-------------------------------------------------------------------
  HTTP.onNotFound([](){
    if(!handleFileRead(HTTP.uri())){
      Event_NotFound();
    }
  });
//------------------------------------------------------------HTTP_Commands-----------------------------------------------------------------
  HTTP.on("/commands", HTTPcommands);
//------------------------------------------------------------------------------------------------------------------------------------------
  HTTP.begin();
}
//---------Use_File_System--------------------------------------------------------------------
bool handleFileRead(String path){//File system
  if(path.endsWith("/")) path += "index.html"; // if we have a root address open index.html
  String contentType = getContentType(path); //get a type of the file
  if(SPIFFS.exists(path)){ // if the file is exist open one for reading
    File file = SPIFFS.open(path, "r");
    size_t sent = HTTP.streamFile(file, contentType); //show the content of the file
    file.close();
    printLN_to_SerialPort(F("\nFileSystem"));// close the file
    return true;
  };
  return false;
};
String getContentType(String filename){//get a title of the type file
  if (filename.endsWith(".html")) return F("text/html");
  else if (filename.endsWith(".css")) return F("text/css");
  else if (filename.endsWith(".js"))  return F("application/javascript");
  else if (filename.endsWith(".jpg")) return F("image/jpeg");
  else if (filename.endsWith(".gif")) return F("image/gif");
  else if (filename.endsWith(".ico")) return F("image/x-icon");
  return F("text/plain");
};
//----------------------------------------------------------------HTTP_requests_manager----------------------------------------------------------------
void Event_NotFound(){
  String message = String("File Not Found") + "\n"
                          "URI: " + HTTP.uri() + "\n"
                          "Method: " + String((HTTP.method() == HTTP_GET)?"GET\n":"POST") + "\n"
                          "Arguments: " + HTTP.args() + "\n";

  for (uint8_t i=0; i<HTTP.args(); i++){
    message += " " + HTTP.argName(i) + ": " + HTTP.arg(i) + "\n";
  };
  HTTP.send(404, F("text/plain"), message);
};

// Перезагрузка модуля по запросу вида http://192.168.0.101/commands?command=ip
void HTTPcommands(){
    String RecievedCommand = HTTP.arg("command");
    Serial.print("The server has recieved a http command: ");
    if(RecievedCommand == "ip"){
      Serial.println("ip.");
      HTTP.send(200, F("text/plain"),"{\"ServerIP\":\"" + getIP() + "\"}");
    }else if(RecievedCommand == "command_list"){
      Serial.println("command_list.");
      HTTP.send(200, F("text/plain"),CreateJSONCommandStack());
    }else if(RecievedCommand == "restart"){
      Serial.println("restart.");
      ESP.restart();
    }else{
      RecievedCommand = RecievedCommand + " is not found. Check and repeat.";
      HTTP.send(200, F("text/plain"),RecievedCommand);
    };
};
