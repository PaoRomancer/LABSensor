/*
  ACS712 Current Sensor Calibration & Measurement
  เวอร์ชันเพิ่ม Linear & Polynomial Calibration
  ใช้ได้กับ ACS712 รุ่น 5A / 20A / 30A
*/

#if defined(ESP32)
  #define CURRENT_PIN 35
  const float ADC_REF = 3.3;    // ESP32 ใช้ 3.3V
  const int ADC_MAX = 4095;     // 12-bit ADC
#else
  #define CURRENT_PIN A0
  const float ADC_REF = 5.0;    // Arduino UNO ใช้ 5V
  const int ADC_MAX = 1023;     // 10-bit ADC
#endif

// -------------------- ค่าเซนเซอร์ --------------------
const float SENSITIVITY = 0.185;   // รุ่น 5A = 0.185V/A, 20A=0.100, 30A=0.066
float zeroOffset = 0;              // ค่าศูนย์ (หาโดยพิมพ์ "cal")

// -------------------- ค่าคาลิเบรต --------------------
// ⚙️ Linear Calibration: y = αx + β
float alpha = 1.2237;   // slope (m)
float beta  = -0.0218;   // intercept (c)

// ⚙️ Polynomial Calibration: y = a*x² + b*x + c
float a_poly = 0.109;
float b_poly =  1.1275;
float c_poly =  -0.0065;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== ACS712 Calibration ===");
  Serial.println("1️⃣ ถอดโหลดออก → พิมพ์ 'cal' เพื่อคาลิเบรตศูนย์");
  Serial.println("2️⃣ พิมพ์ 'run' เพื่อเริ่มวัดกระแส (พร้อม Linear & Polynomial)\n");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "cal") {
      zeroOffset = calibrateZero();
      Serial.print("✅ Zero Offset (V) = ");
      Serial.println(zeroOffset, 4);
      Serial.println("พิมพ์ 'run' เพื่อเริ่มวัดกระแส\n");
    }

    else if (cmd == "run") {
      Serial.println("🔌 เริ่มวัดกระแสจริง (Ctrl+C เพื่อหยุด)");
      while (true) {
        float current = readCurrent();                     // ค่ากระแสดิบ
        float linear_cal = (alpha * current) + beta;       // Linear calibration
        float poly_cal   = (a_poly * current * current) + (b_poly * current) + c_poly; // Polynomial calibration

        Serial.printf("Raw Current: %.3f A | Linear: %.3f A | Poly: %.3f A\n",
                      current, linear_cal, poly_cal);
        delay(700);
      }
    }
  }
}

// -------------------- คาลิเบรตศูนย์ --------------------
float calibrateZero() {
  Serial.println("🌀 กำลังคาลิเบรต... (เฉลี่ย 1000 ครั้ง)");
  long sum = 0;
  for (int i = 0; i < 1000; i++) {
    sum += analogRead(CURRENT_PIN);
    delay(2);
  }
  float avg = sum / 1000.0;
  float Vout = (avg / ADC_MAX) * ADC_REF;
  return Vout;
}

// -------------------- อ่านกระแสดิบ --------------------
float readCurrent() {
  const int N = 50;
  long sum = 0;
  for (int i = 0; i < N; i++) {
    sum += analogRead(CURRENT_PIN);
    delay(2);
  }
  float avg = sum / (float)N;
  float Vout = (avg / ADC_MAX) * ADC_REF;
  float I = (Vout - zeroOffset) / SENSITIVITY;
  return I;
}
