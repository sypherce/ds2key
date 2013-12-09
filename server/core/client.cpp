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
				case kBlue:
					return DS2KEY_BLUE;
				case kYellow:
					return DS2KEY_YELLOW;
				case kRed:
					return DS2KEY_RED;
				case kGreen:
					return DS2KEY_GREEN;
				default:
					return 0;
			}
		}
		//currently unused and broken with GH functions
		/*uint16_t button2bit(uint16_t Button) {
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
				case DS2KEY_BLUE:
					return kBlue;
				case DS2KEY_YELLOW:
					return kYellow;
				case DS2KEY_RED:
					return kRed;
				case DS2KEY_GREEN:
					return kGreen;
				default:
					return kEND;
			}
		}*/
		C::Client *Client[256] = {(C::Client*)NULL};

		namespace C {
			Client::Client() {
				packet = DS2KeyPacket{0};
				keys =
				keysOld =
				ghKeys =
				ghKeysOld = 0;
			}

			Client::~Client() { }

			void Client::Scan(void) {
				keysOld = keys;
				keys = packet.Keys;

				ghKeysOld = ghKeys;
				ghKeys = packet.GHKeys;
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

			void Client::SetPacket(DS2KeyPacket p) {
				packet = p;
			}

			void Client::SetTouchPos(uint8_t x, uint8_t y) {
				packet.TouchX = x;
				packet.TouchY = y;
			}

			void Client::Press(uint16_t key) {
				packet.Keys |= key;
			}

			void Client::Release(uint16_t key) {
				packet.Keys &= ~key;
			}

			bool Client::Held(uint16_t key) {
				return keys&key;
			}

			bool Client::Down(uint16_t key) {
				return (keys &~ keysOld)&key;
			}

			bool Client::Up(uint16_t key) {
				return ((keys ^ keysOld) & (~keys))&key;
			}

			bool Client::Turbo(uint16_t key) {
				return packet.KeysTurbo&key;
			}

			void Client::GHPress(uint8_t key) {
				packet.GHKeys |= key;
			}

			void Client::GHRelease(uint8_t key) {
				packet.GHKeys &= ~key;
			}

			bool Client::GHHeld(uint8_t key) {
				return ghKeys&key;
			}

			bool Client::GHDown(uint8_t key) {
				return (ghKeys &~ ghKeysOld)&key;
			}

			bool Client::GHUp(uint8_t key) {
				return ((ghKeys ^ ghKeysOld) & (~ghKeys))&key;
			}

			bool Client::GHTurbo(uint8_t key) {
				return packet.GHKeysTurbo&key;
			}

			uint8_t Client::GetX() {
				return packet.TouchX;
			}

			uint8_t Client::GetY() {
				return packet.TouchY;
			}

			uint8_t Client::GetProfileNumber() {
				return packet.Profile;
			}
		}
	}
}
