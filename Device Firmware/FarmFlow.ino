


#include <Arduino.h>

#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "nicK"
#define WIFI_PASSWORD "kipnick56"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDNiVIY3EyV_oH-a7PWE732M8BEmDkqQGo"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "farmflow@test.com"
#define USER_PASSWORD "farmflow001"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://farmflow-5403c-default-rtdb.firebaseio.com"


//PINS
#define DHTPIN 2 
#define moisture_sensor A0 
#define LED_GREEN D1
#define LED_RED D2
#define RELAY D3

#define DHTTYPE    DHT11     // DHT 11


// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String tempPath = "/temperature";
String humPath = "/humidity";
String moisturePath = "/moisture";
String timePath = "/timestamp";

// Parent Node (to be updated in every loop)
String parentPath;

FirebaseJson json;

// Define NTP Client to get time
const long utcOffsetInSeconds = 10800; //(UTC+3) Nairobi

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variable to save current epoch time
String timestamp;
int irrigation_state;


//DHT SENSOR
DHT dht(DHTPIN, DHTTYPE);

float h;
float t;
int moisture_data;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 3000;


// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

void setup(){
  Serial.begin(115200);

  // Initialize DHT11 sensor
  dht.begin();
  initWiFi();
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";
}


//**********OUR FUNCTIONS************

// READ DHT DATA
void readDHT()
{
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();

}
  //READ MOISTURE SENSOR DTATA
void readMoisture()
{
  moisture_data = analogRead(moisture_sensor);
}



  //SEND DATA TO FIREBASE
void sendData()
{
  readDHT();
  readMoisture();

  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //Get current timestamp
    timestamp = timeClient.getFormattedTime();
    Serial.print ("time: ");
    Serial.println (timestamp);

    parentPath= databasePath + "/" + String(timeClient.getDay());

    json.set(tempPath.c_str(), String(h));
    json.set(humPath.c_str(), String(t));
    json.set(moisturePath.c_str(), String(moisture_data));
    json.set(timePath, String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
  }
}

  //READ DATA FROM FIREBASE
void readData()
{

    //Reading Irrigation State
   if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getInt(&fbdo, parentPath +"/irrigation_state")) {
      if (fbdo.dataType() == "int") {
        irrigation_state = fbdo.intData();
        Serial.println(irrigation_state);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    
    /*
    if (Firebase.RTDB.getFloat(&fbdo, "/test/float")) {
      if (fbdo.dataType() == "float") {
        floatValue = fbdo.floatData();
        Serial.println(floatValue);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    */


  }
}

//ACTUATE PUMP, LEDS AND DISPLAY
void actuation()
{
      //MOISTURE IS LESS OR IRRIGATION TRIGGERED FROM APP
  if (moisture_data < 21  || irrigation_state == 1)
    { 
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(RELAY, HIGH);
      //analogWrite(servo,180);
    }


  else if (moisture_data > 21  || irrigation_state == 0)
  {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(RELAY, HIGH);
    //analogWrite(servo,180);
  }
    
}



//CODE TO RUN CONTINUOUSLY
void loop(){

  sendData();
  readData();
  actuation()
  
}
