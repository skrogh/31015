#include <stdlib.h>
#include <stdio.h>
#include "kalman.h"

void readPos( kalmanFilter_t * filter ) {
	double pos = Matrix_get( filter->x, 0, 2 );
	printf( "%f\n", pos );	
}

int main() {
	kalmanFilter_t filter;
	initKalman( &filter );


	for ( int i = 0; i < 10; i++ ) {
		predictKalman( &filter, 2 );
		updateKalman( &filter, 5 );
		readPos( &filter );
	}

	destroyKalman( &filter );

	return 0;
}
