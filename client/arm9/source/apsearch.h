#ifndef __APSEARCH_H__
#define __APSEARCH_H__

#include <dswifi9.h>

namespace D2K {
	namespace AP {
		extern int count;
		static Wifi_AccessPoint ap;
		extern bool searching;
		int Update();
		void Init();
		Wifi_AccessPoint* getAP(int i);
		void Print(int i);
	}
}
#endif//__APSEARCH_H__
