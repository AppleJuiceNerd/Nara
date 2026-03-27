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


Nara::LL::LightData Nara::Sayo::LightSetup(int key)
{
	// The data to be sent and received
	uint8_t results[1024] = { 0 };

	// Get current state of the lights
	LL::read_key_lights(device, key, results);
	
	// Copy that state to a Package
	LL::LightData lights;
	lights.LoadBytes(&results[LL::Boxcutter(results).GetOffset(0)]);

	return lights;
}

void Nara::Sayo::LightSend(LL::LightData lights)
{
	// Set the new lights
	// TODO: lights.index feels repetitive; maybe this function shouldn't need an index
	LL::set_key_lights(device, lights.index, lights, NULL);
}


void Nara::Sayo::SetLight(int key, int fn, Color color)
{
	LL::LightData lights = LightSetup(key);

	// Modify the color
	lights.led_fn[fn].r = color.r;
	lights.led_fn[fn].g = color.g;
	lights.led_fn[fn].b = color.b;

	LightSend(lights);

	return;
}

Nara::Color Nara::Sayo::ReadLight(int key, int fn)
{
	LL::LightData lights = LightSetup(key);

	// Return the color
	return {
		lights.led_fn[fn].r,
		lights.led_fn[fn].g,
		lights.led_fn[fn].b
	};
}

void Nara::Sayo::SetLightMode(int key, int fn, LED_Modes mode)
{
	LL::LightData lights = LightSetup(key);

	// Modify the Light mode
	lights.led_fn[fn].led_mode = mode;

	LightSend(lights);

	return;
}

Nara::LED_Modes Nara::Sayo::ReadLightMode(int key, int fn)
{
	LL::LightData lights = LightSetup(key);

	// Return the led mode
	return (LED_Modes) lights.led_fn[fn].led_mode;
}