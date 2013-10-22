/*
	Virtual DS status
*/

#include "client.h"
#include "key.h"

namespace D2K {
	namespace Core {
		uint16_t bit2button(uint16_t Bit) {
			switch(Bit) {
				case kUp:
					return DS2KEY_UP;
				case kDown:
					return DS2KEY_DOWN;
				case kLeft:
					return DS2KEY_LEFT;
				case kRight:
					return DS2KEY_RIGHT;
				case kA:
					return DS2KEY_A;
				case kB:
					return DS2KEY_B;
				case kX:
					return DS2KEY_X;
				case kY:
					return DS2KEY_Y;
				case kL:
					return DS2KEY_L;
				case kR:
					return DS2KEY_R;
				case kStart:
					return DS2KEY_START;
				case kSelect:
					return DS2KEY_SELECT;
				case kLid:
					return DS2KEY_LID;
				default:
					return 0;
			}
		}
		uint16_t button2bit(uint16_t Button) {
			switch(Button) {
				case DS2KEY_UP:
					return kUp;
				case DS2KEY_DOWN:
					return kDown;
				case DS2KEY_LEFT:
					return kLeft;
				case DS2KEY_RIGHT:
					return kRight;
				case DS2KEY_A:
					return kA;
				case DS2KEY_B:
					return kB;
				case DS2KEY_X:
					return kX;
				case DS2KEY_Y:
					return kY;
				case DS2KEY_L:
					return kL;
				case DS2KEY_R:
					return kR;
				case DS2KEY_START:
					return kStart;
				case DS2KEY_SELECT:
					return kSelect;
				case DS2KEY_LID:
					return kLid;
				default:
					return kEND;
			}
		}
		C::Client *Client[256] = {(C::Client*)NULL};

		namespace C {
			Client::Client() {
				packet = ds2keyPacket {0};
				keys =
				keysOld =
				ghKeys =
				ghKeysOld = 0;
			}

			Client::~Client() { }

			void Client::Scan(void) {
				keysOld = keys;
				keys = packet.keys;

				ghKeysOld = ghKeys;
				ghKeys = packet.ghKeys;
			}

			uint16_t *Client::GetSettings() {
				return Profile;
			}

			std::string Client::GetButtonString(int Button) {
				return Key::GetString(GetButton(Button));
			}

			uint16_t Client::GetButton(int Button) {
				return GetSettings()[Button];
			}

			void Client::SetButton(int Button, uint16_t Value) {
				GetSettings()[Button] = Value;
			}

			void Client::SetPacket(ds2keyPacket p) {
				packet = p;
			}

			void Client::SetTouchPos(uint8_t x, uint8_t y) {
				packet.touchX = x;
				packet.touchY = y;
			}

			void Client::Press(uint16_t key) {
				packet.keys |= key;
			}

			void Client::Release(uint16_t key) {
				packet.keys &= ~key;
			}

			uint16_t Client::Held(uint16_t key) {
				return keys&key;
			}

			uint16_t Client::Down(uint16_t key) {
				return (keys &~ keysOld)&key;
			}

			uint16_t Client::Up(uint16_t key) {
				return ((keys ^ keysOld) & (~keys))&key;
			}

			uint16_t Client::Turbo(uint16_t key) {
				return packet.keysTurbo&key;
			}

			void Client::GHPress(uint8_t key) {
				packet.ghKeys |= key;
			}

			void Client::GHRelease(uint8_t key) {
				packet.ghKeys &= ~key;
			}

			uint8_t Client::GHHeld(uint8_t key) {
				return ghKeys&key;
			}

			uint8_t Client::GHDown(uint8_t key) {
				return (ghKeys &~ ghKeysOld)&key;
			}

			uint8_t Client::GHUp(uint8_t key) {
				return ((ghKeys ^ ghKeysOld) & (~ghKeys))&key;
			}

			uint8_t Client::GHTurbo(uint8_t key) {
				return packet.ghKeysTurbo&key;
			}

			uint8_t Client::GetX() {
				return packet.touchX;
			}

			uint8_t Client::GetY() {
				return packet.touchY;
			}

			uint8_t Client::GetProfileNumber() {
				return packet.profile;
			}
		}
	}
}
