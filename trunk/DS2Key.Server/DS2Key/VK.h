#ifndef __VK_H__
#define __VK_H__

extern char *vkTable[0xff];
extern void initVKTable();
extern unsigned char getVKNumber(unsigned char *key);
extern unsigned char *getVKString(unsigned char key);

#endif //__VK_H__
