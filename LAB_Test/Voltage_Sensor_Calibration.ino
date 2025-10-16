// =================================================================
// Multi-Point vs. Raw Voltage Calculation Comparison
// ESP32 + Voltage Sensor Module (0-24V)
// =================================================================

// --- ค่าความต้านทานที่วัดได้จริง ---
const float R1 = 30000.0; // Resistor 1 (ตัวที่ต่อกับ V_in)
const float R2 = 7500.0;  // Resistor 2 (ตัวที่ต่อกับ GND)

// --- Calibration Lookup Table ---
const int NUM_POINTS = 13; 
const int adcValues[NUM_POINTS] = {0,      115, 368 , 604, 1095, 1600, 2086, 2322, 2575, 3091, 3405, 3774, 4095}; 
const float voltageValues[NUM_POINTS] = {0,1.00,2.00, 3.00, 5.00, 7.00, 9.00, 10.00,11.00 ,13.00, 14.00, 15.00, 16.50};

const int VOLTAGE_SENSOR_PIN = 34;
const float VREF = 3.3; // แรงดันอ้างอิงของ ESP32 ADC

void setup() {
  Serial.begin(115200);
  
  // แสดงข้อมูล Calibration Table
  Serial.println("\n=== Calibration Points ===");
  for (int i = 0; i < NUM_POINTS; i++) {
    Serial.print("Point ");
    Serial.print(i + 1);
    Serial.print(": ADC = ");
    Serial.print(adcValues[i]);
    Serial.print(" -> Voltage = ");
    Serial.print(voltageValues[i], 2);
    Serial.println(" V");
  }
  Serial.println("==========================\n");
}

void loop() {
  // 1. อ่านค่า ADC ดิบ
  int adcValue = analogRead(VOLTAGE_SENSOR_PIN);

  // 2. คำนวณ Raw Voltage (จากสูตร Voltage Divider)
  float pinVoltage = (adcValue / 4095.0) * VREF;
  float rawVoltage = pinVoltage * (R1 + R2) / R2;

  // 3. คำนวณ Calibrated Voltage (จาก Lookup Table)
  float calibratedVoltage = getCalibratedVoltage(adcValue);

  // --- แสดงผลเปรียบเทียบ ---
  Serial.print("Raw ADC: ");
  Serial.print(adcValue);

  Serial.print("\t | Raw Voltage (Calc): ");
  Serial.print(rawVoltage, 3); // << ค่าที่คำนวณจาก R (มีความคลาดเคลื่อน)

  Serial.print("\t | Calibrated Voltage: ");
  Serial.print(calibratedVoltage, 3); // << ค่าที่แม่นยำที่สุด

  Serial.println(" V");

  delay(1000);
}

// ฟังก์ชัน map สำหรับค่าทศนิยม (float)
float map_Float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// ฟังก์ชัน Calibration แบบ Piecewise Linear Interpolation
float getCalibratedVoltage(int currentAdc) {
  if (currentAdc <= adcValues[0]) return voltageValues[0];
  if (currentAdc >= adcValues[NUM_POINTS - 1]) return voltageValues[NUM_POINTS - 1];

  for (int i = 0; i < NUM_POINTS - 1; i++) {
    if (currentAdc >= adcValues[i] && currentAdc <= adcValues[i + 1]) {
      return map_Float(currentAdc, 
                       adcValues[i], adcValues[i + 1], 
                       voltageValues[i], voltageValues[i + 1]);
    }
  }
  
  return -1.0; 
}
