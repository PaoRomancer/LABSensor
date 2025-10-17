#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 22 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
void setup() {
  Serial.begin(115200);
  while (!Serial);
  sensors.begin();
  
  Serial.println("--- DS18B20 Temperature Sensor Initialized ---");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" device(s).");
}

void loop() {
 
  sensors.requestTemperatures(); 

  float tempC = sensors.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: DS18B20 disconnected or reading failed!");
  } else {

    Serial.print("Temperature: ");
    Serial.print(tempC, 2); 
    Serial.println(" C");

  }


  delay(2000);
}
