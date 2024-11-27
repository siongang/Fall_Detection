/*
 * lsm9ds1_read_data_polling.h
 *
 *  Created on: Nov 17, 2024
 *      Author: siong
 */

#ifndef INC_LSM9DS1_READ_DATA_POLLING_H_
#define INC_LSM9DS1_READ_DATA_POLLING_H_

// Include necessary libraries or headers
#include <stdint.h>
#include <string.h>


// Function declaration
void lsm9ds1_read_data_polling_init(void);
void lsm9ds1_read(float accel[3], float gyro[3], float mag[3]);

#endif /* INC_LSM9DS1_READ_DATA_POLLING_H_ */
