#ifndef MENU_H
#define	MENU_H
#include <stdio.h>
#include <ncurses.h>
#include "machine_type.h"
#include "config.h"

typedef struct _tMenuItem
{
	char menutext[64];
	tUInt16 x;
	tUInt16 y;
	char hotkey;
} tMenuItem;

typedef struct _tMenu
{
	tMenuItem MenuItems[64];
	int menuitemnum;
	int menuitemactive;
} tMenu;

void clearmenu(tMenu* Menu);
void newMenuItem(tMenu* Menu,char* text,tUInt16 y,tUInt16 x,char hotkey,tBool active,tInt8* itemnum);
void printMenu(WINDOW* win,tMenu* Menu,tUInt16 offsy,tUInt16 offsx);
void MenuMoveLeft(tMenu* Menu);
void MenuMoveRight(tMenu* Menu);
void MenuMoveUp(tMenu* Menu);
void MenuMoveDown(tMenu* Menu);
void MenuSetActiveItem(tMenu* Menu,tInt8 itemnum);
tInt8 MenuInteract(WINDOW* win,tMenu* Menu,tInt16 offsy,tInt16 offsx);
#endif
