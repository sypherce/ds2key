#include <nds.h>
#include <stdio.h>
#include <dswifi9.h>
#include "apsearch.h"

namespace D2K {
	namespace AP {
		int Count = -1;
		bool Searching = false;
		int Update() {
			if(Searching) {
				Count = Wifi_GetNumAP();
			}
			else {
				Count = -1;
			}
			return Count;
		}
		void Init() {
			Wifi_ScanMode();
			Searching = true;
			Update();
		}
		Wifi_AccessPoint* GetAP(int i) {
			Update();
			if(i <= Count)
			{
				Wifi_GetAPData(i, &AP);
				return &AP;
			}
			return (Wifi_AccessPoint*)NULL;
		}
		void Print(int i) {
			GetAP(i);
			// display the name of the AP
			iprintf("%.29s\n  Wep:%s Sig:%i\n",
				AP.ssid,
				AP.flags & WFLAG_APDATA_WEP ? "Yes " : "No ",
				AP.rssi * 100 / 0xD0);
		}
	}
}
