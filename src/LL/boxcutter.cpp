#include "../../naraapi.h"
#include <string.h>


Nara::LL::Boxcutter::Boxcutter(uint8_t *bytes)
{
	// Copy the passed bytes to data
	if(bytes[0] == 0x21)
	{
		memcpy(data, bytes, 64);
	} else
	{
		memcpy(data, bytes, 1024);
	}

	// Look for packages in data

	int offset = 4;

	// If the value at the 
	while(data[offset] != 0)
	{
		offsets.push_back(offset);
		offset += *(uint16_t*)(&data[offset]);
	}
}


uint16_t Nara::LL::Boxcutter::GetOffset(int index)
{
	return offsets[index];
}