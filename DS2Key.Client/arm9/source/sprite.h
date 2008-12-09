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

//Normal Defines
#define SPRITE_MOUSE 0
#define SPRITE_SETTINGS	1
#define SPRITE_HIDDEN_X	SCREEN_WIDTH
#define SPRITE_HIDDEN_Y	SCREEN_HEIGHT
#define SPRITE_SETTINGS_X 0
#define SPRITE_SETTINGS_Y 0
#define SPRITE_MOUSE_X 244
#define SPRITE_MOUSE_Y 175

//Variables
extern SpriteEntry sprites[128];
extern pSpriteRotation spriteRotations;
extern int spriteRotationAngle;

//Functions
extern void updateOAM();
extern void initSprites();

#endif //__SPRITE_H__
