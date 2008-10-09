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
