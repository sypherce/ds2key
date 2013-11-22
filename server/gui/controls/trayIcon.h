#ifndef __TRAYICON_H__
#define __TRAYICON_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"

namespace D2K {
	namespace GUI {
		class TrayIcon : public Label {
			public:
				TrayIcon(std::string text);
				~TrayIcon();
				bool Attach(Object *parentObject);
				void SetText(std::string Text);
				void Delete();
			private:
				NOTIFYICONDATA niData;
		};
	}
}
#endif//__TRAYICON_H__
