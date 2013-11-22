#include <string>	//std::string
#include "ipWindow.h"
#include "gui/button.h"
#include "gui/label.h"
#include "common/udp.h"

namespace D2K {
	namespace GUI {
		namespace IP {
			//private
			extern void button1Function();
			extern void button2Function();
			extern void button3Function();
			extern void button4Function();
			extern void button5Function();
			extern void button6Function();
			extern void button7Function();
			extern void button8Function();
			extern void button9Function();
			extern void button10Function();
			extern void button11Function();
			extern void button12Function();

			//public
			WindowClass *Window;
			Label *label1;
			Button *Button1;
			Button *Button2;
			Button *Button3;
			Button *Button4;
			Button *Button5;
			Button *Button6;
			Button *Button7;
			Button *Button8;
			Button *Button9;
			Button *Button10;
			Button *Button11;
			Button *Button12;

			WindowClass::WindowClass() : Window() {
				Screen = 0;
				int x = 16;
				int y = 24;
				int w = 64;
				int h = 24;
				int gap = 12;

				AppendObject(label1	= new Label(Screen, Rect(24,0+3,128,10), "IPs"));

				AppendObject(Button1 = new Button(Screen, Rect(x,y,w,h), "Command 1", &button1Function));
				y += h + gap;
				AppendObject(Button2 = new Button(Screen, Rect(x,y,w,h), "Command 2", &button2Function));
				y += h + gap;
				AppendObject(Button3 = new Button(Screen, Rect(x,y,w,h), "Command 3", &button3Function));
				y += h + gap;
				AppendObject(Button4 = new Button(Screen, Rect(x,y,w,h), "Command 4", &button4Function));

				x = x + w + gap;
				y = 24;

				AppendObject(Button5 = new Button(Screen, Rect(x,y,w,h), "Command 5", &button5Function));
				y += h + gap;
				AppendObject(Button6 = new Button(Screen, Rect(x,y,w,h), "Command 6", &button6Function));
				y += h + gap;
				AppendObject(Button7 = new Button(Screen, Rect(x,y,w,h), "Command 7", &button7Function));
				y += h + gap;
				AppendObject(Button8 = new Button(Screen, Rect(x,y,w,h), "Command 8", &button8Function));

				x = x + w + gap;
				y = 24;

				AppendObject(Button9 = new Button(Screen, Rect(x,y,w,h), "Command 9", &button9Function));
				y += h + gap;
				AppendObject(Button10 = new Button(Screen, Rect(x,y,w,h), "Command 10", &button10Function));
				y += h + gap;
				AppendObject(Button11 = new Button(Screen, Rect(x,y,w,h), "Command 11", &button11Function));
				y += h + gap;
				AppendObject(Button12 = new Button(Screen, Rect(x,y,w,h), "Command 12", &button12Function));
			}
			WindowClass::~WindowClass() { }
			bool WindowClass::Update() {
				return Window::Update();
			}

			void button1Function() {
				Core::UDP->SendCommand(0);
			}
			void button2Function() {
				Core::UDP->SendCommand(1);
			}
			void button3Function() {
				Core::UDP->SendCommand(2);
			}
			void button4Function() {
				Core::UDP->SendCommand(3);
			}
			void button5Function() {
				Core::UDP->SendCommand(4);
			}
			void button6Function() {
				Core::UDP->SendCommand(5);
			}
			void button7Function() {
				Core::UDP->SendCommand(6);
			}
			void button8Function() {
				Core::UDP->SendCommand(7);
			}
			void button9Function() {
				Core::UDP->SendCommand(8);
			}
			void button10Function() {
				Core::UDP->SendCommand(9);
			}
			void button11Function() {
				Core::UDP->SendCommand(10);
			}
			void button12Function() {
				Core::UDP->SendCommand(11);
			}
		}
	}
}
