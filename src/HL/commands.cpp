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