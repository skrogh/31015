#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void bitUnpack( unsigned char* bitOut8, unsigned char* bitIn7, int n ) {
	int j,a,b;
	for ( int i = 1; i <= n-ceil( n/8.0 ); i++ ) {
		j = floor(( 8*( i - 8 ) ) / 7 + 9 );
		a = 1 + ( i - 1 ) % 7;
		b = -6 + ( i - 1 ) % 7;

		*(bitOut8 + i) = ( *(bitIn7 + j - 1) << a ) | ( *(bitIn7 +j) << b );
	}
}

int main() {
	unsigned char* bitOut8 = malloc( 4 );
	unsigned char bitIn7[] = { 0x8a, 0x70, 0x0a, 0x55 };

	bitUnpack( bitOut8, bitIn7, 4 );
	printf( "%x %x %x %x \n", *(bitOut8+0), *(bitOut8+1), *(bitOut8+2), *(bitOut8+3) );

	return 0;
}
