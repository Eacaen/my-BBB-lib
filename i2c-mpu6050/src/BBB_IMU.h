#ifndef __BBB_IMU_H
#define __BBB_IMU_H
#include "BBB_MPU6050.h"
using namespace exploringBB;

struct K_Filter
{
	double Pitch_Avg,Yaw_Avg,Roll_Avg;
	double Pitch_Avg2,Yaw_Avg2,Roll_Avg2;
	double Pitch_Con,Yaw_Con,Roll_Con;

};

//********************************************************************************
typedef struct EularAngle
{
	double Pitch=0,Yaw=0,Roll=0;
} EularAngle;

void IMU_init();
void  MPUpDate(const MPU6050 & mpu);

void EularAngle_calculate(struct EularAngle *ea);
void get_Eular_angle(const MPU6050 & mpu);

void suanfa_GetOrigin(void);
#endif
