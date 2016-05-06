#pragma once

#include "ekfutil.h"

// Indirect (a.k.a. error-state) EKF.
//
// Rather than tracking the navigation states directly, this filter contains a "dumb"
// dead-reckoning IMU integrator and uses EKF to estimate the error of this integrator.
// When a measurement arrives, the filter uses it to correct the integrator state
// and updates the error estimate correspondingly.
//
// The advantage of this formulation is that it permits us to track the orientation error
// with RPY angles, which eliminates the problem of the quaternion having 4 dimensions
// but only 3 degrees of freedom. Also, it keeps the EKF quantities close to 0,
// which is a good thing [citation needed].

#define EKF_N 15
#define EKF_M 6

struct ekf
{
	// integrator state
	struct vec pos;
	struct vec vel;
	struct quat quat;
	//struct vec bias_gyro;
	//struct vec bias_acc;

	// TODO symmetric matrix storage optimization?
	float P[EKF_N][EKF_N]; // error state covariance

	//float temp[EKF_N][EKF_N]; // temporary storage for matrix calcs
	//float temp2[EKF_N][EKF_N];
};

void ekf_imu(struct ekf const *ekf_prev, struct ekf *ekf, float const acc[3], float const gyro[3], float dt);

void ekf_vicon(struct ekf const *ekf_prev, struct ekf *ekf, float const pos_vicon[3], float const quat_vicon[4]);
