#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif

#include <SPI.h>
#include <LoRa.h>


#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
 //define the pins used by the LoRa transceiver module
#define ss 15
#define rst 16
#define dio0 2

#define BAND 433E6    

int RXPin = 2;
int TXPin = 0;
int GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);
//packet counter
int readingID = 0;
int counter = 0;
String latlong;
String gps_date;
String gps_time;
String gps_altitude;

 IPAddress ip(1,2,3,4);
 IPAddress gateway(1,2,3,4);
 IPAddress subnet(255,255,255,0);
String LoRaMessage = "";

AsyncWebServer server(80);


const char* input1value = "input1";
const char* input2value = "input2";
const char* input3value = "input3";
const char* input4value = "input4";
const char* needcvalue = "needc";
const char* needfvalue = "needf";
const char* needavalue = "needa";
const char* needevalue = "neede";
  String inputMessage1 ,inputMessage2, inputMessage3, inputMessage4 ;
    String inputParam;
    String req1 = "X";
    String req2 = "X";
    String req3 = "X";
    String req4 = "X";
    String inputMessage5 = "aa";

   int Flag;

//Initialize LoRa module
void startLoRa(){

  LoRa.setPins(ss, rst, dio0); //setup LoRa transceiver module
 
  while (!LoRa.begin(BAND) && counter < 10) {
    Serial.print(".");
    counter++;
    delay(500);
  }
  if (counter == 10) 
  {
    // Increment readingID on every new reading
   // readingID++;
    Serial.println("Starting LoRa failed!"); 
  }
  Serial.println("LoRa Initialization OK!");
  delay(2000);
}
void displayInfo()

{
  if (gps.location.isValid())
  {


  char c_lat[7];
  sprintf(c_lat, "%.7f" , gps.location.lat());

char c_log[7];
sprintf(c_log, "%.7f" , gps.location.lng());

latlong = String(c_lat) + "," + String(c_log);
  Serial.println(latlong);
   c_lat[7] = '\0';
  c_log[7] = '\0';

    gps_altitude = gps.altitude.meters();
    Serial.println("altitude :" + gps_altitude);
  }
  else
  {
    Serial.println("Location: Not Available");
  }

  if (gps.date.isValid())
  {

    int xdate = gps.date.month();
    int ydate = gps.date.day();
    int zdate = gps.date.year();
    gps_date = String(xdate)+"/"+String(ydate)+"/"+String(zdate);


    
    
     Serial.println("date : " +gps_date);
    
  }
  else
  {
    Serial.println("Not Available");
  }

  if (gps.time.isValid())
  {
   if (gps.time.hour() < 10);
  int xtime = gps.time.hour() ;
   if(gps.time.minute() < 10) ;
   int ytime = gps.time.minute();
  if (gps.time.second() < 10);
    int ztime = gps.time.second();
  

    gps_time = String(xtime)+":"+ String(ytime)+":"+ String(ztime);
     Serial.print("Time: " + gps_time);

  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}
void webpage(){
 SPIFFS.begin();
   if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  };


if (MDNS.begin("DARK")) {        //dark.local/
    Serial.println("MDNS responder started ...");
   Serial.println("Type   dark.local/  in browser");
  }


  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/DarkPort.html");
  });

  server.on("/Dark.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/Dark.css", "text/css");
    });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    
 

    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(input1value)) {
      inputMessage1 = request->getParam(input1value)->value();
      
    }
     
    if (request->hasParam(input2value)) {
      inputMessage2 = request->getParam(input2value)->value();

    }
    
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    if (request->hasParam(input3value)) {
      inputMessage3 = request->getParam(input3value)->value();

    }

      if (request->hasParam(input4value)) {
      inputMessage4 = request->getParam(input4value)->value();

    }

    if (request->hasParam(needcvalue)) {
      req1 = "✔ ";

    }
   
    if (request->hasParam(needfvalue)) {
      req2 = "✔ ";

      
      Serial.println(req2);
    }
     if (request->hasParam(needavalue)) {
      req3 = "✔";
      
      Serial.println(req3);
    }
     if (request->hasParam(needevalue)) {
      req4 = "✔";
      
    }

     request->send(SPIFFS,"/DarkPort.html");
      
      Flag = 0;
       
  });
 
// Start server
  server.begin();
  


}

void SoftAP(){
      
  Serial.print("Setting soft-AP Fns ... ");
  WiFi.mode(WIFI_AP);

 WiFi.softAPConfig(ip, gateway, subnet);
  boolean result = WiFi.softAP("EMERGENCY PORTAL");
  if(result == true)
  {
    Serial.println("Ready");
  
    IPAddress IP = WiFi.softAPIP();
    
    Serial.print("AP IP address: ");
  Serial.println(IP);
  }
  else
  {
    Serial.println("Failed!");
  }
}

void sendReadings() {

 LoRaMessage = inputMessage1 + "\n" +  inputMessage2 + "\n" +  inputMessage3 + "\n"+  inputMessage4 + "\n" + req1 + "\n" +  req2 + "\n" +  req3  + "\n" +  req4 ;
  //Send LoRa packet to receiver
  
LoRa.beginPacket();
LoRa.print(LoRaMessage);
LoRa.endPacket();
Serial.println(LoRaMessage);
delay(1000);

}


DNSServer dnsServer;

  
class CaptiveRequestHandler : public AsyncWebHandler {
  public:
    CaptiveRequestHandler() {
      /* THIS IS WHERE YOU CAN PLACE THE CALLS */
       server.onNotFound([](AsyncWebServerRequest *request){
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/NotFound.html", "text/html");
        request->send(response);
        
       });

      server.on("/DarkPort.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/DarkPort.html", "text/html");
         request->send(response);
      });
      
      server.on("/Dark.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/Dark.css", "text/css");
         request->send(response);
      });
   
      // webpage();
      
    

    }
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
      //request->addInterestingHeader("ANY");
      return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/DarkPort.html", String(), false);
    }
};
void gpsstart()
{
      while (gpsSerial.available() > 0)
    //Serial.write(gpsSerial.read());
        if (gps.encode(gpsSerial.read()))
      displayInfo();
       if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }}

void setup() {
Serial.begin(9600);
gpsSerial.begin(GPSBaud);
startLoRa();
SoftAP();
webpage();

 dnsServer.start(53, "*", WiFi.softAPIP());
server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
}


void loop() {




 if (Flag == 0) {
   

  sendReadings();
   req1 ="X";
req2 = "X";
req3 = "X";
req4 = "X";
  // Serial.print(Flag);
   delay(2000); 
    Flag++;
    inputMessage5 = "";
LoRaMessage = "";
    }
    //<-------------gps module neo6m enable -------->
//    gpsstart(); 
   delay(1000);

dnsServer.processNextRequest();
 MDNS.update();
dnsServer.processNextRequest();       
        
}
