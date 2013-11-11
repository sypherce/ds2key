#include <string>	//std::string
#include "commandWindow.h"
#include "../ds2key.h"
#include "../config.h"
#include "../system.h"

namespace D2K {
	namespace GUI {
		namespace Command {
			//private
			extern void button1Function();
			extern void button2Function();
			extern void button3Function();
			extern void button4Function();

			//public
			WindowClass *Window;
			Label *label1;
			Button *Button1;
			Button *Button2;
			Button *Button3;
			Button *Button4;

			WindowClass::WindowClass() : Window() {
				Screen = 0;

				AppendObject(label1	= new Label(Screen, Rect(24,0+3,128,10), "Commands"));

				AppendObject(Button1 = new Button(Screen, Rect(9,50,10,10), "Command 1", &button1Function));
				AppendObject(Button2 = new Button(Screen, Rect(9,65,10,10), "Command 2", &button2Function));
				AppendObject(Button3 = new Button(Screen, Rect(9,80,10,10), "Command 3", &button3Function));
				AppendObject(Button4 = new Button(Screen, Rect(9,95,10,10), "Command 4", &button4Function));
			}
			WindowClass::~WindowClass() { }

			void button1Function() {
				DS2Key::SendCommand(1);
			}
			void button2Function() {
				DS2Key::SendCommand(2);
			}
			void button3Function() {
				DS2Key::SendCommand(3);
			}
			void button4Function() {
				DS2Key::SendCommand(4);
			}
		}
	}
}
