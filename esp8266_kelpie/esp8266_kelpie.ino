
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: k_suwatchai@hotmail.com
 *
 * Github: https://github.com/mobizt/Firebase-ESP-Client
 *
 * Copyright (c) 2023 mobizt
 *
 */

#include <Arduino.h>
#if defined(ESP32) || defined(PICO_RP2040)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "APIKEY"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://kelpie-a8758-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "user-email"
#define USER_PASSWORD "user-password"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;


unsigned long count = 0;
int ON =1;
int OFF= 0;
uint8_t Device_Pin = D5; 

void setup()
{
  pinMode(Device_Pin, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  //////////////////////////////////////////////////////////////////////////////////////////////
  // Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  // otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

#if defined(ESP8266)
  // In ESP8266 required for BearSSL rx/tx buffer for large data handle, increase Rx size as needed.
  fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
#endif

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;
  

 
}







void loop()
{

  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

   Serial.printf("Get REQUEST STATUS... %s\n", Firebase.RTDB.getInt(&fbdo, F( "useremail@gmailDOTcom/REQ_STATUS")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
   int read_data = fbdo.intData();
   switch (read_data){
    
    case 0:
    
        Serial.printf("Set LED VALUE... %s\n", Firebase.RTDB.setInt(&fbdo, F("useremail@gmailDOTcom/LED_VALUE"), OFF) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get LED STATUS... %s\n", Firebase.RTDB.getInt(&fbdo, F("useremail@gmailDOTcom/LED_VALUE")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());   
        digitalWrite(Device_Pin,LOW);             
        break;
        
    case 1:
    
        Serial.printf("Set LED VALUE... %s\n", Firebase.RTDB.setInt(&fbdo, F("useremail@gmailDOTcom/LED_VALUE"), ON) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get LED STATUS... %s\n", Firebase.RTDB.getInt(&fbdo, F("useremail@gmailDOTcom/LED_VALUE")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str()); 
        digitalWrite(Device_Pin,HIGH);             
        break;

       
        

    }

    

   

   
   
  }
}
