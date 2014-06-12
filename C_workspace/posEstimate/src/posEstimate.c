/*
 ============================================================================
 Name        : posEstimate.c
 Author      : Soeren Andersen
 Version     :
 Copyright   : 2014
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "estimator.h"
#include "quaternion.h"



#define Fs_ACC 1000.0
#define AHRS_BETA 0.1


int main(void) {
	FILE * fp;
	FILE * out;
	fp = fopen ("src/analysis5.csv", "r");
	out = fopen ("C:/Users/Soren/Documents/MATLAB/fagprojekt/Databeh/out.csv", "w");


	double q1[4] = { 0.7071, 0, 0, -0.7071 };
	double v1[3] = {1, 0, 0};
	double v2[3];
	quaternionVectorRotate( v2, v1, q1 );


	double initState[3] = { 0 };
	double calibOffset[6] = { -32.6854, 6.9044, 44.0265, 93.6858, -107.6490, 69.1131 };
	double calibScale[6] = { 0.004755001962, 0.004794788965, 0.004778196636, 0.000532632218, 0.000532632218, 0.000532632218 };
	estimator_t* estimator = estimatorInit( initState, calibOffset, calibScale, 9.82, 1, 1000.0 );


	double pos;
	double time;
	int motorOut;

	int lines = 0;
	//scan away time, pos and motor output
	while( 3 == fscanf( fp, "%lf, %lf, %d", &time, &pos, &motorOut ) ) {
		// for each line
		int n = 0;
		double acc_gyro[30][6];
		while( 6 == fscanf( fp, ", %lf, %lf, %lf, %lf, %lf, %lf", &acc_gyro[n][0], &acc_gyro[n][1],
				&acc_gyro[n][2], &acc_gyro[n][3], &acc_gyro[n][4], &acc_gyro[n][5] ) ) {
			n++;
		}
		fscanf( fp, "\n" );

		// process line

		int i;
		for ( i = 0; i<n; i++ ) {
			estimatorPredict( estimator, (&acc_gyro[i][0]) , 1 );
			fprintf( out, "%f, ", estimator->stateEstimate[2] );
			fprintf( out, "%f, %f, %f, %f\n", estimator->AHRS.q[0], estimator->AHRS.q[1], estimator->AHRS.q[2], estimator->AHRS.q[3] );
		}
		lines++;
	}
	printf( "%d", lines );

	fclose( fp );
	fclose( out );

	return EXIT_SUCCESS;
}

