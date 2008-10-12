//IupMultiline Simple Example in C
//hows a multiline that ignores the treatment of the DEL key, canceling its effect.


#ifdef GUI
#include <stdio.h>
#include <iup.h>
#include <iupkey.h>
#include "gui.h"
extern char displayText[1024]; //#include "main.h"
extern void serverLoop(); //#include "main.h"

Ihandle *dlg, *ml;

int mlaction(Ihandle *self, int c, char *after)
{
	if(c)
		return IUP_IGNORE;
	else
		return IUP_DEFAULT;
}

void initGui()
{
	enum { hA, hB, hX, hY, hL, hR, hStart, hSelect, hUp, hDown, hLeft, hRight, hEnd };
	Ihandle *handleTxt[hEnd];
	char *charTxt[hEnd];
	charTxt[hA] = "tA";
	charTxt[hB] = "tB";
	charTxt[hX] = "tX";
	charTxt[hY] = "tY";
	charTxt[hL] = "tL";
	charTxt[hR] = "tR";
	charTxt[hStart] = "tStart";
	charTxt[hSelect] = "tSelect";
	charTxt[hUp] = "tUp";
	charTxt[hDown] = "tDown";
	charTxt[hLeft] = "tLeft";
	charTxt[hRight] = "tRight";

	IupOpen();

	ml = IupMultiLine(NULL);
	IupSetCallback(ml, "ACTION", (Icallback)mlaction);
	IupSetAttribute(ml, IUP_EXPAND, IUP_YES);
	IupSetAttribute(ml, IUP_VALUE, displayText);
	IupSetAttribute(ml, IUP_BORDER, IUP_YES);

	dlg = IupDialog
		  (
		IupVbox
		(
			IupHbox
			(
				/*IupVbox
				(
				handleTxt[hA] = IupText(charTxt[hA]),
				handleTxt[hB] = IupText(charTxt[hB]),
				NULL
				),
				IupVbox
				(
				IupSetAttributes(IupButton("z", "Save Config"), "SIZE=40"),
				IupSetAttributes(IupButton("1", "Open Config"), "SIZE=40"),
				NULL
				),
				IupVbox
				(
				IupSetAttributes(IupButton("2", "Save Config"), "SIZE=40"),
				IupSetAttributes(IupButton("3", "Open Config"), "SIZE=40"),
				NULL
				)
				),
				IupHbox
				(
				IupFill(),
				IupSetAttributes(IupButton("Save", "Save Config"), "SIZE=40"),
				IupSetAttributes(IupButton("Open", "Open Config"), "SIZE=40"),
				IupFill(),
				NULL
				),
				IupHbox
				(*/
				ml,
				NULL
			),
			NULL
		)
	);

	IupSetAttribute(dlg, IUP_TITLE, "DS2Key 1.0 Beta");
	IupSetAttribute(dlg, IUP_SIZE,  "QUARTERxQUARTER");
	IupSetFunction("IDLE_ACTION", (Icallback)serverLoop);

	IupShow(dlg);
}

void loopGui()
{
	IupMainLoop();
	//IupDestroy(dlg);
	IupClose();
}

#endif //GUI
