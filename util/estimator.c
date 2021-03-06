/*
 * estimator.c

 *
 *  Created on: 12/06/2014
 *      Author: Soren
 */

#include <stdlib.h>
#include <math.h>
#include "estimator.h"
#include "kalman.h"


//Initiates the estimator, mallocs stuff
estimator_t* estimatorInit( double initState[3], double calibOffset[6], double calibScale[6], double g, double speedOfSound, double distanceOffset, double AHRS_beta, double AHRS_fs ) {
	estimator_t* estimator = (estimator_t*) malloc( sizeof( estimator_t ) );
	int i = 0;
	for ( i = 0; i<6; i++ ) {
		if( i<3 )
			estimator->stateEstimate[i] = initState[3];
		estimator->calibOffset[i] = calibOffset[i];
		estimator->calibScale[i] = calibScale[i];
	}
	estimator->g = g;
	estimator->speedOfSound = speedOfSound;
	estimator->distanceOffset = distanceOffset;

	//Initiate AHRS
	double q[4] = {1, 0, 0, 0};
	MadgwickAHRSinit( &(estimator->AHRS), q, AHRS_beta, AHRS_fs );

	//Initiate Kalman filter
	initKalman( &(estimator->kalmanFilter) );

	//Initiate ultrasonic filter
	estimator->noiseFilterLength = 15;
	estimator->noiseFilterTolerance = 0.10;
	estimator->noiseFilter = calloc( estimator->noiseFilterLength, sizeof( double ) );

	return estimator;
}

//Predict states from 'n' inputs.
void estimatorPredict( estimator_t* estimator, double acc_gyro[][6], uint32_t n ) {
	//for each sample
	int i;
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
		quaternionVectorRotate( estimator->accPlusOffset, acc_gyro_Calib, estimator->AHRS.q );
		//subtract gravity
		estimator->accPlusOffset[2] -= estimator->g;
		//extract z-axis
		//predict Kalman-filter
		predictKalman( &(estimator->kalmanFilter), estimator->accPlusOffset[2] );
	} //end for
	//move Kalman output to stateEstimate
	estimator->stateEstimate[0] = Matrix_get( estimator->kalmanFilter.x, 2, 0 );
	estimator->stateEstimate[1] = Matrix_get( estimator->kalmanFilter.x, 1, 0 );
	estimator->stateEstimate[2] = estimator->accPlusOffset[2] - Matrix_get( estimator->kalmanFilter.x, 0, 0 );
	//calculate z-acceleration without offset.
}

void estimatorPredictUpdate( estimator_t* estimator, double acc_gyro[][6], uint16_t distanceTime[], uint32_t n ) {
	//Reset acceleration average
	estimator->stateEstimate[2] = 0;
	//Reset speed average
	estimator->stateEstimate[1] = 0;

	printf( "n: %d\n", n );
	//for each sample
	int i;
	for ( i = 0; i < n; i++ ) {
		//Apply calibration
		int j;
		double acc_gyro_Calib[6];
		double distance;
		for ( j = 0; j < 6; j++ ) {
			acc_gyro_Calib[j] = acc_gyro[i][j] + estimator->calibOffset[j];
			acc_gyro_Calib[j] *= estimator->calibScale[j];
			distance = distanceTime[i] * estimator->speedOfSound / 2.0 + estimator->distanceOffset;
		}
		//Perform Madgwick
		MadgwickAHRSupdate( &(estimator->AHRS), acc_gyro_Calib[3], acc_gyro_Calib[4], acc_gyro_Calib[5], acc_gyro_Calib[0], acc_gyro_Calib[1], acc_gyro_Calib[2], 1, 0, 0);
		//Rotate acceleration
		quaternionVectorRotate( estimator->accPlusOffset, acc_gyro_Calib, estimator->AHRS.q );
		//subtract gravity
		estimator->accPlusOffset[2] -= estimator->g;
		//extract z-axis
		//predict Kalman-filter
		predictKalman( &(estimator->kalmanFilter), estimator->accPlusOffset[2] );
		if ( distanceTime[i] ) {
		//update noise filter
			for ( j = estimator->noiseFilterLength-1; j > 0; j-- )
				estimator->noiseFilter[j] = estimator->noiseFilter[j-1];
			estimator->noiseFilter[0] = distance;
			// find mean
			double mean = 0;
			for ( j = 1; j < estimator->noiseFilterLength; j++ )
				mean += estimator->noiseFilter[j];
			mean /= estimator->noiseFilterLength - 1;

			if ( fabs( mean - distance ) < estimator->noiseFilterTolerance ) {
				//update Kalman-filter, when we have a valid point
				updateKalman( &(estimator->kalmanFilter), distance );
			}

		}
		// Add to acceleration average
		estimator->stateEstimate[2] += estimator->accPlusOffset[2] - Matrix_get( estimator->kalmanFilter.x, 0, 0 );
		// Add to speed average
		estimator->stateEstimate[1] += Matrix_get( estimator->kalmanFilter.x, 1, 0 );
	} //end for
	//move Kalman output to stateEstimate
	estimator->stateEstimate[0] = Matrix_get( estimator->kalmanFilter.x, 2, 0 );

	if ( n ) {
		estimator->stateEstimate[1] /= n;
		estimator->stateEstimate[2] /= n;
	}
	//calculate z-acceleration without offset.
}

//Update the estimator from a measurement.
void estimatorUpdate( estimator_t* estimator, double pos ) {
	//update Kalman-filter
	updateKalman( &(estimator->kalmanFilter), pos );
	//move Kalman output to stateEstimate
	estimator->stateEstimate[0] = Matrix_get( estimator->kalmanFilter.x, 2, 0 );
	estimator->stateEstimate[1] = Matrix_get( estimator->kalmanFilter.x, 1, 0 );
	estimator->stateEstimate[2] = estimator->stateEstimate[2] - Matrix_get( estimator->kalmanFilter.x, 0, 0 );
	//calculate z-acceleration without offset.
}

//Free malloced data
void estimatorFree( estimator_t* estimator ) {
	free( estimator->noiseFilter );
	destroyKalman( &(estimator->kalmanFilter) );
	free( estimator );
}



