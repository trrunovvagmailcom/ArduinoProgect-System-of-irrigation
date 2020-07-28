#define SSDP_Name "ESP Description"
#define URL_gitHub_Description "https://github.com/trrunovvagmailcom/ESP-01-WiFi-Multi-Client-and-WiFi-AP"
#define Autor_Name "Izotov Vladislav"
#define URL_Autor_Description "https://vk.com/tot_camyi_rembo"

void SSDP_SetUp(){
  HTTP.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(HTTP.client());
  });
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(SSDP_Name);
  SSDP.setSerialNumber("1234567890");
  SSDP.setURL("/");
  SSDP.setModelName("ESP-01");
  SSDP.setModelNumber("1.2");
  SSDP.setModelURL(URL_gitHub_Description);
  SSDP.setManufacturer(Autor_Name);
  SSDP.setManufacturerURL(URL_Autor_Description);
  SSDP.begin();
};
