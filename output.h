#ifndef OUTPUT_H
#define	OUTPUT_H

#include <stdio.h>
#include <ncurses.h>
#include "buffers.h"
#include "machine_type.h"

typedef enum {COLOR_BRACKETS,COLOR_INPUT,COLOR_CURSOR,COLOR_TEXT,COLOR_HEXFIELD,COLOR_DIFF,COLOR_HEADLINE,COLOR_INFO,COLOR_HEADER,COLOR_MENUHOTKEY,COLOR_MENUNORMAL,COLOR_FRAME,COLOR_MENUACTIVE,COLOR_MENUHOTKEYACTIVE,COLOR_CURSORDIFF,UICOLORNUM} uicolors;
typedef struct _tColors
{
	short	fg;
	short	bg;
	int	attrs;
} tColors;
tColors	colors[UICOLORNUM];

void initcolors();
void colorpair(uicolors uicol,short fg,short bg,int attr);
void setcolor(WINDOW* win,uicolors col);
void printbuffersingle(WINDOW* win,tBuffer* hBuf1,tInt64 cursorpos1,tUInt64 firstpos1,tUInt8 windowfield);
void printbufferdiff(WINDOW* win,tBuffer* hBuf1,tBuffer* hBuf2,tInt64 cursorpos1,tInt64 cursorpos2);
void printmainmenu(WINDOW* win,tBool diffmode);
void drawframe(WINDOW* win,tInt8 y,tInt8 x,tInt8 h,tInt8 w,char* header);
void drawcenterframe(WINDOW* win,tInt8 h,tInt8 w,char* header);


tInt32 movepositions(tInt64* cursorpos,tInt64* firstpos,tInt64 maxbufsize,tInt32 chars,tInt32 lines,tInt32 pages,tBool diffmode);

#endif
