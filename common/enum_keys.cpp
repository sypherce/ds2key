#include "enum_keys.h"

namespace D2K {namespace KEYS {

std::string GetKeyName(int key)
{
	switch(key)
	{
	case KEYS::A:
		return "A";
	case KEYS::B:
		return "B";
	case KEYS::X:
		return "X";
	case KEYS::Y:
		return "Y";
	case KEYS::L:
		return "L";
	case KEYS::R:
		return "R";
	case KEYS::DUP:
		return "DPad Up";
	case KEYS::DDOWN:
		return "DPad Down";
	case KEYS::DLEFT:
		return "DPad Left";
	case KEYS::DRIGHT:
		return "DPad Right";
	case KEYS::START:
		return "Start";
	case KEYS::SELECT:
		return "Select";
	case KEYS::LID:
		return "Lid";

	case KEYS::SLIDER_VOLUME:
		return "Volume Slider";
	case KEYS::SLIDER_3D:
		return "3D Slider";

	case KEYS::BLUE:
		return "Blue";
	case KEYS::YELLOW:
		return "Yellow";
	case KEYS::RED:
		return "Red";
	case KEYS::GREEN:
		return "Green";

	case KEYS::ZL:
		return "ZL";
	case KEYS::ZR:
		return "ZR";

	case KEYS::CSTICK_UP:
		return "CStick Up";
	case KEYS::CSTICK_DOWN:
		return "CStick Down";
	case KEYS::CSTICK_LEFT:
		return "CStick Left";
	case KEYS::CSTICK_RIGHT:
		return "CStick Right";

	case KEYS::CPAD_UP:
		return "CPad Up";
	case KEYS::CPAD_DOWN:
		return "CPad Down";
	case KEYS::CPAD_LEFT:
		return "CPad Left";
	case KEYS::CPAD_RIGHT:
		return "CPad Right";
	}
	return "ERROR";
}

}} // namespace D2K::Keys
