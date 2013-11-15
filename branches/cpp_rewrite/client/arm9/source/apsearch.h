#ifndef __APSEARCH_H__
#define __APSEARCH_H__

#include <dswifi9.h>//Wifi_AccessPoint

namespace D2K {
	namespace AP {
		extern int Count;
		static Wifi_AccessPoint AP;
		extern bool Searching;
		int Update();
		void Init();
		Wifi_AccessPoint* GetAP(int i);
		void Print(int i);
	}
}
#endif//__APSEARCH_H__
