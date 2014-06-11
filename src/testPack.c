#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "pack.h"

#define PACKETSIZE 16

FILE * buffer;

int readFrameTest(unsigned char * recBytes ) {
	int recByte = 0;
	unsigned char c = 'c';
	int i = 0;
	while ( fscanf( buffer, "%d%c", &recByte, &c ) > 0 ) {
		*(recBytes + i + PACKETSIZE ) = (char) recByte; 
		//printf( "%u,", recByte );
		i++;
		if ( c == '\n' ) {
			break;
		}

	}

	return i;
}

int main() {
	buffer = fopen( "lorem.txt", "r" );
	unsigned char * recBytes = malloc( sizeof( char ) * 200 );
	unsigned char* bitOut8 = malloc( sizeof( char ) * 200);


	int n = readFrameTest( recBytes );
	int byteCount = 0;
	while ( n > 0 ) {
		byteCount = unpackPackets( 16, recBytes, n, bitOut8 );   
		for ( int i = 0; i < byteCount ; i++ ) {
			printf( "%c", *(bitOut8 + i ) );
		}
		n = readFrameTest( recBytes );
	}


	return 0;
}
