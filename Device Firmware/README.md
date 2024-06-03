# Using the Firmware


- Arduino IDE installed on your computer. You can download it from [here](https://www.arduino.cc/en/software).
- The following libraries installed in your Arduino IDE:
  - [Firebase_ESP_Client](https://github.com/mobizt/Firebase-ESP-Client/archive/refs/tags/v4.4.14.zip) 
  - [NTPClient](https://github.com/arduino-libraries/NTPClient) 
  - [Adafruit DHT Sensor](https://github.com/adafruit/DHT-sensor-library)



## Code Modifications
Here are some parts of the code that you might want to change:

1. **WiFi Credentials:**
   ```cpp
   const char* ssid = "your_SSID";
   const char* password = "your_PASSWORD";

 
2. **Firebase Credentials:**
   ```cpp
   
    // Firebase Project API Key
    #define API_KEY "REPLACE_WITH_YOUR_PROJECT_API_KEY"
   
     // Insert Authorized Email and Corresponding Password
    #define USER_EMAIL "REPLACE_WITH_THE_USER_EMAIL"
    #define USER_PASSWORD "REPLACE_WITH_THE_USER_PASSWORD"

   // Insert RTDB URLefine the RTDB URL
    #define DATABASE_URL "REPLACE_WITH_YOUR_DATABASE_URL"
