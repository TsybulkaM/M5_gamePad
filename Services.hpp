#ifndef SERVICES_HPP
#define SERVICES_HPP

class Battery {
private:
  int prev_batteryPercentage;
  int batteryPercentage;
  float MIN_VOLTAGE;
  float MAX_VOLTAGE;
public:
  Battery() : MIN_VOLTAGE(3.0), MAX_VOLTAGE(4.2) {}

  void updateAndDisplay(uint8_t x, uint8_t y, uint8_t front_size) {
    float batteryVoltage = M5.Axp.GetBatVoltage();

    float normalizedVoltage = (batteryVoltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE);
    if (normalizedVoltage > 1.0) normalizedVoltage = 1.0;
    if (normalizedVoltage < 0.0) normalizedVoltage = 0.0;

    // Пример использования экспоненциального коэффициента
    batteryPercentage = pow(normalizedVoltage, 0.75) * 100;

    if (batteryPercentage > 100) batteryPercentage = 100;
    if (batteryPercentage < 0) batteryPercentage = 0;

    if (prev_batteryPercentage != batteryPercentage) {
      prev_batteryPercentage = batteryPercentage;
      M5.Lcd.setCursor(x, y);
      M5.Lcd.setTextSize(front_size);
      M5.Lcd.fillRect(x, y, 50*front_size,  10*front_size, BLACK);
      M5.Lcd.printf("%d%%", batteryPercentage);
    }
  }

  void display(uint8_t x, uint8_t y, uint8_t front_size) {
      M5.Lcd.setCursor(x, y);
      M5.Lcd.setTextSize(front_size);
      M5.Lcd.fillRect(x, y, 50*front_size,  10*front_size, BLACK);
      M5.Lcd.printf("%d%%", batteryPercentage);
  }
};

#endif // SERVICES_HPP