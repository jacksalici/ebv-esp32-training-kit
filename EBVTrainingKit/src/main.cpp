#include <Arduino.h>
#include <Catena-HS300x.h>
#include <SparkFunCCS811.h>
#include <SPI.h>
#include <Wire.h>
#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include <FXOS8700CQ.h>
#include <PCT2075.h>
#include <ens210.h>
#include <Dps422.h>
#include <LSM303AGR_ACC_Sensor.h>
#include <LSM303AGR_MAG_Sensor.h>



CCS811 ccs811(0x5A);
FXOS8700CQ fx0s8700cq = FXOS8700CQ(0x1D);
PCT2075 pct2075 = PCT2075(0x48);
using namespace McciCatenaHs300x;
cHS300x gHs300x{Wire};
ENS210 ens210;
Dps422 DSP422 = Dps422();
LSM303AGR_ACC_Sensor lsm303agr_acc(&Wire);
LSM303AGR_MAG_Sensor lsm303agr_mag(&Wire);

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

  // ENS210 sensor initialization
  ens210.begin();

  // DPS422 sensor initialization
  DSP422.begin(Wire, 0x76);

  // LSM303AGR sensor initialization
  lsm303agr_acc.begin();
  lsm303agr_acc.Enable();
  lsm303agr_acc.EnableTemperatureSensor();
  lsm303agr_mag.begin();
  lsm303agr_mag.Enable();

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
    Serial.print(" - T. Hyst: ");
    Serial.print(pct2075.getThyst());
    Serial.print("°C");
    Serial.println();
	}
	else {
		pct2075.normal(); 
	}

  // ENS210 sensor
  Serial.print("ENS210 sensor | ");

  int t_data, t_status, h_data, h_status;
  ens210.measure(&t_data, &t_status, &h_data, &h_status );

  Serial.print( ens210.toCelsius(t_data, 10)/10.0, 1 );
  Serial.print("°C, ");
  Serial.print( ens210.toPercentageH(h_data, 1) );
  Serial.println(" %RH");


  // DPS422 sensor
  Serial.print("DPS422 sensor | ");

  float temperature3;
  float pressure3;
  int16_t oversampling = 7;
  int16_t ret;

  ret = DSP422.measureTempOnce(temperature3, oversampling);

  if (ret == 0)
  {
    Serial.print("Temperature: ");
    Serial.print(temperature3);
    Serial.print(" °C - ");
  }

  ret = DSP422.measurePressureOnce(pressure3, oversampling);

  if (ret == 0)
  {
    Serial.print("Pressure: ");
    Serial.print(pressure3);
    Serial.println(" P");
  }

  // LSM303AGR sensor
  Serial.print("LSM303AGR sensor | ");

  int32_t accelerometer[3];
  lsm303agr_acc.GetAxes(accelerometer);

  float temperature4;
  lsm303agr_acc.GetTemperature(&temperature4);

  int32_t magnetometer[3];
  lsm303agr_mag.GetAxes(magnetometer);

  Serial.print("Acc[mg]: ");
  Serial.print(accelerometer[0]);
  Serial.print(" ");
  Serial.print(accelerometer[1]);
  Serial.print(" ");
  Serial.print(accelerometer[2]);
  Serial.print(" - Mag[mGauss]: ");
  Serial.print(magnetometer[0]);
  Serial.print(" ");
  Serial.print(magnetometer[1]);
  Serial.print(" ");
  Serial.print(magnetometer[2]);
  Serial.print(" - Temp[C]: ");
  Serial.println(temperature4, 2);
  


  Serial.println();

  delay(10000); 
}

