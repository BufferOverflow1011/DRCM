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

 //define the pins used by the LoRa transceiver module
#define ss 15
#define rst 16
#define dio0 2

#define BAND 433E6    

//packet counter
int readingID = 0;
int counter = 0;
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
    String req1;
    String req2;
    String req3;
    String req4;
    String inputMessage5;

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

void webpage(){
 SPIFFS.begin();
   if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  };




//dnsServer.start(53, "*", WiFi.softAPIP());
//  server.addHandler(new CaptiveRequestHandler());
 // server.begin();


if (MDNS.begin("DARK")) {        //dark.local/
    Serial.println("MDNS responder started ...");
   Serial.println("Type   dark.local/  in browser");
  }

// Route for root / web page
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

 LoRaMessage = inputMessage1 + "\n" +  inputMessage2 + "\n" +  inputMessage3 + "\n"+  inputMessage4 + "\n" +  req1 + "\n" +  req2 + "\n" +  req3  + "\n" +  req4 + "\n"+ inputMessage5;
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

      webpage();

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
void setup() {
Serial.begin(9600);
startLoRa();
SoftAP();
webpage();

 dnsServer.start(53, "*", WiFi.softAPIP());
server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
}


void loop() {
   if (Flag == 0) {
   sendReadings();
   req1 ="";
req2 = "";
req3 = "";
req4 = "";
  // Serial.print(Flag);
   delay(2000); 
    Flag++;  }
     delay(1000);

inputMessage5 = "";
LoRaMessage = "";
dnsServer.processNextRequest();
   MDNS.update();  //  dnsServer.processNextRequest();       
        
}
