/*
 ============================================================================
 Name        : IIRlib.c
 Author      : Soeren Andersen
 Version     :
 Copyright   : 2014
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "lib/IIRlib.h"

int main(void) {

	double a[] = {1, -1};
	double b[] = {0.001, 0};
	double u[] = {0};


	IIRfilt test = { (double*)a, (double*)b, (double*)u, 1, 0 };


	puts("Lorem Ipsum"); /* prints Lorem Ipsum */

	int i;
	for( i = 0; i<100; i++) {
		printf( "output: %f\n", IIRUpdate( &test, 1 ) );

	}


	return EXIT_SUCCESS;
}
