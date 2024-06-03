# Using the Firmware


- Arduino IDE installed on your computer. You can download it from [here](https://www.arduino.cc/en/software).
- The following libraries installed in your Arduino IDE:
  - [Firebase_ESP_Client]([link](https://github.com/mobizt/Firebase-ESP-Client/archive/refs/tags/v4.4.14.zip)) 
  - [NTPClient]([link](https://github.com/arduino-libraries/NTPClient)) 
  - [DHT](link)



## Code Modifications
Here are some parts of the code that you might want to change:

1. **WiFi Credentials:**
   ```cpp
   const char* ssid = "your_SSID";
   const char* password = "your_PASSWORD";
