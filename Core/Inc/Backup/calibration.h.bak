 /*
 * calibration.h
 *
 *  Created on: Nov 21, 2024
 *      Author: siong
 */

#ifndef INC_CALIBRATION_H_
#define INC_CALIBRATION_H_


#include <stddef.h> // For size_t

// Structure to hold calibration parameters
typedef struct {
    float offset_x;
    float offset_y;
    float offset_z;
    float scale_x;
    float scale_y;
    float scale_z;
} MagCalibration;

// Function prototypes
void calibrate_magnetometer(const float *raw_x, const float *raw_y, const float *raw_z, size_t data_count, MagCalibration *calibration);
void apply_calibration(float *x, float *y, float *z, const MagCalibration *calibration);


#endif /* INC_CALIBRATION_H_ */
