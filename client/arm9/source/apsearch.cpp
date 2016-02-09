#include <nds.h>
#include <cstdio>
#include <dswifi9.h>
#include "apsearch.h"

namespace D2K {namespace AP {

int g_count = -1;
bool g_searching = false;
int Update()
{
	if(g_searching)
		g_count = Wifi_GetNumAP();
	else
		g_count = -1;

	return g_count;
}
void Init()
{
	Wifi_ScanMode();
	m_searching = true;
	Update();
}
Wifi_AccessPoint* GetAP(int i)
{
	Update();
	if(i <= g_count)
	{
		Wifi_GetAPData(i, &s_ap);
		return &s_ap;
	}
	return nullptr;
}
void Print(int i)
{
	GetAP(i);
	// display the name of the s_ap
	iprintf("%.29s\n  Wep:%s Sig:%i\n",
		s_ap.ssid,
		s_ap.flags & WFLAG_APDATA_WEP ? "Yes " : "No ",
		s_ap.rssi * 100 / 0xD0);
}

}}//namespace D2K::AP
