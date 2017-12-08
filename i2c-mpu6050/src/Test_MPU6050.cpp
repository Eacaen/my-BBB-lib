/*
 */
#include <iostream>
#include <cstdio>
#include "BBB_MPU6050.h"
#include <unistd.h>
#include <pthread.h>

using namespace std;
using namespace exploringBB;

int main()
{
	int i;
	MPU6050 sensor(1,0x68);

	sensor.Sensor_WeakUp();
	sensor.readSensorState();

	printf("hello mpu6050\n");

	printf("test id : 0x%02x\n" , sensor.Get_ID());

	sensor.Zero_AG();

	for(i=0;i<99;i++)
	{
		sensor.readSensorDate();
		printf("%f , %f , %f , %f , %f , %f \n" , sensor.accelerationX ,sensor.accelerationY,sensor.accelerationZ  , sensor.GyroX ,sensor.GyroY,sensor.GyroZ);
	}
	return 0;
}
