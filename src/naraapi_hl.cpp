#include "../naraapi.h"
#include <cstring>
#include <hidapi.h>


int Nara::Init()
{
	return hid_init();
}

int Nara::Exit()
{
	return hid_exit();
}

// FIXME: This function might be prone to errors. I don't have more than one device to test.
Nara::Sayo::Sayo()
{
	struct hid_device_info *devices;
	
	// SayoDevice vendor code is 0x8089
	devices = hid_enumerate(0x8089, 0x0);

	// If there are no devices found, just set the device to NULL and stop executing this function.
	if (devices == NULL)
	{
		device = NULL;
		return;
	}

	// The fourth device is writable on both Windows and Linux
	devices = devices->next; 
	devices = devices->next; 
	devices = devices->next; 

	device = hid_open_path(devices->path);
	
	hid_free_enumeration(devices);
}

hid_device *Nara::Sayo::get_device()
{
	return device;
}