# kelpie_esp8266
esp8266 backend of kelpie project
<br/>
<br/>
<br/>
<br/>

------------------------------------------------------------------------------------------------------
## NODE MCU BOARD PIN OUT

<br/>
<br/>

![NodeMCUv3.0-pinout.jpg](https://github.com/sooraj-nhaloor/kelpie_esp8266/blob/main/NodeMCUv3.0-pinout.jpg)

----------------------------------------------------------------------------------------------------
## PINS USED 

| GPIO | NODEMCU PIN | OUTPUT       |
| -----|-------------|------------- |
| D1   | GPIO5       | RELAY 1      |
| D2   | GPIO4       | RELAY 2      |      
| D5   | GPIO14      | RELAY 3      |      
| D6   | GPIO12      | RELAY 4      |      
| D7   | GPIO13      | WATER SENSOR |     


--------------------------------------------------------------------------------------------------------
## ARDUINO CODE FOR OTA FIRMWARE UPDATE

``` C++


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


/////////////////////////////////////////
// NETWORK CREDINALS

const char* ssid = "SSID";
const char* password = "PASSWORD";

/////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
	
	////////////////////////////////////////////////////////////////
	//<--------- WRITE YOUR CODE INSIDE ---------->
	
	
	/////////////////////////////////////////////////////////////
}

void loop() {
  ArduinoOTA.handle();
	/////////////////////////////////////////////////////////////////
	//<--------- WRITE YOUR CODE INSIDE ------->
	
	
	
	
	
	////////////////////////////////////////////////////////////////////
}
```

--------------------------------------
## ESP8266 BACKEND CODE WITH OTA SUPPORT
<br/>
<br/>



``` C++


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"



/////////////////////////////////////////
// NETWORK CREDINALS

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define API_KEY "API-ID"
#define USER_EMAIL "user@gmail.com"
#define USER_PASSWORD "user-password"
#define DATABASE_URL "DATABASE-URL"

/////////////////////////////////////////////////////////////
//INIITIALIZE COMPONENTS

FirebaseData stream;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData fbdo;


char DEVICE_STATUS[] = "user@gmailDOTcom/DEVICE_VALUE";
char REQ_STATUS[]   = "user@gmailDOTcom/REQ_STATUS";

const int RELAY_A      =   5;
const int RELAY_B      =   4;
const int RELAY_C      =  14;
const int RELAY_D      =  12;
const int WATER_SENSOR =  13;



//////////////////////////////////////////////////

void setup() {  
    
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    
    ////////////////////////////////////////////////////////////////
    //<--------- WRITE YOUR CODE INSIDE ---------->
    
        pinMode(RELAY_A,OUTPUT);
        pinMode(RELAY_B,OUTPUT);
        pinMode(RELAY_C,OUTPUT);
        pinMode(RELAY_D,OUTPUT);
        pinMode(WATER_SENSOR,OUTPUT);
 
        digitalWrite(RELAY_A,HIGH);
        digitalWrite(RELAY_B,HIGH);
        digitalWrite(RELAY_C,HIGH);
        digitalWrite(RELAY_D,HIGH);
        digitalWrite(WATER_SENSOR,HIGH);

      
    
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
    
  config.token_status_callback = tokenStatusCallback;
  fbdo.setBSSLBufferSize(2048, 2048 );
  fbdo.setResponseSize(2048);
    
  Firebase.begin(&config, &auth);
    
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;
    /////////////////////////////////////////////////////////////
}

void loop() {
  ArduinoOTA.handle();
    /////////////////////////////////////////////////////////////////
    //<--------- WRITE YOUR CODE INSIDE ------->
    
    if (Firebase.ready()){
        
    Firebase.RTDB.getInt(&fbdo, REQ_STATUS);
    int read_data = fbdo.intData();
        switch(read_data){
            case 0: //motor off
                digitalWrite(RELAY_A,HIGH);
                if(digitalRead(RELAY_A)== HIGH){
            Firebase.RTDB.setInt(&fbdo,DEVICE_STATUS,0);
                }
                
                break;
            case 1: //motor on
                digitalWrite(RELAY_A,LOW);
                Firebase.RTDB.setInt(&fbdo,DEVICE_STATUS,1);
                delay(10000);
                
                break;
            
        }
        
    }
    
   CheckDryRun();

    
    ////////////////////////////////////////////////////////////////////
}
    
void CheckDryRun(){
    if (digitalRead(RELAY_A)== LOW && digitalRead(WATER_SENSOR) == LOW){
                digitalWrite(RELAY_A,HIGH);
                Firebase.RTDB.setInt(&fbdo,REQ_STATUS,0);
                Firebase.RTDB.setInt(&fbdo,DEVICE_STATUS,2);

    }
   
                }


```
