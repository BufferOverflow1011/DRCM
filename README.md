# DRCM
esp8266 based client and server (rpi) communication project with lora
 this project is a poc for an emergency communication during a network failure due to natural or any other disasters 

 # WORKING
 The esp8266 tether a hotsport and when smeone connects to it the opens a offline webpage with data entry forms . the entered data is transmitted via lora to rpi server located in a distance and then uploads through cloud


# ESP8266 OR ESP32
 ## CLIENT SIDE
   * `/esp8266_or32/Data`
   This folder contains the html page which loads in the client side 
   *the data is uploaded via spiffs

# RASPBERRY
 ## SERVER SIDE
   *accepts client form data via lora 
   * uploads to cloud 
   **configure `server.py` with firebase url**

#  WEBPAGE
 ## CLOUD 
   *sort and list the firebase data in to table
   **configure `index.html` with firebase config and api**
             
