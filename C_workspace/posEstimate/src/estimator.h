/*
 * estimator.h
 *
 *  Created on: 12/06/2014
 *      Author: Soren
 */

#ifndef ESTIMATOR_H_
#define ESTIMATOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "MadgwickAHRS.h"
#include "quaternion.h"


typedef struct {
	AHRS_t AHRS;
	double stateEstimate[3];
	double calibOffset[6]; // acc + offset  =  true acc, gyro + offset = true gyro
	double calibScale[6]; // [acc * scale] = m/s^2, [gyro * scale] = rad/s
	double g;

	//kalman...
} estimator_t;


//Initiates the estimator, mallocs stuff
estimator_t* estimatorInit( double initState[3], double calibOffset[6], double calibScale[6], double g, double AHRS_beta, double AHRS_fs );
//Predict states from 'n' inputs.
void estimatorPredict( estimator_t* estimator, double acc_gyro[][6], uint32_t n );
//Update the estimator from a measurement.
void estimatorUpdate( estimator_t* estimator, double pos );
//Free malloced data
void estimatorFree( estimator_t* estimator );





#endif /* ESTIMATOR_H_ */