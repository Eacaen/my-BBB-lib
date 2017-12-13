#include <iostream>
#include <cstdio>
#include "math.h"
#include "BBB_IMU.h"
#include "BBB_MPU6050.h"
using namespace exploringBB;


EularAngle EA;
EularAngle EA_Origin; 
double cord[3];	

struct K_Filter K_F;
double q0 = 1, q1 = 0, q2 = 0, q3 = 0;   // quaternion elements representing the estimated orientation
double exInt = 0, eyInt = 0, ezInt = 0;  // scaled integral error

// #define halfT   GET_NOWTIME() * 0.5f   //采样间隔的一半
#define Kp 2.0f // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.005f// integral gain governs rate of convergence of gyroscope biases
#define hu_du 0.01744

double halfT;
float integralFBx,integralFBy,integralFBz;
float qa0, qa1, qa2, qa3;
float integralFBhand,handdiff;
float halftime ;
float  now; // TIM2 采样时间
float acc_vector = 0;//  加速度合力M/S^2

//Fast inverse square-root
/**************************????********************************************
函数原型：float invSqrt(float x) 
功能   ：快速算1/sqrt
*******************************************************************************/
float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

/**************************????********************************************
函数原型：void IMU_init(void)
功能   ：初始化个参数
*******************************************************************************/
void IMU_init(void)
{
// initialize quaternion
	q0 = 1.0f;    //初始化四元数
	q1 = 0.0f;
	q2 = 0.0f;
	q3 = 0.0f;
	
	qa0 = 1.0f;
	qa1 = 0.0f;
	qa2 = 0.0f;
	qa3 = 0.0f;
	
	exInt = 0.0;
	eyInt = 0.0;
	ezInt = 0.0;
	
	integralFBx = 0.0;
	integralFBy = 0.0; 
	integralFBz= 0.0;
	now = 0;
}

//计算四元数

void  MPUpDate(const MPU6050 & mpu)
{

	double norm;
	double vx, vy, vz;
	double ex, ey, ez;
	
	double ax, ay, az;
	double gx, gy, gz;
	
	ax=mpu.accelerationX;
	ay=mpu.accelerationY;
	az=mpu.accelerationZ;
	
	gx=mpu.GyroX * hu_du;
	gy=mpu.GyroY * hu_du;
	gz=mpu.GyroZ * hu_du;

// Normalize the measurements
	norm = invSqrt(ax*ax + ay*ay + az*az);
	if(norm == 0){ printf(" ERROR  ");}	
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;      
	
// estimated direction of gravity    q1=1
	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
// error is sum of cross product between reference direction of field and direction measured by sensor
	ex = (ay*vz - az*vy);
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);
// integral error scaled integral gain
	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;

// adjusted gyroscope measurements
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;

// integrate quaternion rate and normalize
//	now = GET_NOWTIME();
//	printf("%lf\r\n",now);

	halfT = 0.00025;
	
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

// Normalize quaternion
	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
}

// 四元数-->欧拉角

void EularAngle_calculate(EularAngle *ea)
{
	float t11,t12,t13;
	float t21,t22,t23;
	float t31,t32,t33;
	
	t11 = q0*q0+q1*q1-q2*q2-q3*q3;
	t12=2.0*(q1*q2+q0*q3);
	t13=2.0*(q1*q3-q0*q2);
//	t21=2.0*(q1*q2-q0*q3);
//	t22=q0*q0-q1*q1+q2*q2-q3*q3;
	t23=2.0*(q2*q3+q0*q1);
//	t31=2.0*(q1*q3+q0*q2);
//	t32=2.0*(q2*q3-q0*q1);
	t33=q0*q0-q1*q1-q2*q2+q3*q3;
 
	(*ea).Roll = atan2(t23,t33)*57.3;
	(*ea).Pitch = -asin(t13)*57.3;
	(*ea).Yaw = atan2(t12,t11)*57.3;
//	(*ea).Yaw = Correct_yaw();
	if ((*ea).Yaw < 0)
	{
		(*ea).Yaw += (360);
	}
}

	

void get_Eular_angle(const MPU6050 & mpu)
{
	MPUpDate( mpu );
	EularAngle_calculate(&EA);

//	EA.Roll  -= EA_Origin.Roll ;
//	EA.Pitch -= EA_Origin.Pitch;
//	EA.Yaw   -= EA_Origin.Yaw  ;

	cord[0] =  EA.Roll;
	cord[1] =  EA.Pitch;
//	cord[2] =  0 ;
 	cord[2] =  EA.Yaw ;

	printf("%.2lf  %.2lf  %.2lf\r\n",cord[0],cord[1],cord[2]);

//	printf("%f , %f , %f , %f , %f , %f \n" , mpu.accelerationX ,mpu.accelerationY,mpu.accelerationZ  , mpu.GyroX ,mpu.GyroY,mpu.GyroZ);
}

void suanfa_GetOrigin()
{
	static int  x=0;
	static double roll=0,pitch=0,yaw=0;

	for(x=0;x<100;x++)
	{
//		Data_Pare();
//	//	IMU_AHRSupdate(acc, gyro, Hmc_X,Hmc_Y,Hmc_Z);
//		MPUpDate( acc, gyro);
		EularAngle_calculate(&EA);
		
		roll  = roll  + EA.Roll;
		pitch = pitch + EA.Pitch;
		yaw   = yaw   + EA.Yaw;

		printf("hehe--- %.2lf  %.2lf  %.2lf\r\n",EA.Roll,EA.Pitch,EA.Yaw);	
	}
	
	EA_Origin.Roll  = roll  / 100.0;
	EA_Origin.Pitch = pitch / 100.0;
	EA_Origin.Yaw   = yaw   / 100.0;
	roll=0,pitch=0,yaw=0;
}

 



