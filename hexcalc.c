#include <stdio.h>
#include <stdlib.h>
#include "hexcalc.h"
#include <ncurses.h>
#include "output.h"
#include "input.h"
tUInt64	values[32];
char	operators[32];
tInt8	valuenum=0;
typedef	enum {FIELDHEX,FIELDDEC,FIELDBIN}	eFields;

void hexcalc(WINDOW* win)
{
	tInt16	offsx;
	tInt16	offsy;
	tInt64	svalue;
	tInt64	akkumulator=0;
	int i;
	int j;
	int cursorline=-1;
	tInt16	ch;
	tUInt64 x;
	tUInt64	newval=0;
	tUInt8	dig=0;
	tUInt64	g=10;
	tBool	neg=0;
	eFields	field=FIELDHEX;

	valuenum=0;
	operators[0]='=';

	offsx=COLS/2-40;
	offsy=LINES/2-10;	
	drawframe(win,offsy,offsx,21,79,"HexCalc");

	setcolor(win,COLOR_BRACKETS);
	for (i=0;i<17;i++)
	{
		mvwprintw(win,offsy+2+i,offsx+2,"[                ] [                    ] [                                ]");
	}
	setcolor(win,COLOR_MENUNORMAL);
	mvwprintw(win,offsy+20,offsx+ 2,":Add");
	mvwprintw(win,offsy+20,offsx+ 8,":Sub");
	mvwprintw(win,offsy+20,offsx+14,":Mul");
	mvwprintw(win,offsy+20,offsx+20,":Div");
	mvwprintw(win,offsy+20,offsx+26,":Mod");
	mvwprintw(win,offsy+20,offsx+32,">:Right");
	mvwprintw(win,offsy+20,offsx+41,"<:Left");
	mvwprintw(win,offsy+20,offsx+49,":And");
	mvwprintw(win,offsy+20,offsx+55,":Or");
	mvwprintw(win,offsy+20,offsx+61,":Xor");
	
	mvwprintw(win,offsy+20,offsx+74,"Exit");	

	setcolor(win,COLOR_MENUHOTKEY);
	mvwprintw(win,offsy+20,offsx+ 1,"+");
	mvwprintw(win,offsy+20,offsx+ 7,"-");
	mvwprintw(win,offsy+20,offsx+13,"*");
	mvwprintw(win,offsy+20,offsx+19,"/");
	mvwprintw(win,offsy+20,offsx+25,"%%");
	mvwprintw(win,offsy+20,offsx+31,">");
	mvwprintw(win,offsy+20,offsx+40,"<");
	mvwprintw(win,offsy+20,offsx+48,"&");
	mvwprintw(win,offsy+20,offsx+54,"|");
	mvwprintw(win,offsy+20,offsx+60,"^");
	mvwprintw(win,offsy+20,offsx+75,"x");
	

	setcolor(win,COLOR_TEXT);
	mvwprintw(win,offsy+1,offsx+ 3,"Hex");
	mvwprintw(win,offsy+1,offsx+22,"Dec");
	mvwprintw(win,offsy+1,offsx+45,"Bin");
	while (ch!='x' && ch!='X')
	{
		setcolor(win,COLOR_TEXT);
		mvwprintw(win,offsy+19,offsx+ 1,"%c",operators[0]);
		for (i=0;i<17 && i<valuenum;i++)
		{	
			setcolor(win,COLOR_TEXT);
			mvwprintw(win,offsy+18-i,offsx+ 1,"%c",operators[i+1]);
			x=values[i];
			setcolor(win,(field==FIELDHEX && cursorline==i)?COLOR_INPUT:COLOR_TEXT);
			mvwprintw(win,offsy+18-i,offsx+ 3,"%16llx",x);
			setcolor(win,(field==FIELDDEC && cursorline==i)?COLOR_INPUT:COLOR_TEXT);
			mvwprintw(win,offsy+18-i,offsx+22,"%20lli",(tInt64)x);
			setcolor(win,(field==FIELDBIN && cursorline==i)?COLOR_INPUT:COLOR_TEXT);
			for (j=0;j<32;j++)
			{
				if (x || !j)	mvwprintw(win,offsy+18-i,offsx+76-j,"%i",(x&1));
				else		mvwprintw(win,offsy+18-i,offsx+76-j," ");
				x>>=1;
			}
		}
		if (cursorline==-1) x=newval; else x=values[cursorline];
		setcolor(win,(field==FIELDHEX && cursorline==-1)?COLOR_INPUT:COLOR_TEXT);
		mvwprintw(win,offsy+19,offsx+ 3,"%16llx",x);
		setcolor(win,(field==FIELDDEC && cursorline==-1)?COLOR_INPUT:COLOR_TEXT);
		if (x || !neg) mvwprintw(win,offsy+19,offsx+22,"%20lli",(tInt64)x);
		else if (neg) mvwprintw(win,offsy+19,offsx+22,"                  -"); 
		setcolor(win,(field==FIELDBIN && cursorline==-1)?COLOR_INPUT:COLOR_TEXT);
		for (j=0;j<32;j++)
		{
			if (x || !j)	mvwprintw(win,offsy+19,offsx+76-j,"%i",(x&1));
			else		mvwprintw(win,offsy+19,offsx+76-j," ");
			x>>=1;
		}
		ch=getkey(1);
		switch (ch)
		{
			case	KEYUP:		if (cursorline<(valuenum-1))	cursorline++;break;
			case	KEYDOWN:	if (cursorline>-1)		cursorline--;break;
			case	KEYLEFT:	if (field!=FIELDHEX)		field--;break;
			case	KEYRIGHT:	if (field!=FIELDBIN)		field++;break;
			case	KEYTAB:		field=(field!=FIELDBIN)?field+1:FIELDHEX;break;
			case	'+':
			case	'-':
			case	'*':
			case	'/':
			case	'%':
			case	'&':
			case	'|':
			case	'^':
			case	'>':
			case	'<':
			case	KEYENTER:
					
						if (cursorline!=-1) newval=values[cursorline];
						cursorline=-1;
						if (valuenum)
						{
							switch(operators[0])
							{
								case	'=':	akkumulator=newval;break;
								case	'+':	akkumulator=akkumulator+newval;break;
								case	'-':	akkumulator=akkumulator-newval;break;
								case	'*':	akkumulator=akkumulator*newval;break;
								case	'/':	akkumulator=akkumulator/newval;break;
								case	'%':	akkumulator=akkumulator%newval;break;
								case	'&':	akkumulator=akkumulator&newval;break;
								case	'|':	akkumulator=akkumulator|newval;break;
								case	'^':	akkumulator=akkumulator^newval;break;
								case	'>':	akkumulator=akkumulator>>newval;break;
								case	'<':	akkumulator=akkumulator<<newval;break;
							}
						}
						else
						{
							akkumulator=newval;
						}
						if (ch==KEYENTER) ch='=';
						if (cursorline!=-1) newval=values[cursorline];	
						for (i=31;i>0;i--)
						{
							values[i]=values[i-1];
							operators[i]=operators[i-1];
						}
						operators[0]=ch;
						values[0]=newval;
						if (ch=='=') newval=akkumulator; else newval=0;
						valuenum++;
						break;


			default:
				if (cursorline!=-1) cursorline=-1;
				dig=17;
				if (field==FIELDBIN) g=2;
				else if (field==FIELDDEC) g=10;
				else if (field==FIELDHEX) g=16;

				if (ch>='a' && ch<='f') ch-=32;
				if (field==FIELDBIN && ch>='0' && ch<='1') dig=(ch-'0');
				else if (field==FIELDDEC && ch>='0' && ch<='9') dig=(ch-'0');
				else if (field==FIELDHEX && ch>='0' && ch<='9') dig=(ch-'0');
				else if (field==FIELDHEX && ch>='A' && ch<='F') dig=(ch-'A'+10);
				else if (field==FIELDDEC && ch=='-') neg=1;
				if (ch==KEYBACKSPACE) 
				{
					if (newval) newval=newval/g;
					else neg=0;
				} else if (dig!=17) { 
					newval=newval*g;
					newval=newval+(neg?-dig:dig);
				}
				break;
		}
	}
}

