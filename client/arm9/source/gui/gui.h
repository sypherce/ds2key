#ifndef __GUI_H__
#define __GUI_H__

#include <nds/ndstypes.h>
#include <string>

namespace D2K {
	namespace GUI {
		class Rect {
			private:
				uint8_t X;
				uint8_t Y;
				uint8_t W;
				uint8_t H;
			public:
				Rect();
				Rect(uint8_t X, uint8_t Y, uint8_t W, uint8_t H);
				uint8_t getX();
				void setX(uint8_t X);
				uint8_t getY();
				void setY(uint8_t Y);
				uint8_t getW();
				void setW(uint8_t W);
				uint8_t getH();
				void setH(uint8_t H);
				uint16_t getX2();
				uint16_t getY2();
				bool PointIntersect(uint8_t X, uint8_t Y);
				~Rect();
		};

		extern void VoidFunction();
		extern u16 *Screen[2];
		extern bool Update;
		void setUpdate(bool value);
		bool isUpdated();
		void setPixel(uint8_t Screen, uint8_t X, uint8_t Y, uint16_t C);
		void ClearScreen(uint8_t Screen, uint16_t C);
		void DrawRect(uint8_t Screen, GUI::Rect *Rect, uint16_t C);
		void DrawFilledRect(uint8_t Screen, GUI::Rect *Rect, uint16_t C);
		void DrawLine(uint8_t Screen, std::string String, uint8_t X, uint8_t Y, uint16_t C);
		void DrawLetter(uint8_t Screen, char Letter, uint8_t X, uint8_t Y, uint16_t C);
		void DrawString(uint8_t Screen, std::string String, uint8_t X, uint8_t Y, uint16_t C);
		enum { 	colorBackground,
				colorLabelText, colorLabelBackground,
				colorButtonText, colorButtonBackground, colorButtonOutline,
				colorEditText, colorEditBackground, colorEditOutline,
				colorMax};
		extern uint16_t Color[colorMax];
	}
}

#endif//__GUI_H__
