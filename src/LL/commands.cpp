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


void Nara::LL::light_config_write(hid_device *sayo, uint8_t key, struct API_CMD_0X11 *pkg)
{
	// Set up headers
	struct PKT_HEADER pkt = { 0x22, NARA_ECHO_CODE, 0 };
	struct CMD_HEADER cmd = { 60, 0x11, key };

	// Set up byte array
	uint8_t data[1024] = { 0 };

	// Copy packet header, command header, and package to byte array
	memcpy(data, &pkt, sizeof(pkt));
	memcpy(&data[4], &cmd, sizeof(cmd));
	memcpy(&data[8], pkg, sizeof(*pkg));

	// Calculate checksum
	pkt.checksum = checksum(data, 1024);
	
	// Copy checksum to byte array
	memcpy(&data, &pkt, sizeof(pkt));

	// Send packet to Sayodevice
	hid_write(sayo, data, 1024);
	
	// Wait for a response
	// NOTE: May be prone to infinite loops
	do {
		hid_read(sayo, data, 1024);
	} while (data[1] != NARA_ECHO_CODE);

	// Copy package data to package
	memcpy(pkg, &data[8], sizeof(*pkg));
}

void Nara::LL::light_config_read(hid_device *sayo, uint8_t key, struct API_CMD_0X11 *pkg)
{
	// Set up headers
	struct PKT_HEADER pkt = { 0x22, NARA_ECHO_CODE, 0 };
	struct CMD_HEADER cmd = { 0x04, 0x11, key };

	// Set up byte array
	uint8_t data[1024] = { 0 };

	// Copy packet header and command header to byte array
	memcpy(data, &pkt, sizeof(pkt));
	memcpy(&data[4], &cmd, sizeof(cmd));

	// Calculate checksum
	pkt.checksum = checksum(data, 1000);
	
	// Copy checksum to byte array
	memcpy(&data, &pkt, sizeof(pkt));
	
	// Send packet to Sayodevice
	hid_write(sayo, data, 1024);

	// Wait for a response
	// NOTE: May be prone to infinite loops
	do {
		hid_read(sayo, data, 1024);
	} while (data[1] != NARA_ECHO_CODE);
	
	// Copy package data to package
	memcpy(pkg, &data[8], sizeof(*pkg));
}