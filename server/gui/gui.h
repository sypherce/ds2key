#ifndef __GUI_H__
#define __GUI_H__

#include <windows.h>
#include <string>//std::string
using std::string;

namespace D2K {
	namespace GUI {
		extern void voidFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		extern HINSTANCE hThisInstance;
		extern LPSTR lpszArgument;
		extern int eventCounter;
		const int eventMax = 200;
		extern bool locked;

		void setFont(HWND hwnd);
		class Object {
			public:
				Object(int x, int y, int width, int height);
				~Object();
				virtual bool Attach(Object *parentObject);
				int getID();
				HWND getParentHWND();
				Object *getParent();
				void setVisible(bool visible = true);
			protected:
				HWND hwnd;               /* This is the handle for our window */
				Object *parent;
				int X;
				int Y;
				int Width;
				int Height;
				int ID;
		};

		class Label : public Object {
			public:
				Label(string text, int x, int y, int width, int height);
				virtual ~Label();
				bool Attach(Object *parentObject);
				string getText();
				void setText(string text);
				void setEnabled(bool enabled);
				long getStyle();
				long setStyle(long style);
			protected:
				string Text;
		};

		class Button : public Label {
			public:
				Button(string text, int x, int y, int width, int height);
				~Button();
				bool Attach(Object *parentObject);
		};

		class StatusBar : public Label {
			public:
				StatusBar(string text, int x, int y, int width, int height);
				~StatusBar();
				bool Attach(Object *parentObject);
				void SetText(string Text);
		};

		class TrayIcon : public Label {
			public:
				TrayIcon(string text);
				~TrayIcon();
				bool Attach(Object *parentObject);
				void SetText(string Text);
				void Delete();
			private:
				NOTIFYICONDATA niData;
		};

		class Menu : public Label {
			public:
				Menu();
				~Menu();
				void Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
				void Append(Object *object);
				bool Attach(Object *parentObject);
				bool Update();
		};

		class MenuPopUp : public Label {
			public:
				MenuPopUp(string text);
				~MenuPopUp();
				void Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
				void Append(Object *object);
				bool Attach(Object *parentObject);
		};

		class MenuItem : public Label {
			public:
				MenuItem(string text);
				~MenuItem();
				void Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
				void Append(Object *object);
				bool Attach(Object *parentObject);
		};

		class CheckButton : public Label {
			public:
				CheckButton(string text, int x, int y, int width, int height);
				~CheckButton();
				bool Attach(Object *parentObject);
				void setChecked(bool checked);
		};

		class RadioButton : public CheckButton {
			public:
				RadioButton(string text, int x, int y, int width, int height);
				~RadioButton();
				bool Attach(Object *parentObject);
		};

		class Edit : public Label {
			public:
				Edit(string text, int x, int y, int width, int height);
				~Edit();
				bool Attach(Object *parentObject);
		};

		class ComboButton : public Label {
			public:
				ComboButton(int x, int y, int width, int height);
				~ComboButton();
				bool Attach(Object *parentObject);
				void Append(string text);
				void setSelection(unsigned row);
				int getSelection();
		};

		class ListView : public Label {
			public:
				ListView(string text, int x, int y, int width, int height);
				~ListView();
				unsigned getColumnCount();
				void setHeaderVisible(bool visible);
				void InsertColumn(string Text, int i);
				string GetText(int row, int column);
				void SetText(string Text, int row, int column);
				void SetDoubleClick(void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
				void AutoSizeColumns();
				unsigned getSelection();
				void setSelection(unsigned row);
				void Append(string text);
				void Append(string text, string text2);
				bool Attach(Object *parentObject);
		};

		struct Event {
			D2K::GUI::Object *object;
			void (*function)(HWND, UINT, WPARAM, LPARAM);
			void (*function2)(HWND, UINT, WPARAM, LPARAM);
			//void (*function3)(HWND, UINT, WPARAM, LPARAM);
			//void (*function4)(HWND, UINT, WPARAM, LPARAM);
		};
		extern Event events[D2K::GUI::eventMax];

		void Setup();

		/*  Make the class name into a global variable  */

		extern MSG messages;            /* Here messages to the application are saved */

		/*  This function is called by the Windows function DispatchMessage()  */

		WPARAM GetMessages();

		LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		class Window : public Object {
			public:
				Window();
				~Window();
				void Append(Object *object, void (*function)(HWND, UINT, WPARAM, LPARAM));
				void Append(Object *object);
				bool Setup(HINSTANCE hThisInstance, string ClassName, string Title, int Width, int Height);
				void setVisible(int visible);
				void setVisible(bool visible = true);
				void CreateMainMenu();
				void setText(string text);
			private:
				string Text;
		};
	}
}
#endif // __GUI_H__
