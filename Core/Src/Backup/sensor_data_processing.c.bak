///*
// * sensor_data_processing.c
// *
// *  Created on: Nov 20, 2024
// *      Author: siong
// */
//
//
//#include <stdio.h>
//#include <math.h>
//
//
//float accel_mag_orientation[];
//
//void cross_product_3(float arr_1[3], float arr_2[3], float product[3]) {
////	float* product = (float*) malloc(3*sizeof(float)); // review what this means
//
//	   product[0] = A[1] * B[2] - A[2] * B[1];
//	   product[1] = A[2] * B[0] - A[0] * B[2];
//	   product[2] = A[0] * B[1] - A[1] * B[0];
//
//}
//
//float magnitude_3(float arr [3]) {
//	return sqrt(arr[0]*arr[0] + arr[1]*arr[1] + arr[2]*arr[2]);
//}
//
//void unit_vector_3(float arr[3], float result[3]) {
//	float mag = magnitude (arr);
//	if (mag != 0) {
//		result[0] = arr[0]/mag;
//		result[1] = arr[1]/mag;
//		result[2] = arr[2]/mag;
//	} else {
//		result[0] = result[1] = result[2] = 0;
//	}
//
//}
//
//// accel in m/s^2, mag in micro Gausse???
//// we need the unit vectors for both
//void orientation_a_m (float accel_reading[3], float mag_reading[3], float accel_mag_orientation[3]) {
////	float accel_unit[3] = unit_vector_3(accel_reading);
////	float mag_unit[3] = unit_vector_3(mag_reading);
//
//	// Down Direction
//	float D[3] = {-accel_unit[0], -accel_unit[1], -accel_unit[2]};
//	// Unit_vector_3(D,D); Dont need to make D a unit vector as accel[] is already a unit vector
//
//	// East Direction
//	float E[3];
//	cross_product_3(D, mag_unit, E); // East = D x Mag
//	unit_vector_3(E,E);
//
//	float N[3];
//	cross_product_3(E,D,N);
//	unit_vector_3(N,N);
//
////	accel_mag_orientation[0] =
//
//}
//
//
//
//void orientation_g (float gyro_reading[3], float prev_gyro_orientation[3], float delta_t, float gyro_orientation[3]) {
//
//	gyro_orientation[0] = prev_gyro_orientation[0] + gyro_reading[0] * delta_t;
//	gyro_orientation[1] = prev_gyro_orientation[1] + gyro_reading[1] * delta_t;
//	gyro_orientation[2] = prev_gyro_orientation[2] + gyro_reading[2] * delta_t;
//
//
//
//
//
//}
//
