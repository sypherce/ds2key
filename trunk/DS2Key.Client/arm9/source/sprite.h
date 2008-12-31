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
#ifndef __SPRITE_H__
#define __SPRITE_H__

//function defines
#define showHideArrowSprite(a) oamSet(&oamSub, 0, 244, 175, 0, 0, SpriteSize_16x16, SpriteColorFormat_16Color, arrowSprite, -1, false, a)
#define showHideSettingsSprite(a)  oamSet(&oamSub, 1, -8, -8, 0, 0, SpriteSize_16x16, SpriteColorFormat_16Color, settingsSprite, 0, true, a)
#define showHideSprites(a) \
	showHideArrowSprite(a); \
	showHideSettingsSprite(a);
#define showArrowSprite() showHideArrowSprite(false)
#define hideArrowSprite() showHideArrowSprite(true)
#define showSettingsSprite() showHideSettingsSprite(false)
#define hideSettingsSprite() showHideSettingsSprite(true)
#define showSprites() showHideSprites(false)
#define hideSprites() showHideSprites(true)

//variables
extern uint32 spriteRotation;
extern u16* arrowSprite;
extern u16* settingsSprite;

//functions
extern void initSprites();
extern void updateSprites();

#endif//__SPRITE_H__
