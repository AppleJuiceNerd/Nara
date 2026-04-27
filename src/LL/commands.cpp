#include "../../naraapi.h"
#include <hidapi/hidapi.h>
#include <hidapi.h>


// NOTE: While it may not happen, this function may not work properly with odd-length arrays.
uint16_t Nara::LL::checksum(uint8_t *data, int length)
{
	uint16_t sum = 0;
	for(int b = 0; b < length; b+=2)
	{
		sum += ( data[b+1] << 8 ) + data[b];
	}

	return sum;
}
