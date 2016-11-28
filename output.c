#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "machine_type.h"
#include "output.h"
#include "buffers.h"

int attrs[UICOLORNUM];
#if 0 // openbsd/linux
void initcolors()
{
	start_color();
	init_pair(1	,COLOR_BLACK,COLOR_BLACK);	attrs[COLOR_BRACKETS]		=COLOR_PAIR(1)+A_BOLD;
	init_pair(2	,COLOR_BLACK,COLOR_WHITE);	attrs[COLOR_INPUT]		=COLOR_PAIR(2);
	init_pair(3	,COLOR_WHITE,COLOR_BLACK);	attrs[COLOR_CURSOR]		=COLOR_PAIR(3);
	init_pair(4	,COLOR_WHITE,COLOR_BLACK);	attrs[COLOR_TEXT]		=COLOR_PAIR(4)+A_BOLD;
	init_pair(5	,COLOR_BLACK,COLOR_BLACK);	attrs[COLOR_HEXFIELD]		=COLOR_PAIR(5)+A_BOLD;	
	init_pair(6	,COLOR_YELLOW,COLOR_BLACK);	attrs[COLOR_DIFF]		=COLOR_PAIR(6)+A_BOLD;
	init_pair(7	,COLOR_BLUE,COLOR_BLACK);	attrs[COLOR_HEADLINE]		=COLOR_PAIR(7)+A_BOLD;
	init_pair(8	,COLOR_WHITE,COLOR_BLACK);	attrs[COLOR_INFO]		=COLOR_PAIR(8)+A_BOLD;
	init_pair(9	,COLOR_BLACK,COLOR_CYAN);	attrs[COLOR_HEADER]		=COLOR_PAIR(9);
	init_pair(10	,COLOR_YELLOW,COLOR_BLACK);	attrs[COLOR_MENUHOTKEY]		=COLOR_PAIR(10)+A_BOLD;
//	init_pair(11	,COLOR_BLACK,COLOR_CYAN);	attrs[COLOR_MENUNORMAL]		=COLOR_PAIR(11);
	init_pair(11	,COLOR_CYAN,COLOR_BLACK);	attrs[COLOR_MENUNORMAL]		=COLOR_PAIR(11)+A_BOLD;
	init_pair(12	,COLOR_BLUE,COLOR_BLACK);	attrs[COLOR_FRAME]		=COLOR_PAIR(12)+A_BOLD;
	init_pair(13	,COLOR_BLACK,COLOR_CYAN);	attrs[COLOR_MENUACTIVE]		=COLOR_PAIR(13);
	init_pair(14	,COLOR_YELLOW,COLOR_CYAN);	attrs[COLOR_MENUHOTKEYACTIVE]	=COLOR_PAIR(14)+A_BOLD;

}
#endif

#if 1 // mac os x
void initcolors()
{
//	init_pair(COLOR_BRACKETS	,COLOR_WHITE,COLOR_BLACK);	attrs[COLOR_BRACKETS]		=COLOR_PAIR(COLOR_BRACKETS);
//	init_pair(COLOR_INPUT		,COLOR_BLACK,COLOR_WHITE);	attrs[COLOR_INPUT]		=COLOR_PAIR(COLOR_INPUT);
//	init_pair(COLOR_CURSOR		,COLOR_WHITE,COLOR_BLACK);	attrs[COLOR_CURSOR]		=COLOR_PAIR(COLOR_CURSOR);
	//init_pair(COLOR_TEXT		,COLOR_WHITE,COLOR_BLACK);	attrs[COLOR_TEXT]		=COLOR_PAIR(COLOR_TEXT)+A_BOLD;
	//init_pair(COLOR_HEXFIELD	,COLOR_WHITE,COLOR_BLACK);	attrs[COLOR_HEXFIELD]		=COLOR_PAIR(COLOR_HEXFIELD);
	//init_pair(COLOR_DIFF		,COLOR_YELLOW,COLOR_BLACK);	attrs[COLOR_DIFF]		=COLOR_PAIR(COLOR_DIFF)+A_BOLD;
	//init_pair(COLOR_HEADLINE	,COLOR_BLUE,COLOR_BLACK);	attrs[COLOR_HEADLINE]		=COLOR_PAIR(COLOR_HEADLINE)+A_BOLD;
//	init_pair(COLOR_INFO		,COLOR_WHITE,COLOR_BLACK);	attrs[COLOR_INFO]		=COLOR_PAIR(COLOR_INFO)+A_BOLD;
//	init_pair(COLOR_HEADER		,COLOR_BLACK,COLOR_CYAN);	attrs[COLOR_HEADER]		=COLOR_PAIR(COLOR_HEADER);
//	init_pair(COLOR_MENUHOTKEY	,COLOR_YELLOW,COLOR_BLACK);	attrs[COLOR_MENUHOTKEY]		=COLOR_PAIR(COLOR_MENUHOTKEY)+A_BOLD;
//	init_pair(COLOR_MENUNORMAL	,COLOR_CYAN,COLOR_BLACK);	attrs[COLOR_MENUNORMAL]		=COLOR_PAIR(COLOR_MENUNORMAL)+A_BOLD;
//	init_pair(COLOR_FRAME		,COLOR_BLUE,COLOR_BLACK);	attrs[COLOR_FRAME]		=COLOR_PAIR(COLOR_FRAME)+A_BOLD;
//	init_pair(COLOR_MENUACTIVE	,COLOR_BLACK,COLOR_CYAN);	attrs[COLOR_MENUACTIVE]		=COLOR_PAIR(COLOR_MENUACTIVE);
//	init_pair(COLOR_MENUHOTKEYACTIVE,COLOR_YELLOW,COLOR_CYAN);	attrs[COLOR_MENUHOTKEYACTIVE]	=COLOR_PAIR(COLOR_MENUHOTKEYACTIVE)+A_BOLD;

	colors[COLOR_BRACKETS].fg	=COLOR_WHITE;	colors[COLOR_BRACKETS].bg	=COLOR_BLACK;	colors[COLOR_BRACKETS].attrs	=0;
	colors[COLOR_INPUT].fg		=COLOR_BLACK;	colors[COLOR_INPUT].bg		=COLOR_WHITE;	colors[COLOR_INPUT].attrs	=0;
	colors[COLOR_CURSOR].fg		=COLOR_WHITE;	colors[COLOR_CURSOR].bg		=COLOR_BLACK;	colors[COLOR_CURSOR].attrs	=0;
	colors[COLOR_TEXT].fg		=COLOR_WHITE;	colors[COLOR_TEXT].bg		=COLOR_BLACK;	colors[COLOR_TEXT].attrs	=A_BOLD;
	colors[COLOR_HEXFIELD].fg	=COLOR_WHITE;	colors[COLOR_HEXFIELD].bg	=COLOR_BLACK;	colors[COLOR_HEXFIELD].attrs	=0;
	colors[COLOR_DIFF].fg		=COLOR_YELLOW;	colors[COLOR_DIFF].bg		=COLOR_BLACK;	colors[COLOR_DIFF].attrs	=A_BOLD;
	colors[COLOR_HEADLINE].fg	=COLOR_BLUE;	colors[COLOR_HEADLINE].bg	=COLOR_BLACK;	colors[COLOR_HEADLINE].attrs	=A_BOLD;
	colors[COLOR_INFO].fg		=COLOR_WHITE;	colors[COLOR_INFO].bg		=COLOR_BLACK;	colors[COLOR_INFO].attrs	=A_BOLD;
	colors[COLOR_HEADER].fg		=COLOR_BLACK;	colors[COLOR_HEADER].bg		=COLOR_CYAN;	colors[COLOR_HEADER].attrs	=0;
	colors[COLOR_MENUHOTKEY].fg	=COLOR_YELLOW;	colors[COLOR_MENUHOTKEY].bg	=COLOR_BLACK;	colors[COLOR_MENUHOTKEY].attrs	=A_BOLD;
	colors[COLOR_MENUNORMAL].fg	=COLOR_CYAN;	colors[COLOR_MENUNORMAL].bg	=COLOR_BLACK;	colors[COLOR_MENUNORMAL].attrs	=A_BOLD;
	colors[COLOR_FRAME].fg		=COLOR_BLUE;	colors[COLOR_FRAME].bg		=COLOR_BLACK;	colors[COLOR_FRAME].attrs	=A_BOLD;
	colors[COLOR_MENUACTIVE].fg	=COLOR_BLACK;	colors[COLOR_MENUACTIVE].bg	=COLOR_CYAN;	colors[COLOR_MENUACTIVE].attrs	=0;
	colors[COLOR_MENUHOTKEYACTIVE].fg=COLOR_YELLOW;	colors[COLOR_MENUHOTKEYACTIVE].bg=COLOR_CYAN;	colors[COLOR_MENUHOTKEYACTIVE].attrs=A_BOLD;	
	
//	init_pair(11	,COLOR_BLACK,COLOR_CYAN);	attrs[COLOR_MENUNORMAL]		=COLOR_PAIR(11);
}
#endif
void colorpair(uicolors uicol,short fg,short bg,int attr)
{
	//init_pair(uicol,fg,bg);attrs[uicol]=COLOR_PAIR(uicol)+attr;
	colors[uicol].fg=fg;
	colors[uicol].bg=bg;
	colors[uicol].attrs=attr;
}
void pairsinit()
{
	int i;
	start_color();
	for (i=0;i<UICOLORNUM;i++)
	{
		init_pair(i,colors[i].fg,colors[i].bg);attrs[i]=COLOR_PAIR(i)+colors[i].attrs;
	}
}
void setcolor(WINDOW* win,uicolors col)
{
	int num;
	wattrset(win,attrs[col]);
}

void drawframe(WINDOW* win,tInt8 y,tInt8 x,tInt8 h,tInt8 w,char* header)
{
	int i;
	int j;
	
	setcolor(win,COLOR_FRAME);
	wmove(win,y,x);
	waddch(win,ACS_ULCORNER);
	for (i=0;i<w-2;i++) waddch(win,ACS_HLINE);
	waddch(win,ACS_URCORNER);
	for (j=1;j<h;j++)
	{
		wmove(win,y+j,x);
		waddch(win,ACS_VLINE);
		for (i=0;i<w-2;i++) waddch(win,' ');
		waddch(win,ACS_VLINE);
	}
	wmove(win,y+h,x);
	waddch(win,ACS_LLCORNER);
	for (i=0;i<w-2;i++) waddch(win,ACS_HLINE);
	waddch(win,ACS_LRCORNER);
	if (header)
	{
		setcolor(win,COLOR_BRACKETS);
		mvwprintw(win,y,x+1,"[");
		mvwprintw(win,y,x+1+strlen(header)+1,"]");
		setcolor(win,COLOR_HEADER);
		mvwprintw(win,y,x+2,"%s",header);
	}
}
void drawcenterframe(WINDOW* win,tInt8 h,tInt8 w,char* header)
{
	drawframe(win,LINES/2-h/2,COLS/2-w/2,h,w,header);
}
void printbuffersingle(WINDOW* win,tBuffer* hBuf1,tInt64 cursorpos1,tUInt64 firstpos1,tUInt8 windowfield)
{
	int i;
	int j;
	int k;
	int addrwidth;
	int bytesperline;
	tInt32 intpos1;
	tInt32 charcnt;



	addrwidth=(hBuf1->bufsize>0xffffffffull)?16:8;
	bytesperline=(COLS-(addrwidth+3+3))*8/(8*3+8+1);			// this many bytes can be printed in one line. every 8 bytes there is an extra space in the hex field.
	
	setcolor(win,COLOR_HEADLINE);
	wmove(win,0,0);
	for (i=0;i<COLS;i++)
	{
		waddch(win,ACS_HLINE);
	}
	setcolor(win,COLOR_BRACKETS);
	mvwprintw(win,0,COLS-3-strlen(hBuf1->filename),"[");
	mvwprintw(win,0,COLS-2,"]");
	if (addrwidth==16)
	{
		mvwprintw(win,0,1,"[                /                ]");
		setcolor(win,COLOR_INFO);
		mvwprintw(win,0,2,"%16llX",cursorpos1);
		mvwprintw(win,0,19,"%16llX",hBuf1->bufsize);
	} else {
		mvwprintw(win,0,1,"[        /        ]");
		setcolor(win,COLOR_INFO);
		mvwprintw(win,0,2,"%8llX",cursorpos1);
		mvwprintw(win,0,11,"%8llX",hBuf1->bufsize);
	}
	setcolor(win,COLOR_HEADER);
	mvwprintw(win,0,COLS-2-strlen(hBuf1->filename),"%s",hBuf1->filename);
	intpos1=getbufferidx(hBuf1,firstpos1);
	if (intpos1>=0)
	{
		charcnt=0;
		for (i=0;i<LINES-2;i++)
		{
			setcolor(win,COLOR_HEXFIELD);
			if (addrwidth==8)	mvwprintw(win,i+1,0, "% 8llX    ",firstpos1);
			else			mvwprintw(win,i+1,0,"% 16llX    ",firstpos1);

			mvwprintw(win,i+1,COLS-bytesperline-5,"      ");
			for (j=0;j<bytesperline;j++)
			{
				tInt16	c;
				if ((intpos1+j)<hBuf1->filesize) c=hBuf1->dataptr[intpos1+j];
				else	c=-1;
				for (k=0;k<hBuf1->changesnum;k++)
				{
					if (hBuf1->changes[k].pos==(firstpos1+j))	
					{
						setcolor(win,COLOR_DIFF);
						c=hBuf1->changes[k].after;
					}
				}	
				if (cursorpos1==(firstpos1+j) && windowfield==0) setcolor(win,COLOR_INPUT);
				//  TODO: setcolor(hBuf1->dataptr[intpos1]!=buf2->dataptr[intpos2]?COLOR_DIFF:COLOR_HEXFIELD);
				if (c>=0)
				{
					if (hBuf1->nibble && hBuf1->changepos==(firstpos1+j))
					
						mvwprintw(win,i+1,addrwidth+3+j*3+j/8,"%01x ",hBuf1->nexthex&0xf);
					else
						mvwprintw(win,i+1,addrwidth+3+j*3+j/8,"%02x",c&0xff);
				}
				else
					mvwprintw(win,i+1,addrwidth+3+j*3+j/8,"  ");

				setcolor(win,COLOR_HEXFIELD);
				mvwprintw(win,i+1,addrwidth+5+j*3+j/8,"   ");
			}
			for (j=0;j<bytesperline;j++)
			{
				unsigned char c;
				if (intpos1<hBuf1->filesize)
					c=hBuf1->dataptr[intpos1];
				else c=' ';
				for (k=0;k<hBuf1->changesnum;k++)
				{
					if (hBuf1->changes[k].pos==(firstpos1))
					{
						setcolor(win,COLOR_DIFF);
						c=hBuf1->changes[k].after;
					}
				}	
				if (cursorpos1==firstpos1 && windowfield==1) setcolor(win,COLOR_INPUT);
				//  TODO: setcolor(buf1->dataptr[intpos1]!=buf2->dataptr[intpos2]?COLOR_DIFF:COLOR_HEXFIELD);
				mvwprintw(win,i+1,COLS-bytesperline+j,"%c",(c>=32 && c<127)?c:'.');
				intpos1++;
				firstpos1++;
 				setcolor(win,COLOR_HEXFIELD);
			}
		}
	}
}
void printbufferdiff(WINDOW* win,tBuffer* hBuf1,tBuffer* hBuf2,tInt64 cursorpos1,tInt64 cursorpos2)
{
	int i;
	int j;
	int k;
	int addrwidth;
	int bytesperline;
	tInt32 intpos1;
	tInt32 intpos2;
	tInt32 charcnt;



	addrwidth=(hBuf1->bufsize>0xffffffffull || hBuf2->bufsize>0xffffffffull)?16:8;
	bytesperline=(COLS-(addrwidth+3+3))*8/(8*3+8+1);			// this many bytes can be printed in one line. every 8 bytes there is an extra space in the hex field.
	
	setcolor(win,COLOR_HEADLINE);
	wmove(win,0,0);
	for (i=0;i<COLS;i++)
	{
		waddch(win,ACS_HLINE);
	}
	wmove(win,LINES/2,0);
	for (i=0;i<COLS;i++)
	{
		waddch(win,ACS_HLINE);
	}
	setcolor(win,COLOR_BRACKETS);
	mvwprintw(win,0,COLS-3-strlen(hBuf1->filename),"[");
	mvwprintw(win,0,COLS-2,"]");
	mvwprintw(win,LINES/2,COLS-3-strlen(hBuf2->filename),"[");
	mvwprintw(win,LINES/2,COLS-2,"]");
	if (addrwidth==16)
	{
		mvwprintw(win,0,1,"[                /                ]");
		mvwprintw(win,LINES/2,1,"[                /                ]");
		setcolor(win,COLOR_INFO);
		mvwprintw(win,0,2,"%16llX",cursorpos1);
		mvwprintw(win,0,19,"%16llX",hBuf1->bufsize);
		mvwprintw(win,LINES/2,2,"%16llX",cursorpos1);
		mvwprintw(win,LINES/2,19,"%16llX",hBuf1->bufsize);
	} else {
		mvwprintw(win,0,1,"[        /        ]");
		mvwprintw(win,LINES/2,1,"[        /        ]");
		setcolor(win,COLOR_INFO);
		mvwprintw(win,0,2,"%8llX",cursorpos1);
		mvwprintw(win,0,11,"%8llX",hBuf1->bufsize);
		mvwprintw(win,LINES/2,2,"%8llX",cursorpos2);
		mvwprintw(win,LINES/2,11,"%8llX",hBuf2->bufsize);
	}
	setcolor(win,COLOR_HEADER);
	mvwprintw(win,0,COLS-2-strlen(hBuf1->filename),"%s",hBuf1->filename);
	mvwprintw(win,LINES/2,COLS-2-strlen(hBuf2->filename),"%s",hBuf2->filename);
	intpos1=getbufferidx(hBuf1,cursorpos1);
	intpos2=getbufferidx(hBuf2,cursorpos2);
	if (intpos1>=0)
	{
		charcnt=0;
		for (i=0;i<(LINES+1)/2-2;i++)
		{
			setcolor(win,COLOR_HEXFIELD);
			if (addrwidth==8)	mvwprintw(win,i+1,0, "% 8llX    ",cursorpos1);
			else			mvwprintw(win,i+1,0,"% 16llX    ",cursorpos1);
			if (addrwidth==8)	mvwprintw(win,i+1+LINES/2,0, "% 8llX    ",cursorpos2);
			else			mvwprintw(win,i+1+LINES/2,0,"% 16llX    ",cursorpos2);

			mvwprintw(win,i+1,COLS-bytesperline-5,"      ");
			mvwprintw(win,i+1+LINES/2,COLS-bytesperline-5,"      ");
			for (j=0;j<bytesperline;j++)
			{
				tInt16	c1,c2;
				if ((intpos1+j)<hBuf1->filesize) c1=hBuf1->dataptr[intpos1+j];
				else	c1=-1;
				if ((intpos1+j)<hBuf2->filesize) c2=hBuf2->dataptr[intpos2+j];
				else	c2=-1;
				if (c1!=c2) setcolor(win,COLOR_DIFF);
				if (c1>=0)
				{
					mvwprintw(win,i+1,addrwidth+3+j*3+j/8,"%02x",c1&0xff);
				}
				else
					mvwprintw(win,i+1,addrwidth+3+j*3+j/8,"  ");

				if (c2>=0)
				{
					mvwprintw(win,i+1+LINES/2,addrwidth+3+j*3+j/8,"%02x",c2&0xff);
				}
				else
					mvwprintw(win,i+1+LINES/2,addrwidth+3+j*3+j/8,"  ");

				setcolor(win,COLOR_HEXFIELD);
				mvwprintw(win,i+1,addrwidth+5+j*3+j/8,"   ");
				mvwprintw(win,i+1+LINES/2,addrwidth+5+j*3+j/8,"   ");
			}
			for (j=0;j<bytesperline;j++)
			{
				unsigned char c1,c2;
				if (intpos1<hBuf1->filesize)
					c1=hBuf1->dataptr[intpos1];
				else c1=' ';
				if (intpos2<hBuf2->filesize)
					c2=hBuf2->dataptr[intpos2];
				else c2=' ';

				if (c1!=c2) setcolor(win,COLOR_DIFF);
				mvwprintw(win,i+1,COLS-bytesperline+j,"%c",(c1>=32 && c1<127)?c1:'.');
				mvwprintw(win,i+1+LINES/2,COLS-bytesperline+j,"%c",(c2>=32 && c2<127)?c2:'.');
				intpos1++;
				cursorpos1++;
				intpos2++;
				cursorpos2++;
 				setcolor(win,COLOR_HEXFIELD);
			}
		}
	}
}
void printmainmenu(WINDOW* win,tBool diffmode)
{
	int i;
	int x;
	unsigned char* menutextsnodiff[]={
"Goto  ",	// F1
"Search",	// F2
"Next  ",	// F3
"Prev  ",	// F4
"HexCal",	// F5
"      ",	// F6
"      ",	// F7
"      ",	// F8
"Undo  ",	// F9
"Quit  "};	// F10

	unsigned char* menutextsdiff[]={
"Goto  ",	// F1
"      ",	// F2
"Next  ",	// F3
"Prev  ",	// F4
"HexCal",	// F5
"      ",	// F6
"      ",	// F7
"      ",	// F8
"       ",	// F9
"Quit  "};	// F10

	setcolor(win,COLOR_MENUHOTKEY);
	
	x=0;
	for (i=0;i<10;i++)
	{
		mvwprintw(win,LINES-1,x,"%i",(i+1)%10);
		x+=8;
	}
	setcolor(win,COLOR_MENUNORMAL);
	x=1;
	for (i=0;i<10;i++)
	{
		mvwprintw(win,LINES-1,x,"%s",diffmode?menutextsdiff[i]:menutextsnodiff[i]);
		x+=8;
	}
	
}
tInt32 movepositions(tInt64* cursorpos,tInt64* firstpos,tInt64 maxbufsize,tInt32 chars,tInt32 lines,tInt32 pages,tBool diffmode)
{
	tInt32 bytesperline;
	tInt32 bytesperhalfpage;
	tInt32 bytesperpage;
	tInt32 addrwidth;
	
	tInt64 newfirstpos=*firstpos;
	tInt64 newcursorpos=*cursorpos;
	

	addrwidth=(maxbufsize>0xffffffffull)?16:8;
	bytesperline=(COLS-(addrwidth+3+3))*8/(8*3+8+1);			// this many bytes can be printed in one line. every 8 bytes there is an extra space in the hex field.
	bytesperhalfpage=bytesperline*(LINES/2-2);
	bytesperpage=bytesperline*(LINES-2);
	

	newcursorpos+=(chars+lines*bytesperline+pages*(diffmode?bytesperhalfpage:bytesperpage));
	if (newcursorpos>=(newfirstpos+(diffmode?bytesperhalfpage:bytesperpage)) ||  (newcursorpos<newfirstpos)) newfirstpos+=(chars+lines*bytesperline+pages*(diffmode?bytesperhalfpage:bytesperpage));
		
	if (newcursorpos<0 || newfirstpos<0 || newcursorpos>maxbufsize || newfirstpos>maxbufsize) return RETNOK;
	else
	{
		*firstpos=newfirstpos;	
		*cursorpos=newcursorpos;
	}
	return RETOK;

}
