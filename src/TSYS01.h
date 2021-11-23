#ifndef TSYS01_H_BLUEROBOTICS
#define TSYS01_H_BLUEROBOTICS

#include "Arduino.h"

class TSYS01 {
public:

	TSYS01();

	void init();

	/** The read from I2C takes up for 40 ms, so use sparingly is possible.
	 */
	void read();

	/** This function loads the datasheet test case values to verify that
	 *  calculations are working correctly. No example checksum is provided
	 *  so the checksum test may fail.
	 */
	void readTestCase();

	/** Temperature returned in deg C.
	 */
	float temperature();

private:
	uint16_t C[8];
	uint32_t DD1;
	float TEMP;
	uint32_t adc;

	/** Performs calculations per the sensor data sheet for conversion and
	 *  second order compensation.
	 */
	void calculate();

};

#endif
