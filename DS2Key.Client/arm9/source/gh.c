#include <nds.h>
#include "gh.h"

unsigned short int keysGH = 0;
unsigned short int oldKeysGH = 0;

unsigned short int keysDownGH(unsigned char color)
{
	if(color == GH_BLUE)
		return GH_Blue(GH_keysDown);
	else if(color == GH_YELLOW)
		return GH_Yellow(GH_keysDown);
	else if(color == GH_RED)
		return GH_Red(GH_keysDown);
	else if(color == GH_GREEN)
		return GH_Green(GH_keysDown);
	else if(color == GH_ANY)
		return GH_Any(GH_keysDown);	

	return 0;
}

unsigned short int keysHeldGH(unsigned char color)
{
	if(color == GH_BLUE)
		return GH_Blue(GH_keysHeld);
	else if(color == GH_YELLOW)
		return GH_Yellow(GH_keysHeld);
	else if(color == GH_RED)
		return GH_Red(GH_keysHeld);
	else if(color == GH_GREEN)
		return GH_Green(GH_keysHeld);
	else if(color == GH_ANY)
		return GH_Any(GH_keysHeld);		

	return 0;
}
unsigned short int keysUpGH(unsigned char color)
{
	if(color == GH_BLUE)
		return GH_Blue(GH_keysUp);
	else if(color == GH_YELLOW)
		return GH_Yellow(GH_keysUp);
	else if(color == GH_RED)
		return GH_Red(GH_keysUp);
	else if(color == GH_GREEN)
		return GH_Green(GH_keysUp);
	else if(color == GH_ANY)
		return GH_Any(GH_keysUp);		

	return 0;
}

void initGHPad(void)
{
	sysSetBusOwners(BUS_OWNER_ARM9, BUS_OWNER_ARM9);
	REG_EXMEMCNT &= ~0x0880;
	REG_EXMEMCNT |= 0x3;
}

void updateGHPad(void)
{
	if(insertedGHPad)
	{
		oldKeysGH = keysGH;
		keysGH = ~SRAM[0x0];
	}
	else
	{
		oldKeysGH = keysGH;
	}
}