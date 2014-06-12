#include <stdlib.h>
#include <math.h>
#include "matrix.h"

typedef struct {
    Matrix *x;
	Matrix *F;
	Matrix *B;
	Matrix *H;
	Matrix *K;
} kalmanFilter_t;	

void predictKalman( kalmanFilter_t * filter, double u );
void updateKalman( kalmanFilter_t * filter, double z );
void initKalman( kalmanFilter_t * filter );
void destroyKalman( kalmanFilter_t * filter );
