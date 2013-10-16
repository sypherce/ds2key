/*
	Joystick emulation for windows
*/

#include <stdio.h>//printf
#include <windows.h>
#include <winioctl.h>
#include <sstream>//ostringstream
#include "PPJioctl.h"
#include "PPJoy.h"

namespace D2K {
	namespace Core {
		namespace C {
			PPJoy::PPJoy(int device) {
				if(device < 0 || device > 15) {
					h = INVALID_HANDLE_VALUE;
					printf("Device number %i out of range.\n", device);

					return;
				}
				std::ostringstream DevName;
				DevName << "\\\\.\\PPJoyIOCTL" << device + 1;

				/* Open a handle to the control device for the first virtual joystick. */
				/* Virtual joystick devices are names PPJoyIOCTL1 to PPJoyIOCTL16. */
				h = CreateFile(DevName.str().c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

				/* Make sure we could open the device! */
				if(h == INVALID_HANDLE_VALUE) {
					printf("CreateFile failed with error code %d trying to open %s device\n", (int)GetLastError(), DevName.str().c_str());

					return;
				}

				/* Initialise the IOCTL data structure */
				JoyState.Signature = JOYSTICK_STATE_V1;
				JoyState.NumAnalog = NUM_ANALOG;	/* Number of analog values */
				JoyState.NumDigital = NUM_DIGITAL;	/* Number of digital values */

				ResetAxes();
				ResetButtons();
				Update();
			}

			int PPJoy::Update() {
				/* Send request to PPJoy for processing. */
				/* Currently there is no Return Code from PPJoy, this may be added at a */
				/* later stage. So we pass a 0 byte output buffer.                      */
				if(!DeviceIoControl(h, IOCTL_PPORTJOY_SET_STATE, &JoyState, sizeof(JoyState), NULL, 0, &RetSize, NULL)) {
					rc = GetLastError();
					if(rc == 2) {
						printf("Underlying joystick device deleted. Exiting read loop\n");
					}
					printf("DeviceIoControl error %d\n", (int)rc);

					return rc;
				}

				return 0;
			}

			PPJoy::~PPJoy() {
				CloseHandle(h);
			}

			void PPJoy::ResetAxes() {
				JoyState.Analog[0] = JoyState.Analog[1] =
				JoyState.Analog[2] = JoyState.Analog[3] =
				JoyState.Analog[4] = JoyState.Analog[5] =
				JoyState.Analog[6] = JoyState.Analog[7] = (PPJOY_AXIS_MIN + PPJOY_AXIS_MAX) / 2;
			}

			void PPJoy::ResetButtons() {
				memset(JoyState.Digital, 0, sizeof(JoyState.Digital));
			}

			void PPJoy::SetButton(char button, char value) {
				JoyState.Digital[(int)button] = value;
			}

			void PPJoy::SetAxisPercent(char axis, char value) {
				int range = PPJOY_AXIS_MAX - PPJOY_AXIS_MIN;
				int adjValue = value * range / 100 + PPJOY_AXIS_MIN;
				JoyState.Analog[(int)axis] = adjValue;
			}

			int PPJoy::GetButton(char button) {
				return (int)(JoyState.Digital[(int)button]);
			}

			int PPJoy::GetAxisPercent(char axis) {
				int range = PPJOY_AXIS_MAX - PPJOY_AXIS_MIN;
				int value = (int)(JoyState.Analog[(int)axis]);
				return (value - PPJOY_AXIS_MIN) * 100 / range;
			}
		}
	}
}
