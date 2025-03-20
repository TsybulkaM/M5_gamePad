#ifndef SERVICES_HPP
#define SERVICES_HPP

#include "secrets.hpp"
#include <WiFi.h>

class Battery {
private:
  uint8_t prev_batteryPercentage;
  uint8_t batteryPercentage;
  float MIN_VOLTAGE;
  float MAX_VOLTAGE;
public:
  Battery()
    : MIN_VOLTAGE(3.0), MAX_VOLTAGE(4.2) {}

  void updateAndDisplay(uint8_t x, uint8_t y, uint8_t front_size, uint32_t text_color, uint32_t bg_color) {
    float batteryVoltage = M5.Axp.GetBatVoltage();

    float normalizedVoltage = (batteryVoltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE);
    if (normalizedVoltage > 1.0) normalizedVoltage = 1.0;
    if (normalizedVoltage < 0.0) normalizedVoltage = 0.0;

    batteryPercentage = pow(normalizedVoltage, 0.75) * 100;

    if (batteryPercentage > 100) batteryPercentage = 100;
    if (batteryPercentage < 0) batteryPercentage = 0;

    if (prev_batteryPercentage != batteryPercentage) {
      prev_batteryPercentage = batteryPercentage;
      this->display(x, y, front_size, text_color, bg_color);
    }
  }

  void display(uint8_t x, uint8_t y, uint8_t front_size, uint32_t text_color, uint32_t bg_color) {
    M5.Lcd.setCursor(x, y);
    M5.Lcd.setTextSize(front_size);
    M5.Lcd.setTextColor(text_color, bg_color);
    if (M5.Axp.GetVBusVoltage() > 4.5) {
      M5.Lcd.printf("%d%% CHG", batteryPercentage);
    } else {
       M5.Lcd.printf("%d%%    ", batteryPercentage);
    }
    
  }
};

class WiFiManager {
public:
  static bool isConnected() {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected!");
      return true;
    } else {
      Serial.println("\nFailed to connect.");
      return false;
    }
  }

  static bool connect() {
    for (int i = 0; i < numNetworks; ++i) {
      const char* currentSSID = ssidList[i];
      const char* currentPassword = passwordList[i];

      Serial.print("Connecting to: ");
      Serial.println(currentSSID);

      WiFi.disconnect();
      delay(100);

      if (strlen(currentPassword) > 0) {
        WiFi.begin(currentSSID, currentPassword);
      } else {
        WiFi.begin(currentSSID);
      }

      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        Serial.print(WiFi.status());
        delay(500);
        Serial.print(".");
        attempts++;
      }

      if (isConnected()) {
        return true;
      } else {
        WiFi.disconnect();
      }
    }

    return false;
  }

  static void disconnect() {
    WiFi.disconnect();
    Serial.println("WiFi disconnected.");
  }
};

#endif  // SERVICES_HPP