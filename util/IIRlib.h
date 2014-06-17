/*
 * IIRlib.h
 *
 *  Created on: 05/06/2014
 *      Author: Soren
 *
 *
 *      Implemetation of SISO IIR filter.
 *
 *
 *  output  /   \      b[0] + b[1] z^-1 + b[2]z^-2 + ... + b[n] z^-n
 * --------|  z  |  = -----------------------------------------------
 *   input  \   /       1 + a[1]z^-1 + a[2]z^-2 + ... + a[n]z^-n
 *
 *       Using direct form II transposed (as this is optimal for fp.)
 *
 *       Reference: http://book.huihoo.com/introduction-to-digital-filters-with-audio-applications/Transposed_Direct_Forms.html
 *
 */

#ifndef IIRLIB_H_
#define IIRLIB_H_

#include <stdint.h>


typedef struct {
	double *a; // Pointer to numerator array
	double *b; // Pointer to denominator array, assuming a[0] = 1
	double *u; // Pointer to delay-line array, must be of
	uint32_t order;
	double output;
} IIRfilt_t;

double IIRUpdate( IIRfilt_t *filter, double input );





#endif /* IIRLIB_H_ */
