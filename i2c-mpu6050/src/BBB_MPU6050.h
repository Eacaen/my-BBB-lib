/*
 * MPU6050.h  Created on: 17 May 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone" 
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European 
 * Union Public License (the "EUPL") version 1.1 as published by the 
 * European Union. Any use of this Software, other than as authorized 
 * under this License is strictly prohibited (to the extent such use 
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and 
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef MPU6050_H_
#define MPU6050_H_
#include"I2CDevice.h"


namespace exploringBB {

/**
 * @class MPU6050
 * @brief Specific class for the MPU6050 Accelerometer that is a child of the I2CDevice class
 * Protected inheritance means that the public I2CDevice methods are not publicly accessible
 * by an object of the MPU6050 class.
 */
class MPU6050:protected I2CDevice
{
private:
	unsigned int I2CBus, I2CAddress;
	unsigned char *registers;

	double zero_accX=0, zero_accY=0, zero_accZ=0; // raw 2's complement values
	double zero_GyroX=0, zero_GyroY=0, zero_GyroZ=0;


public:
	double accelerationX, accelerationY, accelerationZ; // raw 2's complement values
	double GyroX, GyroY, GyroZ;

	void Zero_AG();
	short combineRegisters(unsigned char msb, unsigned char lsb);
	void calculatePitchAndRoll();

	MPU6050(unsigned int I2CBus, unsigned int I2CAddress=0x68);

	int readSensorState();
	int readSensorDate();

	int Sensor_WeakUp();
	
	unsigned char Get_ID();

	short getAccelerationX() { return accelerationX; }
	short getAccelerationY() { return accelerationY; }
	short getAccelerationZ() { return accelerationZ; }

	short getGyroX() { return GyroX; }
	short getGyroY() { return GyroY; }
	short getGyroZ() { return GyroZ; }

	~MPU6050();
};

} /* namespace exploringBB */

#endif /* MPU6050_H_ */
