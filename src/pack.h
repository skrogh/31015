#ifndef PACK_H_
#define PACK_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

int unpackPackets( int packetSize, unsigned char * packedData, int numBytesIn, unsigned char * unpackedData );

#endif
