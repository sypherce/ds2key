/*
	Joystick emulation for windows
*/

#include <iostream>//std::cout, std::clog
#include <windows.h>
#include <winioctl.h>
#include <sstream>//ostringstream
#include "PPJIoctl.h"
#include "PPJoy.h"

namespace D2K {
	namespace Core {
		namespace C {
			PPJoy::PPJoy(int device) {
				//Initialise the IOCTL data structure
				JoyState.Signature = JOYSTICK_STATE_V1;
				JoyState.NumAnalog = NUM_ANALOG;
				JoyState.NumDigital = NUM_DIGITAL;

				//Reset axes & buttons
				ResetAxes();
				ResetButtons();

				//Make sure we're using a valid device
				if(device < 0 || device > 15) {
					handle = INVALID_HANDLE_VALUE;
					std::clog << "Error (PPJoy::PPJoy) Device number " << device << " out of range\n";

					return;
				}

				std::ostringstream DevName;
				DevName << "\\\\.\\PPJoyIOCTL" << device + 1;

				//Open a handle to the control device for the virtual joystick.
				handle = CreateFile(DevName.str().c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

				//Make sure we could open the device!
				if(handle == INVALID_HANDLE_VALUE) {
					std::clog << "Error (CreateFile) #" << (int)GetLastError() << "\n"
							<< "Failed to open device: " <<	DevName.str() << "\n";

					return;
				}

				Update();
			}

			int PPJoy::Update() {
				//Send request to PPJoy for processing.
				//Currently there is no Return Code from PPJoy, this may be added at a
				//later stage. So we pass a 0 byte output buffer.
				if(handle == INVALID_HANDLE_VALUE)
					return (int)INVALID_HANDLE_VALUE;
				DWORD RetSize;
				if(!DeviceIoControl(handle, IOCTL_PPORTJOY_SET_STATE, &JoyState, sizeof(JoyState), NULL, 0, &RetSize, NULL)) {
					DWORD rc = GetLastError();
					if(rc == 2) {
						std::clog << "Error (DeviceIoControl) Handle closed\n";
					}
					else {
						std::clog << "Error (DeviceIoControl) #" << (int)rc << "\n";
					}

					return rc;
				}

				return 0;
			}

			PPJoy::~PPJoy() {
				if(CloseHandle(handle) != 0) {//handle wasn't initialized
					if(handle != INVALID_HANDLE_VALUE) {	
						std::clog << "Error (CloseHandle) #" << (int)GetLastError() << "\n";
					}
				}
			}

			void PPJoy::ResetAxes() {
				for(int i = 0; i < JoyState.NumAnalog; i++)
					JoyState.Digital[i] = (PPJOY_AXIS_MIN + PPJOY_AXIS_MAX) / 2;
			}

			void PPJoy::ResetButtons() {
				for(int i = 0; i < JoyState.NumDigital; i++)
					JoyState.Digital[i] = 0;
			}

			void PPJoy::SetButton(char button, char value) {
				if(button >= JoyState.NumDigital) {
					std::clog << "Error (PPJoy::SetButton): button #" << button << " out of range\n";
					return;
				}
				JoyState.Digital[(int)button] = value;
			}

			void PPJoy::SetAxisPercent(char axis, char value) {
				if(axis >= JoyState.NumAnalog) {
					std::clog << "Error (PPJoy::SetAxisPercent): axis #" << axis << " out of range\n";
					return;
				}
				int range = PPJOY_AXIS_MAX - PPJOY_AXIS_MIN;
				int adjValue = value * range / 100 + PPJOY_AXIS_MIN;
				JoyState.Analog[(int)axis] = adjValue;
			}

			int PPJoy::GetButton(char button) {
				if(button >= JoyState.NumDigital) {
					std::clog << "Error (PPJoy::GetButton): button #" << button << " out of range\n";
					return 0;
				}

				return (int)(JoyState.Digital[(int)button]);
			}

			int PPJoy::GetAxisPercent(char axis) {
				if(axis >= JoyState.NumAnalog) {
					std::clog << "Error (PPJoy::GetAxisPercent): axis #" << axis << " out of range\n";
					return (PPJOY_AXIS_MIN + PPJOY_AXIS_MAX) / 2;
				}
				int range = PPJOY_AXIS_MAX - PPJOY_AXIS_MIN;
				int value = (int)(JoyState.Analog[(int)axis]);
				return (value - PPJOY_AXIS_MIN) * 100 / range;
			}
		}
	}
}
