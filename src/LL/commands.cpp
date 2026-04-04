#include "../../naraapi.h"
#include <hidapi/hidapi.h>
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

Nara::LL::Packet Nara::LL::send_packet(hid_device *sayo, Packet pkt)
{
	// Prepare byte array
	uint8_t data[1024] = { 0 };

	// Fill byte array with Packet data
	pkt.GetBytes(data);

	// Send data to the device
	hid_write(sayo, data, 1024); // TODO: This should probably account for short packets

	// Retrieve the result
	// NOTE: May be prone to infinite loops
	do {
		hid_read(sayo, data, 1024);
	} while (data[1] != pkt.echo);

	Packet out;
	return out;
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