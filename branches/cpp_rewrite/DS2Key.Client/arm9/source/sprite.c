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
#include "spritesImage.h"
#include "sprite.h"

//variables
uint32 spriteRotation = 0;
u16* arrowSprite = (u16*)NULL;
u16* settingsSprite = (u16*)NULL;

//functions
void initSprites()
{
    oamInit(&oamSub, SpriteMapping_1D_32, false);

    arrowSprite = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_16Color);
    settingsSprite = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_16Color);

    dmaCopy(spritesImagePal, SPRITE_PALETTE_SUB, spritesImagePalLen);
    dmaCopy(spritesImageTiles, arrowSprite, 128);
    dmaCopy(&spritesImageTiles[32], settingsSprite, 128);
}

void updateSprites()
{
    spriteRotation += 128;
    oamRotateScale(&oamSub, 0, spriteRotation, (1<<8), (1<<8));
    oamUpdate(&oamSub);
}
