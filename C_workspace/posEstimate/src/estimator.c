/*
 * estimator.c

 *
 *  Created on: 12/06/2014
 *      Author: Soren
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "estimator.h"



//Initiates the estimator, mallocs stuff
estimator_t* estimatorInit( double initState[3], double calibOffset[6], double calibScale[6], double g, double AHRS_beta, double AHRS_fs ) {
	estimator_t* estimator = (estimator_t*) malloc( sizeof( estimator_t ) );
	int i = 0;
	for ( i = 0; i<6; i++ ) {
		if( i<3 )
			estimator->stateEstimate[i] = initState[3];
		estimator->calibOffset[i] = calibOffset[i];
		estimator->calibScale[i] = calibScale[i];
	}
	estimator->g = g;

	double q[4] = {1, 0, 0, 0};
	MadgwickAHRSinit( &(estimator->AHRS), q, AHRS_beta, AHRS_fs );

	return estimator;
}

//Predict states from 'n' inputs.
void estimatorPredict( estimator_t* estimator, double acc_gyro[][6], uint32_t n ) {
	//for each sample
	int i;
	double acc[3];
	for ( i = 0; i < n; i++ ) {
		//Apply calibration
		int j;
		double acc_gyro_Calib[6];
		for ( j = 0; j < 6; j++ ) {
			acc_gyro_Calib[j] = acc_gyro[i][j] + estimator->calibOffset[j];
			acc_gyro_Calib[j] *= estimator->calibScale[j];
		}
		//Perform Madgwick
		MadgwickAHRSupdate( &(estimator->AHRS), acc_gyro_Calib[3], acc_gyro_Calib[4], acc_gyro_Calib[5], acc_gyro_Calib[0], acc_gyro_Calib[1], acc_gyro_Calib[2], 1, 0, 0);
		//Rotate acceleration
		quaternionVectorRotate( acc, acc_gyro_Calib, estimator->AHRS.q );
		//subtract gravity
		acc[2] -= estimator->g;
		//extract z-axis
		//predict Kalman-filter
	} //end for
	//move Kalman output to stateEstimate
	estimator->stateEstimate[2] = acc[2] - 0;
	//calculate z-acceleration without offset.
}

//Update the estimator from a measurement.
void estimatorUpdate( estimator_t* estimator, double pos ) {
	//predict Kalman-filter
	//move Kalman output to stateEstimate
	//calculate z-acceleration without offset.
}

//Free malloced data
void estimatorFree( estimator_t* estimator ) {
	free( estimator );
}
