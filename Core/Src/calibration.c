/*
 * calibration.c
 *
 *  Created on: Nov 21, 2024
 *      Author: siong
 */


#include "calibration.h"
#include <float.h> // For FLT_MAX, FLT_MIN

// Function to calibrate the magnetometer
void calibrate_magnetometer(const float *raw_x, const float *raw_y, const float *raw_z, size_t data_count, MagCalibration *calibration) {
    // Initialize min/max variables
    float min_x = FLT_MAX, max_x = -FLT_MAX;
    float min_y = FLT_MAX, max_y = -FLT_MAX;
    float min_z = FLT_MAX, max_z = -FLT_MAX;

    // Iterate through all data points
    for (size_t i = 0; i < data_count; ++i) {
        // Update min/max for X-axis
        if (raw_x[i] < min_x) min_x = raw_x[i];
        if (raw_x[i] > max_x) max_x = raw_x[i];

        // Update min/max for Y-axis
        if (raw_y[i] < min_y) min_y = raw_y[i];
        if (raw_y[i] > max_y) max_y = raw_y[i];

        // Update min/max for Z-axis
        if (raw_z[i] < min_z) min_z = raw_z[i];
        if (raw_z[i] > max_z) max_z = raw_z[i];
    }

    // Calculate offsets (hard iron distortion)
    calibration->offset_x = (min_x + max_x) / 2.0f;
    calibration->offset_y = (min_y + max_y) / 2.0f;
    calibration->offset_z = (min_z + max_z) / 2.0f;

    // Calculate scaling factors (soft iron distortion)
    float range_x = max_x - min_x;
    float range_y = max_y - min_y;
    float range_z = max_z - min_z;

    // Average range for scaling
    float range_avg = (range_x + range_y + range_z) / 3.0f;

    calibration->scale_x = range_avg / range_x;
    calibration->scale_y = range_avg / range_y;
    calibration->scale_z = range_avg / range_z;
}

// Function to apply calibration to a single data point
void apply_calibration(float *x, float *y, float *z, const MagCalibration *calibration) {
    *x = (*x - calibration->offset_x) * calibration->scale_x;
    *y = (*y - calibration->offset_y) * calibration->scale_y;
    *z = (*z - calibration->offset_z) * calibration->scale_z;
}
