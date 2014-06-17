/*
 * IIRlib.c
 *
 *  Created on: 05/06/2014
 *      Author: Soren
 */
#include "IIRlib.h"

double IIRUpdate( IIRfilt_t *filter, double input ) {
	filter->output = input*filter->b[0] + filter->u[0];

	int i = 0;
	for ( i = 0; i<filter->order; i++ ) {
		filter->u[i] = input * filter->b[i+1];
		filter->u[i] -= filter->output*filter->a[i+1];
		if ( i<filter->order-1 )
			filter->u[i] += filter->u[i+1];;
	}

	return filter->output;
};
