#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include <FS.h>   // Include the SPIFFS library
#include "configuration.h"

//IPAddress  Hotspot_IP= apIP(10, 10, 10, 10);
//IPAddress  Hotspot_SUBNET= apIP(255, 255, 255, 0);

#ifdef Enable_webServer
ESP8266WebServer webServer(80);
#endif
#ifdef Enable_WebSocketsServer
WebSocketsServer webSocket = WebSocketsServer(81);
#endif
#ifdef Enable_DNSServer
DNSServer dnsServer;
#endif
int offset=0;
unsigned long timesend=0; 
void setup() {
  #ifdef Enable_Serial
  Serial.begin(baud_rate);
  Serial.println("Seriale Attivato");
  #endif
  #ifdef Enable_External_Hotspot
       Serial.print("Connessione a ");
       Serial.println(External_Hotspot_SSID);  
       WiFi.begin(External_Hotspot_SSID, External_Hotspot_Password); 
       while (WiFi.status() != WL_CONNECTED) 
          {  
            delay(500);          
            Serial.print(".");            
          }
      Serial.println("");
      Serial.println("WiFi Connesso"); 
      Serial.print("Indirizzo Ip: ");
      Serial.println(WiFi.localIP());
  #endif
  #ifdef Enable_Hotspot
      WiFi.mode(WIFI_AP);
      //WiFi.softAPConfig(Hotspot_IP, Hotspot_IP, Hotspot_SUBNET);
      WiFi.softAP(Hotspot_SSID,Hotspot_Password);
      Serial.print("HotSpot WiFI creato,Nome: ");
      Serial.print(Hotspot_SSID);
      Serial.print(" password: ");
      Serial.println(Hotspot_Password);
  #endif
  #ifdef Enable_mDNS
      if (!MDNS.begin(mDNS_Name))
    {
      Serial.println("Error setting up MDNS responder!");
    }
    else  
    {
    Serial.print("mDNS avviato, connetiti a ");
    Serial.print(mDNS_Name);
    Serial.println(".local");    
    }
  delay(20);
  #endif
  #ifdef Enable_DNSServer
  dnsServer.start(DNSServer_port, "*", WiFi.localIP());
  Serial.print("Server DNS attivato sulla porta ");
  Serial.println(DNSServer_port);
  delay(20);
  #endif
  #ifdef Enable_webServer
  SPIFFS.begin();                         
  webServer.onNotFound([]() {                              
    if (!handleFileRead(webServer.uri()))
      webServer.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  webServer.begin();
  Serial.print("Server web attivato sulla porta ");
  Serial.println(webServer_port);
  delay(20);
  #endif
  #ifdef Enable_WebSocketsServer
  webSocket.begin();
  webSocket.onEvent(RiceviDalSocket);
  Serial.print("Server webSocket attivato sulla porta ");
  Serial.println(WebSocketsServer_port);
  delay(20);
  #endif
  InizializzaGiroscopio();
  SetOffset();
  }

void loop() {
  #ifdef Enable_DNSServer
  dnsServer.processNextRequest();
  #endif
  #ifdef Enable_webServer
  webServer.handleClient();
  #endif
  #ifdef Enable_WebSocketsServer
  webSocket.loop();
  #endif
  if(millis()-timesend>MillisBetweenSend)
  {
    String inclinazione=String(Angolo());
    #ifdef Enable_Serial
    Serial.println(inclinazione);
    #endif
    #ifdef Enable_WebSocketsServer
    webSocket.broadcastTXT(inclinazione);    
    #endif
    timesend=millis();
  }
  /*//
  webServer.handleClient();
  webSocket.loop();
  if (Serial.available() > 0){
      char c[] = {(char)Serial.read()};
      webSocket.broadcastTXT(c, sizeof(c));
      
    }
   if(millis()-timesend>500)
  {
            String inclinazione=String(Angolo());
            webSocket.broadcastTXT(inclinazione);
            timesend=millis();
  }
      */
}
void RiceviDalSocket(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
   if (type == WStype_TEXT){
    for(int i = 0; i < length; i++) 
    {
      Serial.print((char) payload[i]);
      if(((char) payload[i])=='0')
            SetOffset();            
    }
    Serial.println();
   }
}

int Angolo(){
  int* ang;
  ang=Getangles();
  
  
//     Serial.print(x);
//     Serial.println((char)176);
//
//     Serial.print("Angle of inclination in Y axis= ");
//       Serial.println(round (ang[1]));
if((ang[1]-offset)>=360)
{
  return (ang[1]-offset)-360;
}else if((ang[1]-offset)<0)
{
  return 360+(ang[1]-offset);
}else
{
  return (ang[1]-offset);   
 } 
}
void SetOffset(){
  offset=Getangles()[1];
}
int *Getangles()
{
    static int angolo[3];
    static int16_t axis_X,axis_Y,axis_Z;
    Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  axis_X=Wire.read()<<8|Wire.read();
  axis_Y=Wire.read()<<8|Wire.read();
  axis_Z=Wire.read()<<8|Wire.read();
    int xAng = map(axis_X,GyrominVal,GyromaxVal,-90,90);
    int yAng = map(axis_Y,GyrominVal,GyromaxVal,-90,90);
    int zAng = map(axis_Z,GyrominVal,GyromaxVal,-90,90);

     int  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
     int  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
     int  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
    
    angolo[0] = round (x);
    angolo[1] = round (y);
    angolo[2] = round (z);
    return angolo;
}

void InizializzaGiroscopio()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(115200);
 }

 String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".ttf")) return "application/octet-stream";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = webServer.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
