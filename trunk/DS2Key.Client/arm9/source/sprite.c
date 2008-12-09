/*
DS2Key Client - An application to use your DS as a PC Gamepad
Copyright (C) 2008  Derrick (sypherce) Wirth

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <nds.h>

//a global copy of sprite attribute memory
SpriteEntry sprites[128];

//rotation attributes overlap so assign then to the same location
pSpriteRotation spriteRotations = (pSpriteRotation)sprites;
int spriteRotationAngle = 0;

//copy our sprite to object attribute memory
void updateOAM()
{
	DC_FlushRange(sprites, 128 * sizeof(SpriteEntry));
	dmaCopy(sprites, OAM, 128 * sizeof(SpriteEntry));
}

//turn off all the sprites
void initSprites()
{
	int i;
	for(i = 0; i < SPRITE_COUNT; i++)
	{
		sprites[i].attribute[0] = ATTR0_DISABLED;
		sprites[i].attribute[1] = 0;
		sprites[i].attribute[2] = 0;
	}

	updateOAM();
}
