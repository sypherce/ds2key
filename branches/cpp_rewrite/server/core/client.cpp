/*
	Virtual DS status
*/

//#include <stdio.h>
#include "client.h"
#include "key.h"

namespace D2K
{
	namespace Core
	{
		Client::Client()
		{
			packet = {0, 0, 0, 0, 0, 0};
			keys =
			keysOld =
			ghKeys =
			ghKeysOld = 0;
		}

		Client::~Client() { }

		void Client::Scan(void)
		{
			keysOld = keys;
			keys = packet.keys;

			ghKeysOld = ghKeys;
			ghKeys = packet.ghKeys;
		}

		uint16_t *Client::GetSettings()
		{
			return Profile;
		}

		std::string Client::GetButtonString(int Button)
		{
			return Key::GetString(GetButton(Button));
		}

		uint16_t Client::GetButton(int Button)
		{
			return GetSettings()[Button];
		}

		void Client::SetButton(int Button, uint16_t Value)
		{
			GetSettings()[Button] = Value;
		}

		void Client::SetPacket(ds2keyPacket p)
		{
			packet = p;
		}

		void Client::SetTouchPos(uint8_t x, uint8_t y)
		{
			packet.touchX = x;
			packet.touchY = y;
		}

		void Client::Press(uint16_t key)
		{
			packet.keys |= key;
		}

		void Client::Release(uint16_t key)
		{
			packet.keys &= ~key;
		}

		uint16_t Client::Held(uint16_t key)
		{
			return keys&key;
		}

		uint16_t Client::Down(uint16_t key)
		{
			return (keys &~ keysOld)&key;
		}

		uint16_t Client::Up(uint16_t key)
		{
			return ((keys ^ keysOld) & (~keys))&key;
		}

		uint16_t Client::Turbo(uint16_t key)
		{
			return packet.keysTurbo&key;
		}

		void Client::GHPress(uint8_t key)
		{
			packet.ghKeys |= key;
		}

		void Client::GHRelease(uint8_t key)
		{
			packet.ghKeys &= ~key;
		}

		uint8_t Client::GHHeld(uint8_t key)
		{
			return ghKeys&key;
		}

		uint8_t Client::GHDown(uint8_t key)
		{
			return (ghKeys &~ ghKeysOld)&key;
		}

		uint8_t Client::GHUp(uint8_t key)
		{
			return ((ghKeys ^ ghKeysOld) & (~ghKeys))&key;
		}

		uint8_t Client::GHTurbo(uint8_t key)
		{
			return packet.ghKeysTurbo&key;
		}

		uint8_t Client::GetX()
		{
			return packet.touchX;
		}

		uint8_t Client::GetY()
		{
			return packet.touchY;
		}

		uint8_t Client::GetProfileNumber()
		{
			return packet.profile;
		}
	}
}
