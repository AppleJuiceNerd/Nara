#include "../../naraapi.h"

using namespace Nara;


void Sayo::SetLight(int key, int fn, Color color)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);

	// Modify the package
	package.led_fn[fn].r = color.r;
	package.led_fn[fn].g = color.g;
	package.led_fn[fn].b = color.b;

	// Write new values
	LL::light_config_write(device, key, &package);
}

Color Sayo::ReadLight(int key, int fn)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);
	
	// Return values
	return {
		package.led_fn[fn].r,
		package.led_fn[fn].g,
		package.led_fn[fn].b
	};
}


void Sayo::SetLightMode(int key, int fn, int mode)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);

	// Modify the package
	package.led_fn[fn].led_mode = ( package.led_fn[fn].led_mode & 0xF0 ) | ( mode & 0xF );

	// Write new values
	LL::light_config_write(device, key, &package);
}

int Sayo::ReadLightMode(int key, int fn)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);
	
	// Return values
	return package.led_fn[fn].led_mode & 0xF;
}


void Sayo::SetColorMode(int key, int fn, int mode)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);

	// Modify the package
	package.led_fn[fn].led_mode = ( package.led_fn[fn].led_mode & 0b11001111 ) | ( (mode & 0xb11) << 4 );

	// Write new values
	LL::light_config_write(device, key, &package);
}

int Sayo::ReadColorMode(int key, int fn)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);
	
	// Return values
	return ( package.led_fn[fn].led_mode & 0b00110000 ) >> 4;
}


void Sayo::SetLightColorTable(int key, int fn, int table)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);

	// Modify the package
	package.led_fn[fn].color_table_number = table;

	// Write new values
	LL::light_config_write(device, key, &package);
}

int Sayo::GetLightColorTable(int key, int fn)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);
	
	// Return values
	return package.led_fn[fn].color_table_number;
}


void Sayo::SetLightTriggerEvent(int key, int fn, int event)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);

	// Modify the package
	package.led_fn[fn].event = event;

	// Write new values
	LL::light_config_write(device, key, &package);
}

int Sayo::GetLightTriggerEvent(int key, int fn)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);
	
	// Return values
	return package.led_fn[fn].event;
}


void Sayo::SetLightSpeed(int key, int fn, int speed)
{
		// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);

	// Modify the package
	package.led_fn[fn].event = ( package.led_fn[fn].event & 0b11000000 ) | ( (speed & 0b11) << 6 );

	// Write new values
	LL::light_config_write(device, key, &package);
}

int Sayo::GetLightSpeed(int key, int fn)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);
	
	// Return values
	return ( package.led_fn[fn].event & 0b11000000 ) >> 6;
}


void Sayo::SetLightDuration(int key, int fn, uint8_t duration)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);

	// Modify the package
	package.led_fn[fn].lighting_time = duration;

	// Write new values
	LL::light_config_write(device, key, &package);
}

uint8_t Sayo::GetLightDuration(int key, int fn)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);
	
	// Return values
	return package.led_fn[fn].lighting_time;
}


void Sayo::SetDarkDuration(int key, int fn, uint8_t duration)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);

	// Modify the package
	package.led_fn[fn].dark_time = duration;

	// Write new values
	LL::light_config_write(device, key, &package);
}

uint8_t Sayo::GetDarkDuration(int key, int fn)
{
	// Initialize package
	struct API_CMD_0X11 package = { 0 };
	
	// Read the current values
	LL::light_config_read(device, key, &package);
	
	// Return values
	return package.led_fn[fn].dark_time;
}
