#include <Arduino.h>
#include <Catena-HS300x.h>
#include <SparkFunCCS811.h>
#include <SPI.h>
#include <Wire.h>
#include <Arduino_HTS221.h>




#define CCS811_ADDR 0x5A // Default I2C Address

CCS811 ccs811(CCS811_ADDR);


using namespace McciCatenaHs300x;
cHS300x gHs300x{Wire};

void setup()
{
  Serial.begin(115200);

  while (! Serial)
        yield();

  Serial.println("CCS811 Basic Example");

  Wire.begin(17, 16); // Inialize I2C Hardware

  if (!ccs811.begin())
  {
    Serial.print("CCS811 failed. Freezing.");
    while (1)
      ;
  }

  if (!gHs300x.begin())
  {
    Serial.println("gHs300x failed. Freezing.");
    while (1)
      ;
  }

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
}

void loop()
{
  float co2, tvoc;
  
  if (ccs811.dataAvailable())
  {

    ccs811.readAlgorithmResults();

    Serial.print("CO2 = ");
    Serial.print(ccs811.getCO2());
    Serial.print(" tVOC = ");
    Serial.println(ccs811.getTVOC());
  }

  cHS300x::Measurements m;
  float t, rh;

  if (gHs300x.getTemperatureHumidity(m))
  {
    m.extract(t, rh);
    Serial.print("T= ");
    Serial.print(t);
    Serial.print("C  RH = ");
    Serial.print(rh);
    Serial.println("%");
  }

  float temperature = HTS.readTemperature();
  float humidity    = HTS.readHumidity();

  // print each of the sensor values
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(10000); 
}

