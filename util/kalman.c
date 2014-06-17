#include <stdio.h>
#include "kalman.h"

#define FS_ACC 1000.0

void predictKalman( kalmanFilter_t * filter, double u ) {
	Matrix * temp1 = Matrix_product( filter->F, filter->x );
	Matrix * temp2 = Matrix_product_scaler( filter->B, u  ); 
	Matrix * temp3 = Matrix_add( temp1, temp2 );
	free( temp1->data );
	free( temp1 );
	free( temp2->data );
	free( temp2 );

	free( filter->x->data );
	free( filter->x );
	filter->x = Matrix_clone( temp3 );
	free( temp3->data );
	free( temp3 );
	
}

void updateKalman( kalmanFilter_t * filter, double z ) {
	Matrix * temp = Matrix_product( filter->H, filter->x );
	Matrix * temp1;
	double y = z - *(temp->data);

	free( temp->data );
	free( temp );

	temp = Matrix_product_scaler( filter->K, y );
	temp1 = Matrix_add( filter->x, temp );

	free( filter->x->data );
	free( filter->x );
	filter->x = Matrix_clone( temp1 );

	free( temp->data );
	free( temp );
	free( temp1->data );
	free( temp1 );

}

void initKalman( kalmanFilter_t * filter ) {
	filter->x = Matrix_create( 3, 1 );
	Matrix_set( filter->x, 0, 0, 0 );
	Matrix_set( filter->x, 1, 0, 0 );
	Matrix_set( filter->x, 2, 0, 0 );

	filter->K = Matrix_create( 3, 1 );
	Matrix_set( filter->K, 0, 0, 0.05 );
	Matrix_set( filter->K, 1, 0, 0.25 );
	Matrix_set( filter->K, 2, 0, 0.25 );

	filter->F = Matrix_create( 3, 3 );
	Matrix_set( filter->F, 0, 0, 1 );
	Matrix_set( filter->F, 0, 1, 0 );
	Matrix_set( filter->F, 0, 2, 0 );
	Matrix_set( filter->F, 1, 0, 1/FS_ACC );
	Matrix_set( filter->F, 1, 1, 1 );
	Matrix_set( filter->F, 1, 2, 0 );
	Matrix_set( filter->F, 2, 0, 0 );
	Matrix_set( filter->F, 2, 1, 1/FS_ACC );
	Matrix_set( filter->F, 2, 2, 1 );

	filter->B = Matrix_create( 3, 1 );
	Matrix_set( filter->B, 0, 0, 0 );
	Matrix_set( filter->B, 1, 0, 1/FS_ACC );
	Matrix_set( filter->B, 2, 0, 0 );

	filter->H = Matrix_create( 1, 3 );
	Matrix_set( filter->H, 0, 0, 0 );
	Matrix_set( filter->H, 0, 1, 0 );
	Matrix_set( filter->H, 0, 2, 1 );

}
	
void destroyKalman( kalmanFilter_t * filter ) {
	free( filter->x->data );
	free( filter->x );
	free( filter->F->data );
	free( filter->F );
	free( filter->B->data );
	free( filter->B );
	free( filter->H->data );
	free( filter->H );
	free( filter->K->data );
	free( filter->K );
}
	


