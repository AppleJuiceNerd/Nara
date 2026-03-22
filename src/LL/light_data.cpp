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
	bytes[4] = valid;
	bytes[5] = led_class;
	*(uint16_t*)(&bytes[6]) = reserve1;
	*(uint16_t*)(&bytes[8]) = led_site_x;
	*(uint16_t*)(&bytes[10]) = led_site_y;
	*(uint16_t*)(&bytes[12]) = led_width;
	*(uint16_t*)(&bytes[14]) = led_height;
	*(uint16_t*)(&bytes[16]) = fillet_angle;
	*(uint16_t*)(&bytes[18]) = reserve2;

	for(int i = 0; i < 5; i++)
	{
		memcpy(&bytes[20 + (i * 8)], &led_fn[i], 8);
	}
}


void Nara::LL::Packet::GetBytes(uint8_t *bytes)
{
	int size;
	
	// Check if the packet should be long 
	if (long_packet)
	{
		size = 1024;
		bytes[0] = 0x22;
	} else
	{
		size = 64;
		bytes[0] = 0x21;
	}
	
	bytes[1] = echo;
	
	
	// setup loop
	// offset in the packet
	int offset = 4;
	
	// The bytes that will be filled by the packages' GetBytes function
	
	uint8_t buffer[1024] = { 0 };
	
	for(int pkg = 0; pkg < packages.size(); pkg++)
	{
		// Assemble package bytes
		packages[pkg]->GetBytes(buffer);
		
		// Copy package to bytes at offset
		memcpy(&bytes[offset], &buffer, packages[pkg]->GetLength() + 4);

		// Move to the next available space
		// There should be four zeros after each package
		offset += packages[pkg]->GetLength() + 4;
	}
	
	*(uint16_t*)(&bytes[2]) = checksum(bytes, size);
}

void Nara::LL::LightData::LoadBytes(uint8_t *bytes)
{
	// Header
	index = bytes[3];

	// Data
	valid = bytes[4];
	led_class = bytes[5];
	reserve1 = *(uint16_t*)(&bytes[6]);
	led_site_x = *(uint16_t*)(&bytes[8]);
	led_site_y = *(uint16_t*)(&bytes[10]);
	led_width = *(uint16_t*)(&bytes[12]);
	led_height = *(uint16_t*)(&bytes[14]);
	fillet_angle = *(uint16_t*)(&bytes[16]);
	reserve2 = *(uint16_t*)(&bytes[18]);

	for(int i = 0; i < 5; i++)
	{
		memcpy(&led_fn[i], &bytes[20 + (i * 8)], 8);
	}
}