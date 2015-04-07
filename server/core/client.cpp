/*
	Virtual DS status
*/

#include "client.h"
#include "key.h"
#include "common/misc.h"
#include <sstream>	//std::stringstream

namespace D2K {
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
	Client *ClientArray[256] = {(Client*)NULL};

	bool ProfileData::isVirtualKey(std::string Button) {
		if(Button.substr(0, D2K_COMMAND_LENGTH) != D2K_COMMAND)	//if button
			return true;
		//if command
			return false;
	}
	uint16_t ProfileData::StringToVirtualKey(std::string Button) {
		if(isVirtualKey(Button))
			return D2K::stoi(Button);
		else
			return 0;
	}
	//this currently is the same as D2K::itos
	std::string ProfileData::VirtualKeyToString(uint16_t Button) {
		std::stringstream stream;
		stream << D2K::itos(Button);

		return stream.str();
	}
	ProfileData::ProfileData() {
		Empty = "";
	}
	ProfileData::~ProfileData() {

	}
	std::string &ProfileData::GetStringReference(int Button) {
		switch(Button) {
			case kMouse:
				return Mouse;
			case kJoy:
				return Joy;
			case kA:
				return A;
			case kB:
				return B;
			case kX:
				return X;
			case kY:
				return Y;
			case kL:
				return L;
			case kR:
				return R;
			case kUp:
				return Up;
			case kDown:
				return Down;
			case kLeft:
				return Left;
			case kRight:
				return Right;
			case kStart:
				return Start;
			case kSelect:
				return Select;
			case kLid:
				return Lid;
			case kBlue:
				return Blue;
			case kYellow:
				return Yellow;
			case kRed:
				return Red;
			case kGreen:
				return Green;
			case kTouch00:
				return Touch[0];
			case kTouch01:
				return Touch[1];
			case kTouch02:
				return Touch[2];
			case kTouch03:
				return Touch[3];
			case kTouch04:
				return Touch[4];
			case kTouch05:
				return Touch[5];
			case kTouch06:
				return Touch[6];
			case kTouch07:
				return Touch[7];
			case kTouch08:
				return Touch[8];
			case kTouch09:
				return Touch[9];
			case kTouch10:
				return Touch[10];
			case kTouch11:
				return Touch[11];
			case kTouchString00:
				return TouchString[0];
			case kTouchString01:
				return TouchString[1];
			case kTouchString02:
				return TouchString[2];
			case kTouchString03:
				return TouchString[3];
			case kTouchString04:
				return TouchString[4];
			case kTouchString05:
				return TouchString[5];
			case kTouchString06:
				return TouchString[6];
			case kTouchString07:
				return TouchString[7];
			case kTouchString08:
				return TouchString[8];
			case kTouchString09:
				return TouchString[9];
			case kTouchString10:
				return TouchString[10];
			case kTouchString11:
				return TouchString[11];
			default:
				return Empty;
		}
	}

	std::string ProfileData::GetButtonString(int Button) {
		if(isVirtualKey(GetStringReference(Button)))
			return Key::GetString(GetVirtualKey(Button));
		else
			return GetCommand(Button);
	}
	uint16_t ProfileData::GetValue(int Button) {
		std::string &Pointer = GetStringReference(Button);
		if(Pointer == "") {
			return 0;
		}
		else {
			return D2K::stoi(Pointer);
		}
	}
	uint16_t ProfileData::GetVirtualKey(int Button) {
		std::string &Pointer = GetStringReference(Button);
		if(Pointer == "" || Pointer == "0") {
			return 0;
		}
		else {
			return StringToVirtualKey(Pointer);
		}
	}

	void ProfileData::SetVirtualKey(int Button, uint16_t Value) {
		std::string &Pointer = GetStringReference(Button);
		Pointer = VirtualKeyToString(Value);
	}

	void ProfileData::SetCommand(int Button, std::string Value) {
		std::string &Pointer = GetStringReference(Button);
		if(isVirtualKey(Value)) {
			SetVirtualKey(Button, Key::GetNumber(Value.c_str()));
		}
		else {
			Pointer = Value;
		}
	}
	void ProfileData::SetTouchPos(uint8_t I, uint8_t X, uint8_t Y, uint8_t W, uint8_t H) {
		if(I <= 11) {
			TouchX[I] = X;
			TouchY[I] = Y;
			TouchW[I] = W;
			TouchH[I] = H;
		}
	}
	const std::string &ProfileData::GetCommand(int Button) {
		std::string &Pointer = GetStringReference(Button);
		if(!isVirtualKey(Pointer)) {
			return Pointer;
		}
		return Empty;
	}

	Client::Client() {
		packet = UDP::DS2KeyPacket{0};
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

	ProfileData *Client::GetProfileDataPointer() {
		return &Profile;
	}

	void Client::SetPacket(UDP::DS2KeyPacket p) {
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
