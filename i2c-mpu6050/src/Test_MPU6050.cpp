#include <iostream>
#include <cstdio>
#include "BBB_MPU6050.h"
#include <unistd.h>
#include <pthread.h>
#include "BBB_IMU.h"

using namespace std;
using namespace exploringBB;

int main()
{
	int i,j;

	MPU6050 sensor(1,0x68);
	sensor.Sensor_WeakUp();
//	sensor.Zero_AG();

	sensor.readSensorDate();

	printf("hello mpu6050\n");

	printf("test id : 0x%02x\n" , sensor.Get_ID());

	IMU_init();

	for(i=0;i<999999;i++)
	{
		sensor.readSensorState();

//		printf("%f , %f , %f , %f , %f , %f \n" , sensor.accelerationX ,sensor.accelerationY,sensor.accelerationZ  , sensor.GyroX ,sensor.GyroY,sensor.GyroZ);

		get_Eular_angle(sensor);
	}


	return 0;
}
