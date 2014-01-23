#ifndef __PPJOY_H__
#define __PPJOY_H__

#include <windef.h>

namespace D2K {
	namespace Input {
		#define MAX_JOYSTICKS 16
		#define	NUM_ANALOG	8		//Number of analog values which we will provide
		#define	NUM_DIGITAL	20		//Number of digital values which we will provide

		#pragma pack(push,1)		//All fields in structure must be byte aligned.
		typedef struct JOYSTICK_STATE {
			unsigned long	Signature;				//Signature to identify packet to PPJoy IOCTL
			char			NumAnalog;				//Num of analog values we pass
			long			Analog[NUM_ANALOG];		//Analog values
			char			NumDigital;				//Num of digital values we pass
			char			Digital[NUM_DIGITAL];	//Digital values
		} JOYSTICK_STATE;
		#pragma pack(pop)

		class PPJoy {
			public:
				PPJoy(int device);
				~PPJoy();
				int Update();
				void SetButton(char button, char value);
				void SetAxisPercent(char axis, char value);
				int GetButton(char button);
				int GetAxisPercent(char axis);

			private:
				HANDLE handle;
				JOYSTICK_STATE JoyState;
				void ResetAxes();
				void ResetButtons();
		};
	}
}
#endif//__PPJOY_H__
