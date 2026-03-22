#include "../../naraapi.h"
#include <string.h>
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

void Nara::LL::set_key_lights(hid_device *sayo, uint8_t key, LightData req_data, uint8_t *result)
{
	uint8_t data[1024] = { 0 };

	Packet pkt;

	pkt.packages.push_back(&req_data);

	pkt.GetBytes(data);

	hid_write(sayo, data, 1024);

	// NOTE: May be prone to infinite loops
	do {
		hid_read(sayo, data, 1024);
	} while (data[1] != pkt.echo);

	if (result != NULL)
	{
		memcpy(result, &data, 1024);
	}
}

void Nara::LL::read_key_lights(hid_device *sayo, uint8_t key, uint8_t *result)
{
	uint8_t data[1024] = { 0 };

	Packet pkt;
	LightData lights(true);

	lights.index = key;

	pkt.packages.push_back(&lights);

	pkt.GetBytes(data);

	hid_write(sayo, data, 1024);

	// NOTE: May be prone to infinite loops
	do {
		hid_read(sayo, data, 1024);
	} while (data[1] != pkt.echo);
	
	if (result != NULL)
	{
		memcpy(result, &data, 1024);
	}
}