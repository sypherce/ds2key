#pragma once

#ifdef _NDS
#include <dswifi9.h>//Wifi_AccessPoint

namespace D2K{ namespace AP{

extern int g_count;
static Wifi_AccessPoint s_ap;
extern bool m_searching;
int Update();
void Init();
Wifi_AccessPoint* GetAP(int i);
void Print(int i);

}}//namespace D2K::AP
#endif
