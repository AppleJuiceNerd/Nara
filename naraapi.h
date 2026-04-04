#include <stdint.h>
#include <hidapi/hidapi.h>
#include <vector>

#pragma once


// SayoDevice API Structures
struct API_LED_DATA
{
	uint8_t led_mode;
	uint8_t event;
	uint8_t lighting_time;
	uint8_t dark_time;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t color_table_number;
};


#define CMD_0X11_SIZE 56

#pragma pack(1)
struct API_CMD_0X11 {
	uint8_t valid;
	uint8_t led_class;
	uint16_t reserve1;
	uint16_t led_site_x;
	uint16_t led_site_y;
	uint16_t led_width;
	uint16_t led_height;
	uint16_t fillet_angle;
	uint16_t reserve2;
	struct API_LED_DATA led_fn[5]; // 5 fns
};


// Nara's Echo (mostly functionally useless, just cool to have)
#define NARA_ECHO_CODE 0xC5


namespace Nara 
{
	// Enums for the led_data values

	// The light mode that determines how the light will behave
	enum LED_Modes
	{
		STATIC         = 0x0,
		INDICATOR      = 0x1,
		BREATHING      = 0x2,
		BREATHING_ONCE = 0x3,
		WAVE           = 0x4,
		SWITCH         = 0x6,
		SWITCH_ONCE    = 0x7,
		BLINK          = 0x8,
		BLINK_ONCE     = 0x9,
		FADE_OUT       = 0xE,
		FADE_IN        = 0xF
	};

	// The color mode of the light, which changes what colors are used 
	enum LED_ColorModes
	{
		STATIC_COLOR        = 0x0,
		LOOP_COLOR_TABLES   = 0x1,
		RANDOM_COLOR_TABLES = 0x2,
		RANDOM_COLOR        = 0x3
	};

	// The speed, which changes how fast the lights change, if the light mode allows for it
	enum LED_Speed
	{
		X8 = 0x0,
		X4 = 0x1,
		X2 = 0x2,
		X1 = 0x3
	};

	// The trigger event, changes what the light reacts to
	enum LED_Event
	{
		NONE               = 0x0,
		PRESS_LIGHT_UP     = 0x1,
		RELEASE_LIGHT_UP   = 0x2,
		PRESS_FADE_IN      = 0x3,
		RELEASE_FADE_IN    = 0x4,
		PRESS_NEXT_COLOR   = 0x5,
		RELEASE_NEXT_COLOR = 0x6,
		SCRIPT_RUNNING     = 0x7,
		SCRIPT_STOP        = 0x8
	};

	// The indicator mode, changes what the light will indicate
	enum LED_Indicator
	{
		NUM_LOCK     = 0x1,
		CAPS_LOCK    = 0x2,
		SCROLL_LOCK  = 0x4,
		COMPOSE_LOCK = 0x8,
		KANA_LOCK    = 0x10
	};


	// Lower level functions
	namespace LL
	{
		// Represents a SayoDevice command package
		// Acts as a base class
		class Package
		{
		protected:
			uint8_t command; // Configured based on derived class
			uint16_t length; // Also configured based on derived class

		public:
			uint8_t index;

			// Assembles the package into a byte array for use in a Packet
			virtual void GetBytes(uint8_t *bytes);

			// Reads a Package and loads corresponding fields
			virtual void LoadBytes(uint8_t *bytes);

			// Gets the length of this Package
			virtual uint16_t GetLength();
		};

		class LightData : public Package
		{
		public:
			// The API structure this package represents
			struct API_CMD_0X11 pkg;

			// Assembles the package into a byte array for use in a Packet
			void GetBytes(uint8_t *bytes) override;

			// Assembles the package into a byte array for use in a Packet
			void LoadBytes(uint8_t *bytes) override;
			

			uint16_t GetLength() override;

			// Set private fields
			LightData(bool read = false)
			{
				command = 0x11;
				if(!read)
				{
					length = 56; // LightData packages are always 56 bytes long
				}
				else 
				{
					length = 0;
				}
			}
		};

		// Represents a SayoDevice HID packet
		class Packet
		{
		private:
			// The byte array that will be used as the packet
			uint8_t data[1024] = { 0 };

			// Updates the byte array
			// Should be called whenever changes are made to the package list
			void UpdateData();
		public:
			bool long_packet = true; // Determines if the packet length is 64 (false) or 1024 (true)
			uint8_t echo = NARA_ECHO_CODE; // The echo code

			std::vector<Package*> packages; // The packages that make up the packet

			// Assembles the packet into a byte array for sending
			void GetBytes(uint8_t *bytes);
		};

		
		// A deserialization helper for SayoDevice communication packets
		class Boxcutter
		{
		private:
			uint8_t data[1024] = { 0 };
			std::vector<uint16_t> offsets; 

		public:
			Boxcutter(uint8_t *bytes);

			// Gets the offset at index
			uint16_t GetOffset(int index);

		};
		
		uint16_t checksum(uint8_t *data, int length);
		Packet send_packet(hid_device *sayo, Packet pkt);

		void set_key_lights(hid_device *sayo, uint8_t key, LightData req_data, uint8_t *result);
		void read_key_lights(hid_device *sayo, uint8_t key, uint8_t *result);
	};

	// Higher level functions and interfaces

	// Represents a color
	typedef struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	} Color;

	// Represents a Sayodevice.
	class Sayo
	{
	private:
		hid_device *device;

		// Base functions to mitigate code repetition

		LL::LightData LightSetup(int key);
		void LightSend(LL::LightData lights);

	public:
		// Gets the first sayodevice found and uses it
		Sayo();

		// Getter for the write hid device
		hid_device *get_device();

		
		void SetLight(int key, int fn, Color color);
		Color ReadLight(int key, int fn);

		void SetLightMode(int key, int fn, int mode);
		int ReadLightMode(int key, int fn);

		void SetColorMode(int key, int fn, int mode);
		int ReadColorMode(int key, int fn);

		void SetLightColorTable(int key, int fn, int table);
		int GetLightColorTable(int key, int fn);
		
		// NOTE: These don't use enum types because there's two event types
		void SetLightTriggerEvent(int key, int fn, int event);
		int GetLightTriggerEvent(int key, int fn);

		void SetLightSpeed(int key, int fn, int speed);
		int GetLightSpeed(int key, int fn);

		void SetLightDuration(int key, int fn, uint8_t duration);
		uint8_t GetLightDuration(int key, int fn);

		void SetDarkDuration(int key, int fn, uint8_t duration);
		uint8_t GetDarkDuration(int key, int fn);
	};

	// Basically wraps hid_init() to a T. Returns 0 on success, -1 otherwise.
	int Init();

	// Basically wraps hid_exit() to a T. Returns 0 on success, -1 otherwise.
	int Exit();

	// Returns a list of all of the Sayodevices that are connected
	// This doesn't exist yet due to limited testing resources
	// void Devices();
}

