#include <Arduino.h>
#include <Catena-HS300x.h>
#include <SparkFunCCS811.h>
#include <SPI.h>
#include <Wire.h>
#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include <FXOS8700CQ.h>
#include <PCT2075.h>




CCS811 ccs811(0x5A);
FXOS8700CQ fx0s8700cq = FXOS8700CQ(0x1D);
PCT2075 pct2075 = PCT2075(0x48);
using namespace McciCatenaHs300x;
cHS300x gHs300x{Wire};

void setup()
{
  // Serial and Wire initialization
  Serial.begin(115200);

  while (! Serial)
        yield();

  Wire.begin(17, 16); 

  // CCS811 sensor initialization
  if (!ccs811.begin())
  {
    Serial.println("Failed to initialize CCS811 sensor. Freezing.");
    while (1)
      ;
  }

  // HS300x sensor initialization
  if (!gHs300x.begin())
  {
    Serial.println("Failed to initialize HS300x sensor. Freezing.");
    while (1)
      ;
  }

  // HTS221 sensor initialization
  if (!HTS.begin()) {
    Serial.println("Failed to initialize HTS221 sensor. Freezing.");
    while (1);
  }

  // LPS22HB sensor initialization
  if (!BARO.begin()) {
    Serial.println("Failed to initialize LPS22HB sensor. Freezing.");
    while (1);
  }

  // FXOS8700CQ sensor initialization
  fx0s8700cq.init();

  // PCT2075 sensor initialization
  pct2075.begin();
}

void loop()
{
  // CCS811 sensor
  Serial.print("CCS811 sensor | ");

  if (ccs811.dataAvailable())
  {
    ccs811.readAlgorithmResults();
    Serial.print("CO2 = ");
    Serial.print(ccs811.getCO2());
    Serial.print(" - tVOC = ");
    Serial.println(ccs811.getTVOC());
  }

  // HS300x sensor
  Serial.print("HS300x sensor | ");

  cHS300x::Measurements m;
  float t, rh;
  if (gHs300x.getTemperatureHumidity(m))
  {
    m.extract(t, rh);
    Serial.print("Temperature = ");
    Serial.print(t);
    Serial.print("°C - Humidity = ");
    Serial.print(rh);
    Serial.println("%");
  }

  // HTS221 sensor
  Serial.print("HTS221 sensor | ");

  float temperature = HTS.readTemperature();
  float humidity = HTS.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C - Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // LPS22HB sensor
  Serial.print("LPS22HB sensor | ");

  float pressure = BARO.readPressure();
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print("kPa ");

  float temperature2 = BARO.readTemperature();
  Serial.print(" - Temperature: ");
  Serial.print(temperature2);
  Serial.print("°C");

  float altitude = BARO.readAltitude();
  Serial.print(" - Altitude: ");
  Serial.print(altitude);
  Serial.println("m");

  // FXOS8700CQ sensor
  Serial.print("FXOS8700CQ sensor | ");
  fx0s8700cq.readAccelData();
  fx0s8700cq.readMagData();

  Serial.print("Accelerometer: ");
  Serial.print((int)fx0s8700cq.accelData.x);
  Serial.print(", ");
  Serial.print((int)fx0s8700cq.accelData.y);
  Serial.print(", ");
  Serial.print((int)fx0s8700cq.accelData.z);
  
  Serial.print(" - Magnetometer: ");
  Serial.print((int)fx0s8700cq.magData.x);
  Serial.print(", ");
  Serial.print((int)fx0s8700cq.magData.y);
  Serial.print(", ");
  Serial.println((int)fx0s8700cq.magData.z);

  // PCT2075 sensor
  Serial.print("PCT2075 sensor | ");

  if (pct2075.isShutdown() == 0) {
		float tempC = pct2075.getTempC();
		Serial.print("Temperature: ");
		Serial.print(tempC);
		Serial.print("°C");
	}
	else {
		pct2075.normal(); 
	}


  Serial.println();

  delay(10000); 
}

