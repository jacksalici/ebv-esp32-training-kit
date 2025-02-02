#ifndef TSL2572_h
#define TSL2572_h

// Communication address with the sensor
#define TSL2572_I2CADDR     0x39

// Sets the gain
#define   GAIN_1X 0
#define   GAIN_8X 1
#define  GAIN_16X 2
#define GAIN_120X 3

//only use this with 1x and 8x gain settings
#define GAIN_DIVIDE_6 true

#include <inttypes.h>

class TSL2572 {
public:
	int gain_val;

	TSL2572();
	void registerWrite(uint8_t regAddr, uint8_t regData);
	void init(uint8_t gain);
	float readAmbientLight(void);
};

#endif