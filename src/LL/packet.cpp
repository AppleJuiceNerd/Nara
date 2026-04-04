#include "../../naraapi.h"


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