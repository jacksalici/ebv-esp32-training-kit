# EBV ESP32Secure kit 

In 2020, the Avnet company EVB Elektronik released training kit specifically designed for adding a security (hardware) layer on top of the well known ESP32 DevKitC V4. This layer is indeed handled by the Microchip ATECC608A secure element, that offers an hardware-based secure storage. **Besides to that, the kit comes with 5 other PCBs, providing 11 sensors very useful for plenty of IoT applications.**

![EBV ESP32Secure kit](ESPSecure.jpg)

For unknown reasons, the kit disapperead from the Internet (the company blog posts on that have been deleted too), and now it is very hard to find any information about it. As of Jan 2025, the only reference to this is an old [EBV website page](https://www.iotconnect.io/ebv/), where the kit is still listed, but the links are broken. _The image above shows the kit, as it was presented there._

This repository aims to provide the library and the code for the kit, offering a starting point for those who want to use it. Even if I don't think it is possible to find it anymore, I hope this can be useful for someone or at least for my future reference.

## Sensors and PCBs

### Description

The kit is composed by 6 PCBs, each one hosting one or more sensors. The following list shows the sensors available on each PCB, which are reported from top to bottom as in the image above.

#### PCB *Microchip*

- [ ] **`ATECC608A`** – a secure element that provides hardware-based key storage and cryptographic operations to enhance the security of connected applications. [^1]
  
[^1]: Not implemented in this code but [this guide](https://www.instructables.com/Secure-Communication-Arduino/) should be a good starting point.

#### PCB *NXP Semiconductors*

- [x] **`FXOS8700CQ`** – a 6-axis sensor with an integrated linear accelerometer and magnetometer.
- [x] **`SE95DP`** – a high-accuracy digital temperature sensor and thermal watchdog. [^2]

[^2]: In the original blog post this was a `PCT2075DP` temperature sensor, unclear which sensor is actual on board. A library for `PCT2075DP` has been tested and works pretty well. 

#### PCB *Infineon*

- [x] **`DPS422`** – a digital XENSIV™ barometric pressure and temperature sensor for portable and IoT devices.
- [ ] **`TLV493D`** – a 3D magnetic sensor with low power consumption.[^3]

[^3]: I have not been able to get [the official library](https://github.com/Infineon/arduino-xensiv-3d-magnetic-sensor-tlx493d) working, so this sensor is not implemented in the code.

#### PCB *STMicroelectronics*

- [x] **`LPS22HB`** – an absolute digital output barometer with a pressure range from 260 to 1260 hPa, with also temperature and altitude indication.
- [x] **`HTS221`** – a capacitive digital sensor for relative humidity and temperature.
- [x] **`LSM303AGR`** – a high-performance eCompass module: an ultra-low-power 3D accelerometer and a 3D magnetometer.

#### PCB *Renesas*

- [x] **`HS3001`** – a high-performance relative humidity and temperature sensor.

#### PCB *ams*

- [x] **`CCS811`** – an ultra-low-power digital gas sensor for indoor air quality monitoring.
- [x] **`ENS210`** – a highly accurate relative humidity and temperature sensor.
- [x] **`TSL2572`** – a light-to-digital converter.

### Addresses

The sensors are addressable through the I2C bus. The following table shows the addresses of the sensors on the PCBs.

| Module Code  | Module Utility               | I2C Address|
|--------------|------------------------------|------------|
| LPS22HB      | Pressure sensor              | 0x5C       |
| LSM303-ACC   | Accelerometer                | 0x19       |
| LSM303-MAG   | Magnetometer                 | 0x1E       |
| HTS221       | Humidity & temperature       | 0x5F       |
| SE95DP       | Temperature sensor           | 0x48       |
| FXOS8700CQ   | 6-axis motion sensor         | 0x1D       |
| CCS811       | Air quality (TVOC, eCO2)     | 0x5A       |
| ENS210       | Humidity & temperature       | 0x43       |
| TSL2572      | Ambient light sensor         | 0x39       |
| DPS422       | Pressure & temperature       | 0x76       |
| TLV493D      | 3D magnetic sensor           | 0x5E       |
| ATECC608A    | Crypto authentication        | 0x60       |
| HS3001       | Humidity & temperature       | 0x44       |



## Software and libraries  

The project has been developed using the [PlatformIO](https://platformio.org/) IDE. The librieries in the `EBVTrainingKit/lib` folder are developed by the respective authors. Minor changes done in order to make them working have been reported below along with the link to the original repository.

- `FXOS8700CQ` – [link](https://github.com/mlwarner/fxos8700cq-arduino) 
- `PCT2075DP` – [link](https://github.com/jpliew/PCT2075) – GNU
- `DPS422` – [link](https://github.com/Infineon/DPS422-Library-Arduino) – MIT Licence 
    - Avaiable both continuous and single shot mode, here implemented only the latter.
- `LPS22HB` – [link](https://github.com/arduino-libraries/Arduino_LPS22HB) – LGPL-2.1 license 
- `HTS221` – [link](https://github.com/arduino-libraries/Arduino_HTS221) – LGPL-2.1 license
- `LSM303AGR` – [link](https://github.com/stm32duino/LSM303AGR) 
- `HS3001` – [link](https://github.com/mcci-catena/MCCI-Catena-HS300x) – MIT License
  - Adding the Arduino library [in the header file](https://github.com/jacksalici/ebv-esp32-training-kit/blob/56873641cd3b97781fdd2c818a971b706acd858c/EBVTrainingKit/lib/MCCI-Catena-HS300x/src/Catena-HS300x.h#L21) was needed in order to avoid a compilation error
- `CCS811` – [link](https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library) – MIT Licence
- `ENS210` – [link](https://github.com/maarten-pennings/ENS210) - MIT Licence
- `TSL2572` – [link](https://github.com/TinyCircuits/TinyCircuits-TSL2572-library) – LGPL-3.0 license
  - a [casting to float](https://github.com/jacksalici/ebv-esp32-training-kit/blob/7b57f895f41ebe9d049f6c2cd5e5ef58636d4c28/EBVTrainingKit/lib/TinyCircuits-TSL2572-library/src/TSL2572.cpp#L53) was needed in order to avoid a compilation error
