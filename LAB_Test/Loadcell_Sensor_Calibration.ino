#include "HX711.h"
#define calibration_factor 2400000
#define DOUT 23
#define CLK 22
HX711 scale(DOUT, CLK);
void setup() {
  Serial.begin(9600);
  Serial.println("ArduinoAll Calibrating...");
  scale.set_scale(calibration_factor);
  scale.tare();  //รีเซตน้ำหนักเป็น 0
  Serial.println("OK Start :");
}
void loop() {

  double mg = 0.00;
  double g = 0.00;
  Serial.print("Reading: ");
  double kg = -1 * scale.get_units();
  if (kg <= 0.01) {
    mg = (kg * (1000000));
    Serial.print(mg);
    Serial.println(" mg");
  } else if (kg <= 1) {
    g = (kg * (1000));
    Serial.print(g);
    Serial.println(" g");
  } else {
    Serial.print(kg);
    Serial.println(" kg");
  }
  delay(200);
}
