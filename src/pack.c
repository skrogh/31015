#include "pack.h"

void bitUnpack( unsigned char* bitOut8, unsigned char* bitIn7, int n, int unpackSize ); 
int findPackets( int n, int packetSize, unsigned char * recBytes, unsigned char * recPackets ); 

static int numBytesBuffered;

int unpackPackets( int packetSize, unsigned char * packedData, int numBytesIn, unsigned char * unpackedData ) {
	unsigned char * recPackets = malloc( sizeof( unsigned char ) * ( numBytesIn + packetSize ) );
	int numPackets = findPackets( numBytesIn, packetSize, packedData, recPackets );
	int unpackSize = numPackets * (( packetSize * 7 ) / 8 + ( ( ( packetSize * 7 ) % 8 ) != 0 ) ); 
	bitUnpack( unpackedData, recPackets, numPackets, unpackSize ); 
	
	free( recPackets );
	return unpackSize;
}

void bitUnpack( unsigned char* bitOut8, unsigned char* bitIn7, int n, int unpackSize ) {
	int j,a,b;
	for ( int i = 1; i <= unpackSize; i++ ) {
		*(bitIn7 + i - 1) &=0x7f;
		j = ( 8*( i - 1 ) ) / 7 + 1;
		a = 1 + ( i - 1 ) % 7;
		b = -6 + ( i - 1 ) % 7;
		*(bitOut8 + i - 1) = ( *(bitIn7 + j - 1) << a ) | ( *(bitIn7 +j) >> -b );
	}
}


int findPackets( int n, int packetSize, unsigned char * recBytes, unsigned char * recPackets ) {
	unsigned char temp[packetSize];
	bool completepacket = true;
	int shortPacket = 0;
	int packetCount = 0;
	for ( int i = packetSize - numBytesBuffered - 1; i < n + packetSize; i++ ) {
		numBytesBuffered = 0;
		shortPacket = 0;
		completepacket = true;
		if ( ( ( *(recBytes + i ) & 0x80 ) > 0 )  && ( i + packetSize ) <= ( n + packetSize ) ) {
			temp[ 0 ] = *(recBytes + i );
			for ( int j = 1; j < packetSize; j++ ) {
				temp[ j ] = *(recBytes + i + j );
				if ( (temp[ j ] & 0x80) > 0 ) {
					completepacket = false;
					shortPacket = j;
					break;
				}
			}
			if ( completepacket == true ) {
				for ( int j = 0; j < packetSize; j++ ) {
					*(recPackets+j + packetCount * packetSize) = temp[ j ];
				}
				packetCount++;
				i += ( packetSize - 1 ); // i++ happens after loop exit
			} else {
				i += shortPacket;
			}
		} else if ( ( i + packetSize ) > ( n + packetSize ) ) {
			numBytesBuffered = n + packetSize - i;
			for ( int j = 0; j < numBytesBuffered; j++ ) {
				*( recBytes + packetSize - numBytesBuffered + j ) = *( recBytes + j + i );
			}
			break;
		}

	}

	return packetCount;
}
