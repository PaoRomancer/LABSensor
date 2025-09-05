int adcPin = 34; // ขา ADC ของ ESP32
float adcValue = 0;
float voltage = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  adcValue = analogRead(adcPin);

  // ADC 12 บิต: 0–4095, Reference = 3.3V
  float Vadc = (adcValue * 3.3) / 4095.0;

  // --- ถ้าใช้โมดูลอย่างเดียว (≤16.5V) ---
  float Vin = Vadc * (30.0 + 7.5) / 7.5; // คูณ factor 5

  // --- ถ้าใช้ Divider เพิ่ม (≤25V) ---
  // float Vin = Vadc * ((R3+R4)/R4) * ((30.0+7.5)/7.5);

  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" , Vin: ");
  Serial.print(Vin, 2);
  Serial.println(" V");

  delay(500);
}
