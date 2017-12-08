#include "BBB_MPU6050.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace exploringBB 
{

//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	MPU_CONFIG		0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG		0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H		0x3B
#define	ACCEL_XOUT_L		0x3C
#define	ACCEL_YOUT_H		0x3D
#define	ACCEL_YOUT_L		0x3E
#define	ACCEL_ZOUT_H		0x3F
#define	ACCEL_ZOUT_L		0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress		0xD0	//IIC写入时的地址字节数据，+1为读取

#define	MPU6050_ID		0x68

/**
 * Method to combine two 8-bit registers into a single short, which is 16-bits on the BBB. It shifts
 * the MSB 8-bits to the left and then ORs the result with the LSB.
 * @param msb an unsigned character that contains the most significant byte
 * @param lsb an unsigned character that contains the least significant byte
 */
short MPU6050::combineRegisters(unsigned char msb, unsigned char lsb)
{
	//shift the MSB left by 8 bits and OR with LSB
	return ((short)msb<<8)|(short)lsb;
}




/**
 * The constructor for the MPU6050 accelerometer object. It passes the bus number and the
 * device address (with is 0x53 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the MPU6050 device is on - typically 0 or 1
 * @param I2CAddress The address of the ADLX345 device (default 0x53, but can be altered)
 */
MPU6050::MPU6050(unsigned int I2CBus, unsigned int I2CAddress):I2CDevice(I2CBus, I2CAddress)
{	
	// this member initialisation list calls the parent constructor
	this->I2CAddress = I2CAddress;
	this->I2CBus = I2CBus;

	this->accelerationX = 0;
	this->accelerationY = 0;
	this->accelerationZ = 0;

	this->GyroX = 0;
	this->GyroY = 0;
	this->GyroZ = 0;

	this->registers = NULL;
	this->Sensor_WeakUp();
}

/**
 * Read the sensor state. This method checks that the device is being correctly
 * read by using the device ID of the MPU6050 sensor. It will read in the accelerometer registers
 * and pass them to the combineRegisters() method to be processed.
 * @return 0 if the registers are successfully read and -1 if the device ID is incorrect.
 */
int MPU6050::readSensorState(){
	this->registers = this->readRegisters(1, WHO_AM_I);
	if(*this->registers!=MPU6050_ID)
	{
		perror("MPU6050: Failure Condition - Sensor ID not Verified");
		return -1;
	}
	this->accelerationX = this->combineRegisters(this->readRegister(ACCEL_XOUT_H), this->readRegister(ACCEL_XOUT_L));
	this->accelerationY = this->combineRegisters(this->readRegister(ACCEL_YOUT_H), this->readRegister(ACCEL_YOUT_L));
	this->accelerationZ = this->combineRegisters(this->readRegister(ACCEL_ZOUT_H), this->readRegister(ACCEL_ZOUT_L));

	this->GyroX = this->combineRegisters(this->readRegister(GYRO_XOUT_H), this->readRegister(GYRO_XOUT_L));
	this->GyroY = this->combineRegisters(this->readRegister(GYRO_YOUT_H), this->readRegister(GYRO_YOUT_L));
	this->GyroZ = this->combineRegisters(this->readRegister(GYRO_ZOUT_H), this->readRegister(GYRO_ZOUT_L));

	return 0;
}

int MPU6050::readSensorDate(){
	this->registers = this->readRegisters(1, WHO_AM_I);
	if(*this->registers!=MPU6050_ID)
	{
		perror("MPU6050: Failure Condition - Sensor ID not Verified");
		return -1;
	}
	this->accelerationX = this->combineRegisters(this->readRegister(ACCEL_XOUT_H), this->readRegister(ACCEL_XOUT_L));
	this->accelerationY = this->combineRegisters(this->readRegister(ACCEL_YOUT_H), this->readRegister(ACCEL_YOUT_L));
	this->accelerationZ = this->combineRegisters(this->readRegister(ACCEL_ZOUT_H), this->readRegister(ACCEL_ZOUT_L));

	this->GyroX = this->combineRegisters(this->readRegister(GYRO_XOUT_H), this->readRegister(GYRO_XOUT_L));
	this->GyroY = this->combineRegisters(this->readRegister(GYRO_YOUT_H), this->readRegister(GYRO_YOUT_L));
	this->GyroZ = this->combineRegisters(this->readRegister(GYRO_ZOUT_H), this->readRegister(GYRO_ZOUT_L));

	this->accelerationX -=  this -> zero_accX;
	this->accelerationY -=  this -> zero_accY;
	this->accelerationZ -=  this -> zero_accZ;

	this->GyroX -=  this -> zero_GyroX;
	this->GyroY -=  this -> zero_GyroY;
	this->GyroZ -=  this -> zero_GyroZ;


	return 0;
}

/**
 * Get the MPU6050 ID
 */
unsigned char  MPU6050::Get_ID()
{
	unsigned char mpuid;
	mpuid = this->readRegister(WHO_AM_I);
	if(mpuid!=MPU6050_ID)
	{
		perror("MPU6050: Failure Condition - Sensor ID not Verified");
		return -1;
	}
	return mpuid;
}

/**
 * Set the MPU6050 power on
 */
int MPU6050::Sensor_WeakUp()
{
	this->writeRegister(PWR_MGMT_1, 0x00);	//解除休眠状态
	this->writeRegister(SMPLRT_DIV, 0x07);		//陀螺仪采样率，典型值：0x07(125Hz)
	this->writeRegister(MPU_CONFIG, 0x06);		//低通滤波频率，典型值：0x06(5Hz)
	this->writeRegister(GYRO_CONFIG, 0x18);	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	this->writeRegister(ACCEL_CONFIG, 0x00); 	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

	return 0;
}

void MPU6050::Zero_AG()
{
	double ax , ay , az;
	double gx , gy , gz;
	int i , num = 500;
	for(i=0; i < num; i++)
	{
		this->readSensorState();
		ax += this->accelerationX ;
		ay += this->accelerationY ;
		az += this->accelerationZ ;

		gx += this->GyroX ;
		gy += this->GyroY ;
		gz += this->GyroZ ;
	}
	this -> zero_accX = ax / num; 
	this -> zero_accY = ay / num; 
	this-> zero_accZ = az / num; 

	this -> zero_GyroX = gx / num; 
	this -> zero_GyroY = gy / num; 
	this -> zero_GyroZ = gz / num; 
}


MPU6050::~MPU6050() 
{

}

} /* namespace exploringBB */
