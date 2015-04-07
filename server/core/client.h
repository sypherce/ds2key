#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>		//std::string
#include <cstdint>		//uint8_t, etc
#include "common/ds2keyPacket.h"

#define D2K_COMMAND "*"
#define D2K_COMMAND_LENGTH 1

namespace D2K {
	static const uint16_t DS2KEY_A = (1 << (0));
	static const uint16_t DS2KEY_B = (1 << (1));
	static const uint16_t DS2KEY_SELECT = (1 << (2));
	static const uint16_t DS2KEY_START = (1 << (3));
	static const uint16_t DS2KEY_RIGHT = (1 << (4));
	static const uint16_t DS2KEY_LEFT = (1 << (5));
	static const uint16_t DS2KEY_UP = (1 << (6));
	static const uint16_t DS2KEY_DOWN = (1 << (7));
	static const uint16_t DS2KEY_R = (1 << (8));
	static const uint16_t DS2KEY_L = (1 << (9));
	static const uint16_t DS2KEY_X = (1 << (10));
	static const uint16_t DS2KEY_Y = (1 << (11));
	static const uint16_t DS2KEY_TOUCH = (1 << (12));
	static const uint16_t DS2KEY_LID = (1 << (13));

	static const uint8_t DS2KEY_BLUE = (1 << (3));
	static const uint8_t DS2KEY_YELLOW = (1 << (4));
	static const uint8_t DS2KEY_RED = (1 << (5));
	static const uint8_t DS2KEY_GREEN = (1 << (6));
	//this represents each value in a Keys Profile array. Example: Profile[kUp] is defined as KEY_UP by default
	//this was stored in config.h but will be garbage soon
	enum Keys {
		kProfile, kMouse, kJoy, kUp, kDown, kLeft, kRight, kA, kB, kX, kY, kL, kR, kStart, kSelect, kLid,
		kBlue, kYellow, kRed, kGreen,
		kTouch00, kTouch01, kTouch02, kTouch03, kTouch04, kTouch05, kTouch06, kTouch07, kTouch08, kTouch09, kTouch10, kTouch11,
		kTouchString00, kTouchString01, kTouchString02, kTouchString03, kTouchString04, kTouchString05, kTouchString06, kTouchString07, kTouchString08, kTouchString09, kTouchString10, kTouchString11,
		kTouch00X, kTouch01X, kTouch02X, kTouch03X, kTouch04X, kTouch05X, kTouch06X, kTouch07X, kTouch08X, kTouch09X, kTouch10X, kTouch11X,
		kTouch00Y, kTouch01Y, kTouch02Y, kTouch03Y, kTouch04Y, kTouch05Y, kTouch06Y, kTouch07Y, kTouch08Y, kTouch09Y, kTouch10Y, kTouch11Y,
		kTouch00W, kTouch01W, kTouch02W, kTouch03W, kTouch04W, kTouch05W, kTouch06W, kTouch07W, kTouch08W, kTouch09W, kTouch10W, kTouch11W,
		kTouch00H, kTouch01H, kTouch02H, kTouch03H, kTouch04H, kTouch05H, kTouch06H, kTouch07H, kTouch08H, kTouch09H, kTouch10H, kTouch11H,
		kEND };
	//probably not the best place for bit2button
	extern uint16_t bit2button(uint16_t Bit);

	class ProfileData {
	private:
		bool isVirtualKey(std::string Button);
		uint16_t StringToVirtualKey(std::string Button);
		std::string VirtualKeyToString(uint16_t Button);

	public:
		ProfileData();
		~ProfileData();

		std::string &GetStringReference(int Button);

		//@param Button (enum Keys) value. Example: kA, kUp, kGreen
		//@return Human readable version of (Button)'s current setting. Example: KEY_A, KEY_UP
		std::string GetButtonString(int Button);
		uint16_t GetValue(int Button);
		uint16_t GetVirtualKey(int Button);

		void SetVirtualKey(int Button, uint16_t Value);
		void SetCommand(int Button, std::string Value);
		void SetTouchPos(uint8_t I, uint8_t X, uint8_t Y, uint8_t W, uint8_t H);
		const std::string &GetCommand(int Button);

		uint8_t ID;//uint8_t
		std::string Mouse;//uint8_t
		std::string Joy;//uint8_t
		//buttons
		std::string Up, Down, Left, Right, A, B, X, Y, L, R, Start, Select, Lid;
		//guitar grip buttons
		std::string Blue, Yellow, Red, Green;//uint8_t?
		std::string Empty;//this probably shouldn't be used but I don't know what to do
		std::string Touch[12];//Command
		std::string TouchString[12];//String
		uint8_t TouchX[12];//uint8_t
		uint8_t TouchY[12];//uint8_t
		uint8_t TouchW[12];//uint8_t
		uint8_t TouchH[12];//uint8_t
	};

	//Contains a client's current state
	class Client {
		public:
			Client();
			~Client();

			//Updates current button state after calls made to Press, Release, GHPress, and GHRelease
			void Scan(void);

			//@return Pointer to Profile array
			ProfileData *GetProfileDataPointer();

			void SetPacket(UDP::DS2KeyPacket p);
			bool Held(uint16_t key);
			bool Down(uint16_t key);
			bool Up(uint16_t key);

			//@return true if (key) has Turbo mode enabled
			bool Turbo(uint16_t key);

			bool GHHeld(uint8_t key);
			bool GHDown(uint8_t key);
			bool GHUp(uint8_t key);

			//@return true if (key) has Turbo mode enabled
			bool GHTurbo(uint8_t key);

			//@return Stylus current X Position. Values range 0-255
			uint8_t GetX();

			//@return Stylus current X Position. Values range 0-191
			uint8_t GetY();

		private:
			uint8_t GetProfileNumber();
			void SetTouchPos(uint8_t x, uint8_t y);
			void Press(uint16_t key);
			void Release(uint16_t key);

			void GHPress(uint8_t key);
			void GHRelease(uint8_t key);

			UDP::DS2KeyPacket packet;
			ProfileData Profile;

			uint16_t keys;
			uint16_t keysOld;
			uint8_t ghKeys;
			uint8_t ghKeysOld;
	};

	extern Client *ClientArray[256];
}

#endif//__CLIENT_H__
