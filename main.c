#define	MAJORVERSION	0
#define	MINORVERSION	6
#define	REVISION	0
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "menu.h"
#include "machine_type.h"
#include "input.h"
#include "output.h"
#include "buffers.h"
#include "ui.h"
#include "hexcalc.h"
#include "search.h"
#include "gpl.h"
char configfile[512];
void welcomescreen(char* argv0)
{
	fprintf(stderr,"*** DHEX %i.%i%i\n",MAJORVERSION,MINORVERSION,REVISION);
	fprintf(stderr,"*** (C)opyleft 2010 by Thomas Dettbarn\n");
	fprintf(stderr,"*** dettus@dettus.net (include DHEX somewhere in the subject)\n\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"(start it with %s -gpl to see the license)\n",argv0);
	fprintf(stderr,"\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"This program is free software; you can redistribute it and/or modify\n");
	fprintf(stderr,"it under the terms of the GNU General Public License as published by\n");
	fprintf(stderr,"the Free Software Foundation; either version 2 of the License, or\n");
	fprintf(stderr,"(at your option) any later version.\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"This program is distributed in the hope that it will be useful,\n");
	fprintf(stderr,"but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	fprintf(stderr,"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	fprintf(stderr,"GNU General Public License for more details.\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"You should have received a copy of the GNU General Public License\n");
	fprintf(stderr,"along with this program; if not, write to the Free Software\n");
	fprintf(stderr,"Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n");
	fprintf(stderr,"	\n\n");
}
void helpscreen(char* argv0,int exitval)
{
	endwin();
	welcomescreen(argv0);
	fprintf(stderr,"%s [Parameters]: General parameters\n",argv0);
	fprintf(stderr," -h, -H, -?                 show this help\n");
	fprintf(stderr," -v, -V                     show the version number %i.%i%i\n",MAJORVERSION,MINORVERSION,REVISION);
        //fprintf(stderr," -k, -K                     start the keyboard setup (TODO)\n");
	//fprintf(stderr," -x, -X                     start the hexcalc program (TODO)\n");
	fprintf(stderr," -g, -G                     show the license\n");
	fprintf(stderr," -f, -F [configfile]        read the config from [configfile]\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"%s [Parameters] [Filename]: Edit a single file\n",argv0);
	//fprintf(stderr," -o, -O [x]                 set the cursor to [x] (TODO)\n");
	//fprintf(stderr," -s, -S [x]                 find the hexvalue x in file (TODO)\n");
	//fprintf(stderr," -r, -R [read searchlog]    read the search positions from this searchlog (TODO)\n");
	//fprintf(stderr," -w, -W [write searchlog]   write the location of the occurances to this log (TODO)\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"%s [Parameters] [Filename1] [Filename2]: Diff mode\n",argv0);
	//fprintf(stderr," -c, -C [corrlen]           find the best correlation between the files (TODO)\n");
	//fprintf(stderr," -o, -O [x,y]               set the first cursor to [x] and the second to [y] (TODO)\n");
	exit(exitval);
}
int parsecommandlineoptions(int argc,char** argv,tInt64* cursorpos,tBool* diffmode,int* filename1,int* filename2,tBool* keyboardsetupreq)
{
	int filenamecnt=0;
	int i;
	int retval=RETOK;
	char lastopt=0;
	tBool moreopts=1;
	
	for (i=1;i<argc;i++)
	{
		if (argv[i][0]=='-' && moreopts) 
		{
			lastopt=argv[i][1];
			switch (lastopt)	// this is for parameters with just one part
			{
				case '-':	lastopt=0;	 // -- means: no more options
						if (argv[i][2]) retval=RETNOK;
						moreopts=0;
						break;
				case '?':
				case 'H':	
				case 'h':	helpscreen(argv[0],1);	// print the helpscreen and be done with it
						lastopt=0;
						break;
				case 'g':
				case 'G':
						endwin();
						print_gpl();		// print the gpl and be done with it
						exit(1);
						lastopt=0;
						break;	
				case 'X':
				case 'x':	printf("TODO:hexcalc\n");
						break;
				case 'K':	
				case 'k':	
						*keyboardsetupreq=1;
						lastopt=0;
						break;
				case 'v':
				case 'V':
						printf("%i.%i%i\n",MAJORVERSION,MINORVERSION,REVISION);
						exit(1);
						break;
				default:	
						break;
				
			}
		}
		else if (lastopt)	// two-parted parameters
		{
			switch (lastopt)
			{
				case 'F':	
				case 'f':
						snprintf(configfile,512,"%s",argv[i]);
						break;
				case 'S':
				case 's':	printf("TODO:search\n");
						break;
				case 'r':
				case 'R':
						printf("TODO:read searchlog\n");
						break;
				case 'w':
				case 'W':
						printf("TODO:write searchlog\n");
						break;
				case 'c':
				case 'C':	printf("TODO:correlation\n");
						break;
				case 'O':
				case 'o':	printf("TODO:cursor offset\n");
						break;
				default:
						retval=RETNOK;	
						break;
			}	
			lastopt=0;		
		}
		else // no parameters, must be a filename
		{
			if (filenamecnt==0)	
			{
				*filename1=i;
			}
			if (filenamecnt==1)	
			{
				*filename2=i;
				*diffmode=1;
			}
			filenamecnt++;
			if (filenamecnt>2) retval=RETNOK;
		}
	}
	return retval;
}

int main(int argc,char** argv)
{
	int i;
	tBuffer buf1;
	tBuffer buf2;
	tInt64 cursorpos;
	tInt64 firstpos;
	tSearch search;
	WINDOW* win;
	tUInt8	windowfield=0;	// 0=hex field buffer 1, 1=ascii field buffer 1, 2=hex field buffer 2, 3=ascii field buffer 2
	tBool	nibble;
	tBool	diffmode=0;
	tUInt8	nexthex;
	tBool	keyboardsetupreq;
	int	retval;
	
	int ch;
	unsigned char text[64];
	int filename1=-1;
	int filename2=-1;

	snprintf(configfile,512,"%s/.dhexrc",getenv("HOME"));
	initcolors();
	memset(&buf1,0,sizeof(buf1));
	memset(&buf2,0,sizeof(buf2));
	keyboardsetupreq=0;
	if (parsecommandlineoptions(argc,argv,&cursorpos,&diffmode,&filename1,&filename2,&keyboardsetupreq)!=RETOK)
	{
		helpscreen(argv[0],0);
	}
	if (filename1==-1 || (filename2==-1 && diffmode)) helpscreen(argv[0],0);
	retval=readconfigfile(configfile);
	if (retval==1)		// config file did not exists. creating one
	{
		tFptr f=fopen(configfile,"w");
		if (!f)
		{
			welcomescreen(argv[0]);
			fprintf(stderr,"config file %s did not exists, and i couldn't create it\n");
			exit(1);
		}
		fprintf(f,"#DHEXCOLORSCHEME\n");
		fprintf(f,"#VERSION 0\n");
		fprintf(f,"#possible colors are: BLACK,GREEN,GREEN,YELLOW,BLUE,MAGENTA,CYAN,BLACK\n");
		fprintf(f,"#possible extra flags are: UNDERLINE,REVERSE,BLINK,DIM,BOLD\n");
		fprintf(f,"\n");
		fprintf(f,"BRACKETS:       FG=BLACK,BG=BLACK,BOLD\n");
		fprintf(f,"HEXFIELD:       FG=WHITE,BG=BLACK\n");
		fprintf(f,"INPUT:          FG=BLACK,BG=WHITE\n");
		fprintf(f,"CURSOR:         FG=WHITE,BG=BLACK\n");
		fprintf(f,"TEXT:           FG=WHITE,BG=BLACK,BOLD\n");
		fprintf(f,"MENU_NORMAL:    FG=CYAN,BG=BLACK\n");
		fprintf(f,"MENU_HIGHLIGHT: FG=BLACK,BG=CYAN\n");
		fprintf(f,"MENU_HOTKEY:    FG=YELLOW,BG=BLACK,BOLD\n");
		fprintf(f,"MENU_HOTKEY_HI: FG=YELLOW,BG=CYAN,BOLD\n");
		fprintf(f,"FRAME:          FG=BLUE,BG=BLACK,BOLD\n");
		fprintf(f,"NORMAL_DIFF:    FG=YELLOW,BG=BLACK\n");
		fprintf(f,"CURSOR_DIFF:    FG=YELLOW,BG=WHITE,BOLD\n");
		fprintf(f,"HEADLINE:       FG=BLUE,BG=BLACK,BOLD\n");
		fprintf(f,"	\n");
		fclose(f);
		retval=2;
	}
	if (retval==2) keyboardsetupreq=1;
	if (openbuf(&buf1,1,argv[filename1])!=RETOK)
	{
		fprintf(stderr,"error opening inputfile %s\n",argv[filename1]);
		exit(1);
	}
	if (diffmode) if (openbuf(&buf2,2,argv[filename2])!=RETOK)
	{
		fprintf(stderr,"error opening second inputfile %s\n",argv[filename2]);
		exit(1);
	}
	win=initscr();
	pairsinit();
	noecho();
	nodelay(win,1);
	if (keyboardsetupreq) keyboardsetup(win,configfile);
	readbuf(&buf1,0);
	cursorpos=0;
	firstpos=0;
	ch=0;
	clearsearch(&search);
	while (ch!=KEYF10)
	{	
		tInt64	maxbufsize;
		printmainmenu(win,diffmode);
		if (diffmode)
			printbufferdiff(win,&buf1,&buf2,cursorpos,cursorpos);
		else		
			printbuffersingle(win,&buf1,cursorpos,firstpos,windowfield);
		ch=getkey(1);
		if (diffmode)
			maxbufsize=(buf1.bufsize>buf2.bufsize)?(buf1.bufsize):(buf2.bufsize);
		else
			maxbufsize=buf1.bufsize;
		if (ch==KEYRIGHT) movepositions(&cursorpos,&firstpos,maxbufsize,1,0,0,diffmode);
		if (ch==KEYLEFT) movepositions(&cursorpos,&firstpos,maxbufsize,-1,0,0,diffmode);
		if (ch==KEYDOWN) movepositions(&cursorpos,&firstpos,maxbufsize,0,1,0,diffmode);
		if (ch==KEYUP) movepositions(&cursorpos,&firstpos,maxbufsize,0,-1,0,diffmode);
		if (ch==KEYPGDOWN) movepositions(&cursorpos,&firstpos,maxbufsize,0,0,1,diffmode);
		if (ch==KEYPGUP) movepositions(&cursorpos,&firstpos,maxbufsize,0,0,-1,diffmode);
		if (ch==KEYHOME) {firstpos=cursorpos=0;}
		if (ch==KEYEND) {
					firstpos=cursorpos=buf1.bufsize;
					if (diffmode)
					{
						if (buf2.bufsize<cursorpos)
						{
							firstpos=cursorpos=buf2.bufsize;	// go to the end of the SMALLEST file
						}
					}
				}
		if (ch==KEYTAB) {windowfield=(windowfield+1)&1;}
		if (ch==KEYF1)	{
			if (gotomask(win,&cursorpos)==RETOK)
			{
				firstpos=cursorpos;	
			}
		}
		if (ch==KEYF2)	
		{
			searchmask(win,&search,&buf1,&cursorpos);
			firstpos=cursorpos;
		}
		if (ch==KEYF3 || ch==KEYF4)
		{
			if (diffmode)
			{
				tInt64	oldcursorpos=cursorpos;
				tInt64	actcursorpos=cursorpos;
				tInt32	idx1,idx2;
				tBool	found;
				tUInt64	minbufsize=(buf1.bufsize<buf2.bufsize)?buf1.bufsize:buf2.bufsize;	// get the minimum of the filesizes
				tUInt64	searchcnt=minbufsize;

				actcursorpos=actcursorpos+((ch==KEYF3)?1:-1);	// F3: next, F4:prev
				found=(!minbufsize);
				while (actcursorpos!=oldcursorpos && searchcnt && !found)
				{
					idx1=getbufferidx(&buf1,actcursorpos);
					idx2=getbufferidx(&buf2,actcursorpos);
					if (buf1.dataptr[idx1]!=buf2.dataptr[idx2]) found=1;
					else 	actcursorpos=actcursorpos+((ch==KEYF3)?1:-1);	// F3: next, F4:prev
					if (actcursorpos<0) actcursorpos=minbufsize-1;	// start at the end of the smallest file
					if (actcursorpos>=minbufsize) actcursorpos=0;	// if you are at the end of the smallest file, start at the beginning
					searchcnt--;	
				}
				if (found) cursorpos=actcursorpos;
			} 
			else if (search.occurancesfound)
			{
				searchfor(&search,&buf1,&cursorpos,(ch==KEYF3));	// f3: next
				firstpos=cursorpos;
			}
		}	
		if (ch==KEYF5)	hexcalc(win);
		if (ch==KEYF9)
		{
			if (buf1.changesnum) 
			{
				if ((buf1.changes[buf1.changesnum].pos==(buf1.bufsize-1) && (buf1.filesize<=buf1.changes[buf1.changesnum].pos))) buf1.bufsize--;	// if this was the last byte in the buffer, decrease its size
				cursorpos=buf1.changes[--buf1.changesnum].pos;
				if (cursorpos<firstpos) firstpos=cursorpos;
				if ((firstpos+100)>cursorpos) firstpos=cursorpos;	// TODO: 100 depends on the size of the window
			}

		}
		if (buf1.changesnum<CHANGEBUFSIZE-1)
		{
			if (((ch>='0' && ch<='9') || (ch>='a' && ch<='f') || (ch>='A' && ch<='F')) && windowfield==0)
			{
				buf1.nexthex<<=4;
				if (ch>='a') ch-=32;
				buf1.nexthex|=((ch>='A')?(ch-'A'+10):(ch-'0'));
					if (buf1.nibble)
				{
					if (cursorpos==buf1.bufsize) buf1.bufsize++;	// append the byte at the end
					buf1.changes[buf1.changesnum].pos=cursorpos;
					buf1.changes[buf1.changesnum].after=buf1.nexthex;
						buf1.changesnum++;
					movepositions(&cursorpos,&firstpos,&buf1,1,0,0,diffmode);
				}
				buf1.nibble=!buf1.nibble;
				buf1.changepos=cursorpos;
			} else buf1.nibble=0;
			if (ch>=32 && ch<127 && windowfield==1)
			{
				if (cursorpos==buf1.bufsize) buf1.bufsize++;	// append the byte at the end
				buf1.changes[buf1.changesnum].pos=cursorpos;
				buf1.changes[buf1.changesnum].after=ch;
				buf1.changesnum++;
				movepositions(&cursorpos,&firstpos,&buf1,1,0,0,diffmode);
			}
		}
		if (ch==KEYF10)
		{
			if (buf1.changesnum) if (savedialog(win,&buf1)!=RETOK)
			{
				ch=0;
			}
		}
	}

	endwin();
	welcomescreen(argv[0]);
	return 0;
}
