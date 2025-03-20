#ifndef RTCAPP_HPP
#define RTCAPP_HPP

#include "GameArch.hpp"

class RTCapp : public ArcadeGame {
private:
  RTC_TimeTypeDef RTC_TimeStruct;
  String country;

  const uint8_t BUTTON_PIN;
  const char* ntpServer;
  String timezoneUrl;

  WiFiUDP ntpUDP;
  NTPClient timeClient;

  uint8_t pastHours, pastMinutes, pastSeconds;

public:
  RTCapp()
    : pastSeconds(-1),
      BUTTON_PIN(37),
      ntpServer("pool.ntp.org"),
      timezoneUrl("http://ipwho.is/"),
      timeClient(ntpUDP, ntpServer) {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
  }

  void execute() override {
    this->Setup();
    while (this->MainLoop()) {}
  }

  bool Setup() override {
    if (!WiFiManager::isConnected()) {
      if (!WiFiManager::connect()) {
        return false;
      }
    }

    HTTPClient http;
    http.begin(timezoneUrl);

    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      Serial.println(httpResponseCode);
      String payload = http.getString();

      DynamicJsonDocument jsonDoc(1024);
      deserializeJson(jsonDoc, payload);

      JsonObject timezone = jsonDoc["timezone"];
      int offset = timezone["offset"];

      country = jsonDoc["country"].as<String>();

      timeClient.setTimeOffset(offset);
    }
    http.end();

    timeClient.begin();
    timeClient.update();

    RTC_TimeTypeDef TimeStruct;
    TimeStruct.Hours = timeClient.getHours();
    TimeStruct.Minutes = timeClient.getMinutes();
    TimeStruct.Seconds = timeClient.getSeconds();

    M5.Rtc.SetTime(&TimeStruct);

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setRotation(1);

    return true;
  }

  bool MainLoop() override {
    M5.update();

    if (M5.BtnA.wasPressed()) {
      Serial.println("Entering Light Sleep...");

      M5.Lcd.writecommand(ST7789_DISPOFF);
      M5.Axp.SetLDO2(false);

      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }

      esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, LOW);
      esp_light_sleep_start();

      Serial.println("Woke up from Light Sleep!");

      M5.Axp.SetLDO2(true);
      M5.Lcd.writecommand(ST7789_DISPON);
      M5.Lcd.fillScreen(BLACK);
    }

    if (M5.BtnB.wasPressed()) {
      return false;
    }

    M5.Rtc.GetTime(&RTC_TimeStruct);

    if (pastSeconds != RTC_TimeStruct.Seconds) {
      M5.Lcd.setTextColor(BLACK, BLACK);
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(35, 45);
      M5.Lcd.printf("%02d:%02d:%02d", pastHours, pastMinutes, pastSeconds);

      pastHours = RTC_TimeStruct.Hours;
      pastMinutes = RTC_TimeStruct.Minutes;
      pastSeconds = RTC_TimeStruct.Seconds;

      M5.Lcd.setTextColor(WHITE, BLACK);
      M5.Lcd.setCursor(35, 45);
      M5.Lcd.printf("%02d:%02d:%02d", pastHours, pastMinutes, pastSeconds);

      M5.Lcd.setTextColor(BLACK, BLACK);
      M5.Lcd.setCursor(40, 75);
      M5.Lcd.printf(country.c_str());

      M5.Lcd.setTextColor(MAGENTA, BLACK);
      M5.Lcd.setCursor(40, 75);
      M5.Lcd.printf(country.c_str());
    }

    delay(5);

    return true;
  }

  void Reset() override {
    return;
  }
  bool End() override {
    return true;
  }
};

#endif  // RTCAPP_HPP
