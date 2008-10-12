#ifndef __GUI_H__
#define __GUI_H__

#ifdef GUI
#include <iup.h>

Ihandle *dlg, *ml;

int mlaction(Ihandle *self, int c, char *after);
void initGui();
void loopGui();

#endif //GUI

#endif //__GUI_H__
