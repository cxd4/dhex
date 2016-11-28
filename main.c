#define	MAJORVERSION	0
#define	MINORVERSION	6
#define	REVISION	3
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ncurses.h>
#include "menu.h"
#include "configfile.h"
#include "machine_type.h"
#include "input.h"
#include "output.h"
#include "buffers.h"
#include "ui.h"
#include "hexcalc.h"
#include "search.h"
#include "gpl.h"
#include "markers.h"

void welcomescreen(char* argv0)
{
	fprintf(stderr,"*** DHEX %i.%i%i\n",MAJORVERSION,MINORVERSION,REVISION);
	fprintf(stderr,"*** (C)opyleft 2011 by Thomas Dettbarn\n");
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
	welcomescreen(argv0);
	fprintf(stderr,"%s [Parameters]: General parameters\n",argv0);
	fprintf(stderr," -h, -H, -?                 show this help\n");
	fprintf(stderr," -v, -V                     show the version number %i.%i%i\n",MAJORVERSION,MINORVERSION,REVISION);
        fprintf(stderr," -k, -K                     start the keyboard setup manually\n");
	//fprintf(stderr," -x, -X                     start the hexcalc program (TODO)\n");
	fprintf(stderr," -g, -G                     show the license\n");
	fprintf(stderr," -f, -F [configfile]        read the config from [configfile]\n");
	fprintf(stderr," -m, -M [markerfile]        read the bookmarks from [markerfile]\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"%s [Parameters] [Filename]: Edit a single file\n",argv0);
	fprintf(stderr," -ob, -OB [x]                 set the cursor to [x] (binary)\n");
	fprintf(stderr," -od, -OD [x]                 set the cursor to [x] (decimal)\n");
	fprintf(stderr," -oh, -OH [x]                 set the cursor to [x] (hexadecimal)\n");
	fprintf(stderr," -oo, -OO [x]                 set the cursor to [x] (octal)\n");
	fprintf(stderr," -sa, -SA, -sab, -SAB [x]     find the ascii string x in file (b=backwards)\n");
	fprintf(stderr," -sh, -SH, -shb, -SHB [x]     find the hex string x in file (b=backwards)\n");
	//fprintf(stderr," -r, -R [read searchlog]    read the search positions from this searchlog (TODO)\n");
	//fprintf(stderr," -w, -W [write searchlog]   write the location of the occurances to this log (TODO)\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"%s [Parameters] [Filename1] [Filename2]: Diff mode\n",argv0);
	fprintf(stderr," -o1b, -O1B [x] -o2b, -O2B [y]  set the cursors to [x] and [y] (binary)\n");
	fprintf(stderr," -o1d, -O1D [x] -o2d, -O2D [y]  set the cursors to [x] and [y] (decimal)\n");
	fprintf(stderr," -o1h, -O1H [x] -o2h, -O2H [y]  set the cursors to [x] and [y] (hexadecimal)\n");
	fprintf(stderr," -o1o, -O1O [x] -o2o, -O2O [y]  set the cursors to [x] and [y] (octal)\n");
	//fprintf(stderr," -c, -C [corrlen]           find the best correlation between the files (TODO)\n");
	exit(exitval);
}
int parsecursorpos(tInt64* cursorpos1,tInt64* cursorpos2,char* lastopt,char* argv)
{
	int i;
	int g;
	int d;
	char c;
	tInt64	x=0;

	g=16;	// default: hex
	if (lastopt[1]=='D' || lastopt[2]=='D') g=10;	// -od: decimal
	if (lastopt[1]=='d' || lastopt[2]=='d') g=10;	// -od: decimal
	if (lastopt[1]=='H' || lastopt[2]=='H') g=16;	// -oh: hexadecimal
	if (lastopt[1]=='h' || lastopt[2]=='h') g=16;	// -oh: hexadecimal
	if (lastopt[1]=='B' || lastopt[2]=='B') g=2;	// -ob: binary
	if (lastopt[1]=='b' || lastopt[2]=='b') g=2;	// -ob: binary
	if (lastopt[1]=='O' || lastopt[2]=='O') g=8;	// -oo: octal
	if (lastopt[1]=='o' || lastopt[2]=='o') g=8;	// -oo: octal

	
	for (i=0;i<strlen(argv);i++)
	{
		c=argv[i];
		if (c>32)
		{
			x=x*g;
			d=g;
			if (c>='0' && c<='9') d=c-'0';
			if (c>='a' && c<='f') d=c-'a'+10;
			if (c>='A' && c<='F') d=c-'A'+10;
	
			if (d>=g) return RETNOK;
			x=x+d;
		}
	}
	if (lastopt[1]=='1' || lastopt[2]=='1')	*cursorpos1=x;			// -o1: cursorpos1
	else if (lastopt[1]=='2' || lastopt[2]=='2')	*cursorpos2=x;		// -o2:	cursorpos2
	else {
		*cursorpos1=x;		// -o: both
		*cursorpos2=x;		// -o: both
	}	
	return RETOK;	
	
}
int parsecommandlineoptions(int argc,char** argv,tInt64* cursorpos1,tInt64* cursorpos2,tBool* diffmode,int* filename1,int* filename2,tBool* keyboardsetupreq,char* markerfilename,char* configfile,tSearch* search,tBool* gosearch)
{
	int filenamecnt=0;
	int i;
	int retval=RETOK;
	char lastopt[8];
	tBool moreopts=1;

	*gosearch=0;
	memset(lastopt,0,8);	
	for (i=1;i<argc;i++)
	{
		if (argv[i][0]=='-' && moreopts) 
		{
			memcpy(lastopt,&argv[i][1],8);
			switch (lastopt[0])	// this is for parameters with just one part
			{
				case '-':	lastopt[0]=0;	 // -- means: no more options
						if (argv[i][2]) retval=RETNOK;
						moreopts=0;
						break;
				case '?':
				case 'H':	
				case 'h':	helpscreen(argv[0],1);	// print the helpscreen and be done with it
						lastopt[0]=0;
						break;
				case 'g':
				case 'G':
						print_gpl();		// print the gpl and be done with it
						exit(1);
						lastopt[0]=0;
						break;	
				case 'X':
				case 'x':	printf("TODO:hexcalc\n");
						break;
				case 'K':	
				case 'k':	
						*keyboardsetupreq=1;
						lastopt[0]=0;
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
		else if (lastopt[0])	// two-parted parameters
		{
			switch (lastopt[0])
			{
				case 'F':	
				case 'f':
						snprintf(configfile,512,"%s",argv[i]);
						break;
				case 'S':
				case 's':	
						*gosearch=1;
						search->forwardnotbackward=!(lastopt[2]=='b' || lastopt[2]=='B');
						if (lastopt[1]=='a' || lastopt[1]=='A') 
						{
							memcpy(search->searchstring,argv[i],19);
							search->searchlen=strlen(argv[i]);
							if (search->searchlen>19) search->searchlen=19;
						} else if (lastopt[1]=='h' || lastopt[1]=='H') {
							int j;
							int k=0;
							int x=0;
							search->searchlen=0;
							for (j=0;j<strlen(argv[i]) && search->searchlen<19;j++)
							{
								int y;
								y=-1;
								x<<=4;
								y=(argv[i][j]>='0' && argv[i][j]<='9')?argv[i][j]-'0':y;	
								y=(argv[i][j]>='a' && argv[i][j]<='f')?argv[i][j]-'a'+10:y;
								y=(argv[i][j]>='A' && argv[i][j]<='F')?argv[i][j]-'A'+10:y;
								if (y>=0) 
								{
									x|=y;
									if (k&1)
									{
										search->searchstring[search->searchlen++]=(x&0xff);
									}
									k++;
								}
								else if (argv[i][j]!=' ') 
								{
									fprintf(stderr,"illegal character in hex searchstring.\n");
									retval=RETNOK;
								}
							}
						} else {
							fprintf(stderr,"please select one of -sa or -sh\n");
							retval=RETNOK;	
						}
						break;
				case 'r':
				case 'R':
						memcpy(search->readlogfilename,argv[i],64);
						search->readsearchlog=1;
						break;
				case 'w':
				case 'W':
						memcpy(search->writelogfilename,argv[i],64);
						search->writesearchlog=1;
						break;
				case 'c':
				case 'C':	printf("TODO:correlation\n");
						break;
				case 'O':
				case 'o':	
						retval=parsecursorpos(cursorpos1,cursorpos2,lastopt,argv[i]);
						break;
				case 'm':
				case 'M':
						if (markerfilename) memcpy(markerfilename,argv[i],64);
						break;
				default:
						retval=RETNOK;	
						break;
			}	
			lastopt[0]=0;		
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
//	tBuffer buf1;
//	tBuffer buf2;
	thHexCalc*	hHexCalc=NULL;
	tBuffer*	buf1=NULL;
	tBuffer*	buf2=NULL;
	tOutput*	output=NULL;
	tInt64 cursorpos1;
	tInt64 firstpos1;
	tInt64 cursorpos2;
	tInt64 firstpos2;
	tInt64 oldcursorpos1;
	tInt64 oldfirstpos1;
	tInt64 oldcursorpos2;
	tInt64 oldfirstpos2;
	tSearch search;
	tMarkers* markers;
	tBool	gosearch=0;
	char	markerfilename[64];
	
	tUInt8	windowfield=0;	// 0=hex field buffer 1, 1=ascii field buffer 1, 2=hex field buffer 2, 3=ascii field buffer 2
	tBool	diffmode=0;
	tBool	keyboardsetupreq;
	int	retval;
	
	int ch;
	int filename1=-1;
	int filename2=-1;
	char* homedir;
	char configfile[512];

	memset(configfile,0,512);
	memset(markerfilename,0,64);
	markers=initmarkers();
	output=malloc(sizeof(tOutput));
	memset(output,0,sizeof(tOutput));
	initcolors(output);
	initkeytab(output);
	
	keyboardsetupreq=0;
	cursorpos1=0;
	cursorpos2=0;
	clearsearch(&search);
	if (parsecommandlineoptions(argc,argv,&cursorpos1,&cursorpos2,&diffmode,&filename1,&filename2,&keyboardsetupreq,markerfilename,configfile,&search,&gosearch)!=RETOK)
	{
		if (output)
		{
			if (output->pKeyTab) free(output->pKeyTab);
			free(output);
		}
		if (markers) free(markers);
		helpscreen(argv[0],0);
	}
	if (configfile[0]==0)
	{
		homedir=getenv("HOME");
		if (homedir==NULL)
		{
			fprintf(stderr,"$HOME-variable not set. \n\n");
			fprintf(stderr,"I'm sorry, but you'll have to provide a config-file by using\n");
			fprintf(stderr,"%s -f /PATHTOCONFIGFILE/.dhexrc\n",argv[0]);
			exit(0);
		} else {
			snprintf(configfile,512,"%s/.dhexrc",homedir);
		}
	}

	if (markerfilename[0])	if (parsemarkerfile(markers,markerfilename)!=RETOK)
	{
		if (output)
		{
			if (output->pKeyTab) free(output->pKeyTab);
			free(output);
		}
		if (markers) free(markers);
		helpscreen(argv[0],0);
		
	}
	
	if (filename1==-1 || (filename2==-1 && diffmode)) helpscreen(argv[0],0);
	retval=readconfigfile(output,configfile);
	if (retval==1)		// config file did not exists. creating one
	{
		tFptr f=fopen(configfile,"w");
		if (!f)
		{
			welcomescreen(argv[0]);
			fprintf(stderr,"config file %s did not exists, and i couldn't create it\n",configfile);
			exit(1);
		}
		fprintf(f,"#DHEXCOLORSCHEME\n");
		fprintf(f,"#VERSION 0\n");
		fprintf(f,"#possible colors are: BLACK,RED,GREEN,YELLOW,BLUE,MAGENTA,CYAN,WHITE\n");
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

	hHexCalc=malloc(sizeof(thHexCalc));
	memset(hHexCalc,0,sizeof(thHexCalc));
	buf1=malloc(sizeof(tBuffer));
	memset(buf1,0,sizeof(tBuffer));
	if (diffmode)
	{
		buf2=malloc(sizeof(tBuffer));
		memset(buf2,0,sizeof(tBuffer));
	}

	if (openbuf(buf1,1,argv[filename1])!=RETOK)
	{
		fprintf(stderr,"error opening inputfile %s\n",argv[filename1]);
		exit(1);
	}
	if (diffmode) if (openbuf(buf2,2,argv[filename2])!=RETOK)
	{
		fprintf(stderr,"error opening second inputfile %s\n",argv[filename2]);
		exit(1);
	}
	if (gosearch && !diffmode) searchfor(&search,buf1,&cursorpos1,1);
	if (!gosearch || diffmode || !search.writesearchlog)
	{
		output->win=initscr();
		pairsinit(output);
		noecho();
		nodelay(output->win,1);
		if (keyboardsetupreq) keyboardsetup(output,configfile);
		readbuf(buf1,0);
		firstpos1=cursorpos1;
		firstpos2=cursorpos2;
		ch=0;
		while (ch!=KEYF10)
		{	
			printmainmenu(output,diffmode);
			if (diffmode)
				printbufferdiff(output,buf1,buf2,cursorpos1,cursorpos2);
			else		
				printbuffersingle(output,buf1,cursorpos1,firstpos1,windowfield);
			ch=getkey((tKeyTab*)output->pKeyTab,1);

#define	MOVEMENTDEFINE(KEY,VIKEY,mvchar,mvline,mvpage)	\
			if (ch==KEY || (VIKEY && ch==VIKEY && windowfield==0))	\
			{	\
				tInt32	err;	\
				oldfirstpos1=firstpos1;	\
				oldcursorpos1=cursorpos1;	\
				oldfirstpos2=firstpos2;	\
				oldcursorpos2=cursorpos2;	\
				\
				err=movepositions(&cursorpos1,&firstpos1,buf1->bufsize,mvchar,mvline,mvpage,diffmode);	\
				\
				if (diffmode)	\
				{	\
					movepositions(&cursorpos2,&firstpos2,buf2->bufsize,mvchar,mvline,mvpage,diffmode);	\
					err=((cursorpos1<0 && cursorpos2<0) || (cursorpos1>buf1->bufsize && cursorpos2>buf2->bufsize)); \
				}	\
				if (err)	\
				{	\
					firstpos1=oldfirstpos1;		\
					cursorpos1=oldcursorpos1;	\
					firstpos2=oldfirstpos2;		\
					cursorpos2=oldcursorpos2;	\
				}	\
			}
			MOVEMENTDEFINE(KEYRIGHT,'l'	, 1, 0, 0);
			MOVEMENTDEFINE(KEYLEFT,'h'	,-1, 0, 0);
			MOVEMENTDEFINE(KEYDOWN,'j'	, 0, 1, 0);
			MOVEMENTDEFINE(KEYUP,'k'	, 0,-1, 0);
			MOVEMENTDEFINE(KEYPGDOWN,' '	, 0, 0, 1);
			MOVEMENTDEFINE(KEYPGUP,0	, 0, 0,-1);

#undef	MOVEMENTDEFINE
			if (ch==KEYHOME || (ch=='^' && windowfield==0))
			{
				if (diffmode)
				{
					tInt64	dmin,dmax;

					dmin=MIN(cursorpos1,cursorpos2);
					dmax=MAX(cursorpos1,cursorpos2);
					if (dmin==0)
					{
						cursorpos1-=dmax;
						cursorpos2-=dmax;
					} else {
						cursorpos1-=dmin;
						cursorpos2-=dmin;
					}
					firstpos1=cursorpos1;
					firstpos2=cursorpos2;

				} else {
					firstpos1=cursorpos1=0;
				}
			}
			if (ch==KEYEND || (ch=='$' && windowfield==0))
			{
				if (diffmode)
				{
					tInt64	dmin,dmax;
					dmin=MIN(buf1->bufsize-cursorpos1,buf2->bufsize-cursorpos2);
					dmax=MAX(buf1->bufsize-cursorpos1,buf2->bufsize-cursorpos2);

					if (dmin==0)
					{

						cursorpos2+=dmax;
						cursorpos1+=dmax;
					} else {
						cursorpos1+=dmin;
						cursorpos2+=dmin;
					}
				} else {
					firstpos1=cursorpos1=buf1->bufsize;
				}
			}
			if (ch==KEYTAB) {windowfield=(windowfield+1)&1;}
			if ((ch==KEYF1 || (ch==':' && windowfield==0))&& !diffmode)	{
				if (gotomask(output,markers,&cursorpos1)==RETOK)
				{
					firstpos1=cursorpos1;	
				}
			}
			if ((ch==KEYF2 || ((ch=='/' || ch=='?') && (windowfield==0))) && !diffmode)	
			{
				if (ch=='/')	search.forwardnotbackward=1;	// / means forward
				if (ch=='?')	search.forwardnotbackward=0;	// ? means backward
				searchmask(output,&search,buf1,&cursorpos1);
				firstpos1=cursorpos1;
			}
			if (ch==KEYF3 || ch==KEYF4 || (windowfield==0 && (ch=='n' || ch=='N')))
			{
				if (ch=='n')	ch=KEYF3;	// n=next
				if (ch=='N')	ch=KEYF4;	// N=previous
				if (diffmode)
				{
					tInt64	actcursorpos1=cursorpos1;
					tInt64	actcursorpos2=cursorpos2;
					tInt32	idx1,idx2;
					tBool	found;
					tBool	diffnotdiff;
					tUInt64	minbufsize=(buf1->bufsize<buf2->bufsize)?buf1->bufsize:buf2->bufsize;	// get the minimum of the filesizes
					tUInt64	searchcnt=minbufsize;

					oldcursorpos1=cursorpos1;
					oldcursorpos2=cursorpos2;
					actcursorpos1=actcursorpos1+((ch==KEYF3)?1:-1);	// F3: next, F4:prev
					actcursorpos2=actcursorpos2+((ch==KEYF3)?1:-1);	// F3: next, F4:prev
					found=(!minbufsize);
					idx1=getbufferidx(buf1,actcursorpos1);
					idx2=getbufferidx(buf2,actcursorpos2);
					diffnotdiff=(buf1->data[idx1]!=buf2->data[idx2]);	// if the cursor is on something which is a diff, look for the next time the values are the same
					while (actcursorpos1!=oldcursorpos1 && actcursorpos2!=oldcursorpos2 && searchcnt && !found)
					{
						idx1=getbufferidx(buf1,actcursorpos1);
						idx2=getbufferidx(buf2,actcursorpos2);
						if ((buf1->data[idx1]!=buf2->data[idx2]) != diffnotdiff) found=1;
						else 	{
							actcursorpos1=actcursorpos1+((ch==KEYF3)?1:-1);	// F3: next, F4:prev
							actcursorpos2=actcursorpos2+((ch==KEYF3)?1:-1);	// F3: next, F4:prev
						}
						if (actcursorpos1<0 || actcursorpos2<0) 
						{
							actcursorpos1+=minbufsize;	// start at the end of the smallest file
							actcursorpos2+=minbufsize;	// start at the end of the smallest file
						}
						if (actcursorpos1>=minbufsize && actcursorpos2>=minbufsize)
						{
							actcursorpos1-=minbufsize;		// if you are at the end of the smallest file, start at the beginning
							actcursorpos2-=minbufsize;		// if you are at the end of the smallest file, start at the beginning
						}
						searchcnt--;	
					}
					if (found) 
					{
						cursorpos1=actcursorpos1;
						cursorpos2=actcursorpos2;
					}
				} 
				else if (search.occurancesfound)
				{
					searchfor(&search,buf1,&cursorpos1,(ch==KEYF3));	// f3: next
					firstpos1=cursorpos1;
				}
			}	
			if (ch==KEYF5)	hexcalc(output,hHexCalc);
			if ((ch==KEYF9 || (ch=='u' && windowfield==0)) && !diffmode)
			{
				if (buf1->changesnum) 
				{
					if ((buf1->changes[buf1->changesnum].pos==(buf1->bufsize-1) && (buf1->filesize<=buf1->changes[buf1->changesnum].pos))) buf1->bufsize--;	// if this was the last byte in the buffer, decrease its size
					cursorpos1=buf1->changes[--buf1->changesnum].pos;
					if (cursorpos1<firstpos1) firstpos1=cursorpos1;
					if ((firstpos1+100)>cursorpos1) firstpos1=cursorpos1;	// TODO: 100 depends on the size of the window
				}

			}
			if (buf1->changesnum<CHANGEBUFSIZE-1 && !diffmode)
			{
				if (((ch>='0' && ch<='9') || (ch>='a' && ch<='f') || (ch>='A' && ch<='F')) && windowfield==0)
				{
					buf1->nexthex<<=4;
					if (ch>='a') ch-=32;
					buf1->nexthex|=((ch>='A')?(ch-'A'+10):(ch-'0'));
					if (buf1->nibble)
					{
						if (cursorpos1==buf1->bufsize) buf1->bufsize++;	// append the byte at the end
						buf1->changes[buf1->changesnum].pos=cursorpos1;
						buf1->changes[buf1->changesnum].after=buf1->nexthex;
						buf1->changesnum++;
						movepositions(&cursorpos1,&firstpos1,buf1->bufsize,1,0,0,diffmode);
					}
					buf1->nibble=!buf1->nibble;
					buf1->changepos=cursorpos1;
				} else buf1->nibble=0;
				if (ch>=32 && ch<127 && windowfield==1)
				{
					if (cursorpos1==buf1->bufsize) buf1->bufsize++;	// append the byte at the end
					buf1->changes[buf1->changesnum].pos=cursorpos1;
					buf1->changes[buf1->changesnum].after=ch;
					buf1->changesnum++;
					movepositions(&cursorpos1,&firstpos1,buf1->bufsize,1,0,0,diffmode);
				}
			}
			if (ch==KEYF10)
			{
				if (buf1->changesnum) if (savedialog(output,buf1)!=RETOK)
				{
					ch=0;
				}
			}
		}
		if (output)
		{
			if (output->pKeyTab) free(output->pKeyTab);
			free(output);
		}
		if (buf1) free(buf1);
		if (buf2) free(buf2);
		if (hHexCalc)	free(hHexCalc);
		if (markers)	free(markers);
		if (output->win) endwin();
		welcomescreen(argv[0]);
	} else {
		fprintf(stderr,"%lli occurances found\n",search.occurancesfound);
	}
	return 0;
}
