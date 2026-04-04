#include "../../naraapi.h"
#include <string.h>


uint16_t Nara::LL::LightData::GetLength()
{
	return length;
}

void Nara::LL::LightData::GetBytes(uint8_t *bytes)
{
	// Header
	*(uint16_t*)(&bytes[0]) = length + 4;
	bytes[2] = command;
	bytes[3] = index;

	if (length == 0)
	{
		return;
	}

	// Data
	memcpy(&bytes[4], &pkg, CMD_0X11_SIZE);
}

void Nara::LL::LightData::LoadBytes(uint8_t *bytes)
{
	// Header
	index = bytes[3];

	// Data
	memcpy(&pkg, &bytes[4], CMD_0X11_SIZE);
}