#include <stdio.h>
#include <sys/types.h>        // uint64_t
#include <stdlib.h>
#include <ncurses.h>
#include "gpl.h"
#include "ui.h"
#ifdef FREEBSD
	#define file_position_t uint64_t
#endif
#ifdef LINUX
	#include <stdint.h>
	#define file_position_t uint64_t
#endif
#ifdef IRIX
        #define file_position_t fpos_t
#endif
#ifdef SOLARIS
	#define file_position_t fpos64_t
#endif
#ifdef HPUX
	#define file_position_t fpos64_t
#endif


FILE* inputfile;
FILE* inputfile2;
int obenanfangen=1;
file_position_t cursorpos;
unsigned int cols;
int rows;
FILE *inputfile;
file_position_t chpos[524288];
unsigned char change[524288];
int chnum=0;
char* searchstring;
int searchstring2[255];
unsigned int searchstring2len=0;
int searchstring3[255];
unsigned int searchstring3len=0;
char* writesearchfilename;
char* readsearchfilename;
FILE *readsearchfile;
int writesearch=0;
int readsearch=0;
int kmp[256];
int kmpback[256];
int diffnotedit=0;
char* tohex(char x)
{
	char ziffern[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	char* ch;
	int z=(int)x;
	if (z<0) z=256+z;
	ch=malloc(3);	
	ch[0]=ziffern[z/16];
	ch[1]=ziffern[z%16];
	ch[2]=0;
	return ch;	
	
}
void print_pos(WINDOW *parent_window, file_position_t p,int y)
{
	mvwprintw(parent_window,y,0,"%10X",(unsigned long) p);	
	
}
void print_hex(WINDOW *parent_window,file_position_t p,file_position_t cursorpos,file_position_t filesize,file_position_t rfilesize,int hexnotasc,int ch2)
{
	unsigned char buffer[2];
	float f;
	unsigned int i;
	int j;
	int x;
	int y;
	int c;
	file_position_t ap=p;
	f=(float)COLS-10;
	f=f/4.125;
	cols=(int)f;
	x=COLS-10-((int)((float)cols*4.125));
	rows=LINES-2;
	wattrset(parent_window,attrs[COLOR_BRACKETS]);
	mvwprintw(parent_window,0,1,"[          /          ]");
	wattrset(parent_window,attrs[COLOR_TEXT]);
	mvwprintw(parent_window,0,2,"%10X",(unsigned long)cursorpos);	
	mvwprintw(parent_window,0,13,"%10X",(unsigned long)(filesize-1));	
	wattrset(parent_window,attrs[COLOR_HEXFIELD]);
	fseek(inputfile,p,SEEK_SET);
	for (y=1;y<LINES-1;y++)
	{
		wattrset(parent_window,attrs[COLOR_HEXFIELD]);
		print_pos(parent_window,p+(y-1)*cols,y);
		for (i=0;i<cols;i++)
		{
			if (!feof(inputfile)) fgets(buffer,sizeof(buffer),inputfile);
			c=buffer[0];
			
			if (chnum!=0) for (j=0;j<chnum;j++) if (ap==chpos[j]) c=change[j];
			f=(float)i;
			f=f*3.125;
			if (ap==cursorpos && hexnotasc==1 && ap<=filesize) 
			{
				if (c==buffer[0]) wattrset(parent_window,attrs[COLOR_CURSOR]); else wattrset(parent_window,attrs[COLOR_DIFF_CURSOR]);
				if (ap>=rfilesize) wattrset(parent_window,attrs[COLOR_DIFF_CURSOR]);

			} else {
				if (c==buffer[0]) wattrset(parent_window,attrs[COLOR_HEXFIELD]); else wattrset(parent_window,attrs[COLOR_DIFF]);
				if (ap>=rfilesize) wattrset(parent_window,attrs[COLOR_DIFF]);
			}
			if (ch2==0 || ap!=cursorpos) 
			if (ap<filesize) mvwprintw(parent_window,y,(int)f+10+x/2,"%s",tohex(c)); else mvwprintw(parent_window,y,(int)f+10+x/2,"  ");		 
			else if (ch2!=0 && ap==cursorpos)
			{
				mvwprintw(parent_window,y,(int)f+10+x/2,"%c ",ch2);
			}
			wattrset(parent_window,attrs[COLOR_HEXFIELD]);
			mvwprintw(parent_window,y,(int)f+12+x/2," ");	
			if (ap==cursorpos && hexnotasc==0 && ap<=filesize) 
			{
				if (c==buffer[0]) wattrset(parent_window,attrs[COLOR_CURSOR]); else wattrset(parent_window,attrs[COLOR_DIFF_CURSOR]);
			} else {
				if (c==buffer[0]) wattrset(parent_window,attrs[COLOR_HEXFIELD]); else wattrset(parent_window,attrs[COLOR_DIFF]);
			}
			if (ap<filesize) if (c>=32 && c<=127) mvwprintw(parent_window,y,(int)i+(COLS-cols),"%c",(char)c);	else
			mvwprintw(parent_window,y,(int)i+(COLS-cols),".");	else mvwprintw(parent_window,y,(int)i+(COLS-cols)," ");
			ap++;
		}
	}
	
}
void print_hex_diff( WINDOW *parent_window,
	                 file_position_t p,
					 file_position_t cursorpos,
					 file_position_t filesize1,
					 file_position_t filesize2,
					 char* filename2)
{
	unsigned char buffer[2];
	unsigned char buffer2[2];
	float f;
	unsigned int i;
	int x;
	int y;
	int b;
	file_position_t ap=p;
	f=(float)COLS-10;
	f=f/4.125;
	cols=(int)f;
	x=COLS-10-((int)((float)cols*4.125));
	rows=LINES-2;
	b=(LINES-1)/2;
	draw_mainheadline(parent_window,b,filename2);
	wattrset(parent_window,attrs[COLOR_BRACKETS]);
	mvwprintw(parent_window,0,1,"[          /          ]");
	mvwprintw(parent_window,b,1,"[          /          ]");
	wattrset(parent_window,attrs[COLOR_TEXT]);
	mvwprintw(parent_window,0,2,"%10X",(unsigned long)cursorpos);	
	mvwprintw(parent_window,0,13,"%10X",(unsigned long)(filesize1-1));	
	mvwprintw(parent_window,b,2,"%10X",(unsigned long)cursorpos);	
	mvwprintw(parent_window,b,13,"%10X",(unsigned long)(filesize2-1));	
	wattrset(parent_window,attrs[COLOR_HEXFIELD]);
	fseek(inputfile,p,SEEK_SET);
	fseek(inputfile2,p,SEEK_SET);
	for (y=1;y<b;y++)
	{
		wattrset(parent_window,attrs[COLOR_HEXFIELD]);
		print_pos(parent_window,p+(y-1)*cols,y);
		print_pos(parent_window,p+(y-1)*cols,y+b);
		for (i=0;i<cols;i++)
		{
			if (!feof(inputfile)) fgets(buffer,sizeof(buffer),inputfile);
			if (!feof(inputfile2)) fgets(buffer2,sizeof(buffer2),inputfile2);
			// TODO: find a nice and satisfactional way to edit two files at once!
/*
			c=buffer[0];
			
			if (chnum!=0) for (j=0;j<chnum;j++) if (ap==chpos[j]) c=change[j];
			f=(float)i;
			f=f*3.125;
			if (ap==cursorpos && hexnotasc==1 && ap<=filesize) 
			{
				if (c==buffer[0]) wattrset(parent_window,attrs[COLOR_CURSOR]); else wattrset(parent_window,attrs[COLOR_MENU_HOTKEY_HI]);
				if (ap>=rfilesize) wattrset(parent_window,attrs[COLOR_MENU_HOTKEY_HI]);

			} else {
				if (c==buffer[0]) wattrset(parent_window,attrs[COLOR_BRACKETS]); else wattrset(parent_window,attrs[COLOR_MENU_HOTKEY]);
				if (ap>=rfilesize) wattrset(parent_window,attrs[COLOR_MENU_HOTKEY]);
			}
			if (ch2==0 || ap!=cursorpos) 
			if (ap<filesize) mvwprintw(parent_window,y,(int)f+10+x/2,"%s",tohex(c)); else mvwprintw(parent_window,y,(int)f+10+x/2,"  ");		 
			else if (ch2!=0 && ap==cursorpos)
			{
				mvwprintw(parent_window,y,(int)f+10+x/2,"%c ",ch2);
			}
			wattrset(parent_window,attrs[COLOR_BRACKETS]);
			mvwprintw(parent_window,y,(int)f+12+x/2," ");	
			if (ap==cursorpos && hexnotasc==0 && ap<=filesize) 
			{
				if (c==buffer[0]) wattrset(parent_window,attrs[COLOR_CURSOR]); else wattrset(parent_window,attrs[COLOR_MENU_HOTKEY_HI]);
			} else {
				if (c==buffer[0]) wattrset(parent_window,attrs[COLOR_BRACKETS]); else wattrset(parent_window,attrs[COLOR_MENU_HOTKEY]);
			}
			if (ap<filesize) if (c>=32 && c<=127) mvwprintw(parent_window,y,(int)i+(COLS-cols),"%c",(char)c);	else
			mvwprintw(parent_window,y,(int)i+(COLS-cols),".");	else mvwprintw(parent_window,y,(int)i+(COLS-cols)," ");
			*/
			
			f=(float)i;
			f=f*3.125;
			if (buffer[0]!=buffer2[0] || ap>=filesize1 || ap>=filesize2) wattrset(parent_window,attrs[COLOR_DIFF]); else wattrset(parent_window,attrs[COLOR_HEXFIELD]);
			if (ap<filesize1) mvwprintw(parent_window,y,(int)f+10+x/2,"%s",tohex(buffer[0])); else mvwprintw(parent_window,y,(int)f+10+x/2,"  ");
			if (ap<filesize2) mvwprintw(parent_window,y+b,(int)f+10+x/2,"%s",tohex(buffer2[0])); else mvwprintw(parent_window,y+b,(int)f+10+x/2,"  ");
			if (ap<filesize1) if (buffer[0]>=32 && buffer[0]<=127) mvwprintw(parent_window,y,(int)i+(COLS-cols),"%c",(char)buffer[0]); else mvwprintw(parent_window,y,(int)i+(COLS-cols),".");      else mvwprintw(parent_window,y,(int)i+(COLS-cols)," ");
			if (ap<filesize2) if (buffer2[0]>=32 && buffer2[0]<=127) mvwprintw(parent_window,y+b,(int)i+(COLS-cols),"%c",(char)buffer2[0]); else mvwprintw(parent_window,y+b,(int)i+(COLS-cols),".");      else mvwprintw(parent_window,y+b,(int)i+(COLS-cols)," ");
			wattrset(parent_window,attrs[COLOR_HEXFIELD]);
			mvwprintw(parent_window,y,(int)f+12+x/2," ");
			mvwprintw(parent_window,y+b,(int)f+12+x/2," ");
			ap++;
			
		}
	}
	wrefresh(parent_window);
	
}
char* tobin(unsigned int value)
{
	char* s;
	unsigned int i;
	s=malloc(20);
	s[19]=0;
	for (i=0;i<19;i++)
	{
		if (value & (1<<i)) {
		  s[18-i]='1'; 
		} else {
		  s[18-i]='0';
		}
	}
	for (i=0;i<18;i++)
	{
		if (s[i]=='0') s[i]=' '; else i=18;
	}
	return s;
}
char* tooct(unsigned int value)
{
	char *s;
	int i;
	s=malloc(20);
	s[19]=0;
	for (i=0;i<19;i++)
	{
		s[18-i]=48+(value%8);
		value=value/8;
	}
	for (i=0;i<18;i++)
	{
		if (s[i]=='0') s[i]=' '; else i=18;
	}
	return s;
}
char* toint(unsigned int value)
{
	char *s;
	int i;
	s=malloc(20);
	s[19]=0;
	for (i=0;i<19;i++)
	{
		s[18-i]=48+(value%10);
		value=value/10;
	}
	for (i=0;i<18;i++)
	{
		if (s[i]=='0') s[i]=' '; else i=18;
	}
	return s;
}
char* tohex2(unsigned int value,int maxlen)
{
	char *s;
	int i;
	s=malloc(maxlen+1);
	for (i=0;i<maxlen;i++)
	{
		s[maxlen-1-i]=48+(value%16);
		value=value/16;
		if (s[maxlen-1-i]>=58) s[maxlen-1-i]=s[maxlen-1-i]+7;
	}
	for (i=0;i<maxlen-1;i++)
	{
		if (s[i]=='0') s[i]=' '; else i=18;
	}
	return s;
}
int stobin(const char* s)
{
	unsigned int i;
	int v=0;
	for (i=0;i<strlen(s);i++)
	{
		if (s[i]=='0') v=v<<1;
		if (s[i]=='1') v=(v<<1)+1;
	}
	return v;
}
int stooct(const char* s)
{
	unsigned int i;
	int v=0;
	for (i=0;i<strlen(s);i++)
	{
		if (s[i]>='0' && s[i]<='7') v=v*8+(s[i]-48);
	}
	return v;

}
int stoint(const char* s)
{
	unsigned int i;
	int v=0;
	for (i=0;i<strlen(s);i++)
	{
		if (s[i]>='0' && s[i]<='9') v=v*10+(s[i]-48);
	}
	return v;

}
file_position_t stohex(const char* s)
{
	file_position_t i;
	file_position_t v=0;
	for (i=0;i<strlen(s);i++)
	{
		if (s[i]>='0' && s[i]<='9') v=v*16+(s[i]-48);
		if (s[i]>='A' && s[i]<='F') v=v*16+(s[i]-55);
		if (s[i]>='a' && s[i]<='f') v=v*16+(s[i]-87);
	}
	return v;

}
void hexcalc(WINDOW* parent_window)
{
	int wtop;
	int wbot;
	int wleft;
	int wright;
	int m=0;
	long value=45054;
	char* s;
	wtop=LINES/2-3;
	wbot=wtop+7;
	wleft=COLS/2-15;
	wright=wleft+30;
	new_menu(1);
	menu_item(0,wtop+1,wleft+1,"%Binary",'b','B',0);
	menu_item(1,wtop+2,wleft+1,"%%Octal",'o','O',0);
	menu_item(2,wtop+3,wleft+1,"%%Decimal",'d','D',0);
	menu_item(3,wtop+4,wleft+1,"%%Hexadec",'h','H',0);
	menu_item(4,wtop+6,wleft+1,"E%%xit Hexcalc",'x','X',0);
	m=0;
	if (LINES>9 && COLS>30)
	{
		draw_frame(parent_window,wtop,wleft,wbot,wright,' ');
		headline(parent_window,wtop,wleft,"HEXCALC");
		while (m!=4) 
		{
			wattrset(parent_window,attrs[COLOR_BRACKETS]);
			mvwprintw(parent_window,wtop+1,wleft+9,"[                   ]");
			mvwprintw(parent_window,wtop+2,wleft+9,"[                   ]");
			mvwprintw(parent_window,wtop+3,wleft+9,"[                   ]");
			mvwprintw(parent_window,wtop+4,wleft+9,"[                   ]");
			wattrset(parent_window,attrs[COLOR_TEXT]);

			mvwprintw(parent_window,wtop+1,wleft+10,"%s",tobin(value));
			mvwprintw(parent_window,wtop+2,wleft+10,"%19o",value);
			mvwprintw(parent_window,wtop+3,wleft+10,"%19i",value);
			mvwprintw(parent_window,wtop+4,wleft+10,"%19X",value);
			m=menu_show(parent_window);
			if (m==0)
			{
				s=input2(parent_window,wtop+1,wleft+10,19,tobin(value),19,0,0);
				value=stobin(s);
				free(s);
			}
			if (m==1)
			{
				s=input2(parent_window,wtop+2,wleft+10,19,tooct(value),19,0,0);
				value=stooct(s);
				free(s);
			}
			if (m==2)
			{
				s=input2(parent_window,wtop+3,wleft+10,19,toint(value),19,0,0);
				value=stoint(s);
				free(s);
			}
			if (m==3)
			{
				s=input2(parent_window,wtop+4,wleft+10,19,tohex2(value,19),19,0,0);
				value=(int)stohex(s);
				free(s);
			}
		}
		wattrset(parent_window,attrs[COLOR_HEXFIELD]);
		erase_frame(parent_window,wtop,wleft,wbot,wright,' ');

	}
}
void draw_menu(WINDOW* parent_window)
{
	wattrset(parent_window,attrs[COLOR_MENU]);
	mvwprintw(parent_window,LINES-1,1 ,"Search ");
	mvwprintw(parent_window,LINES-1,9 ,"Goto   ");
	mvwprintw(parent_window,LINES-1,17,"HexCalc");
	mvwprintw(parent_window,LINES-1,25,"       "); 
	mvwprintw(parent_window,LINES-1,33,"Next   ");
	mvwprintw(parent_window,LINES-1,41,"Previou");
	mvwprintw(parent_window,LINES-1,49,"       "); 
	mvwprintw(parent_window,LINES-1,57,"       "); 
	mvwprintw(parent_window,LINES-1,65,"UnDo   ");
	mvwprintw(parent_window,LINES-1,73,"Exit   ");
	wattrset(parent_window,attrs[COLOR_MENU_HOTKEY]);
	mvwprintw(parent_window,LINES-1,0,"1");
	mvwprintw(parent_window,LINES-1,8,"2");
	mvwprintw(parent_window,LINES-1,16,"3");
	mvwprintw(parent_window,LINES-1,24,"4");
	mvwprintw(parent_window,LINES-1,32,"5");
	mvwprintw(parent_window,LINES-1,40,"6");
	mvwprintw(parent_window,LINES-1,48,"7");
	mvwprintw(parent_window,LINES-1,56,"8");
	mvwprintw(parent_window,LINES-1,64,"9");
	mvwprintw(parent_window,LINES-1,72,"0");
	
}
void exit_yesno(WINDOW* parent_window,char* filename)
{
	int wtop;
	int wbot;
	int wleft;
	int wright;
	int m,i;
	wtop=LINES/2-2;
	wbot=wtop+4;
	wleft=COLS/2-16;
	wright=wleft+33;
	new_menu(0);

	menu_item(1,wtop+3,wleft+1,"<%%Yes>",'y','Y',0);
	menu_item(2,wtop+3,wleft+15,"<%%No>",'n','N',0);
	menu_item(0,wtop+3,wleft+25,"<%%Cancel>",'d','D',0);

	if (LINES>9 && COLS>32)
	{
		draw_frame(parent_window,wtop,wleft,wbot,wright,' ');
		wattrset(parent_window,attrs[COLOR_TEXT]);
		mvwprintw(parent_window,wtop+1,wleft+1,"Do you want to save the changes?");	
		headline(parent_window,wtop,wleft,"EXIT");
		m=menu_show(parent_window);
		if (m==2) finish(0);
		if (m==1) 
		{
			fclose(inputfile);
			inputfile=fopen(filename,"r+");
			for (i=0;i<chnum;i++)
			{
				fseek(inputfile,chpos[i],SEEK_SET);
				fprintf(inputfile,"%c",change[i]);
			}
			fclose(inputfile);	
			finish(0);
		}
		wattrset(parent_window,attrs[COLOR_HEXFIELD]);
		erase_frame(parent_window,wtop,wleft,wbot,wright,' ');
	}
}
void printsearchstring3(WINDOW* parent_window,int y,int x,int offset)
{
	unsigned int i;
	for (i=0;i<10 ;i++)
	{
		if ((i+offset)<searchstring3len) {
		mvwprintw(parent_window,y,x+i*3,tohex(searchstring3[i+offset]&255));
		mvwprintw(parent_window,y,x+2+i*3," ");
		if ((searchstring3[i+offset]&256)==256) mvwprintw(parent_window,y,x+i*3,".");
		if ((searchstring3[i+offset]&512)==512) mvwprintw(parent_window,y,x+1+i*3,".");
		} else mvwprintw(parent_window,y,x+i*3,"   ");
	}

}
void kmpPreprocesshex()
{
	unsigned int i=0;
	int j=-1;
	kmp[i]=j;
	while ( i < searchstring2len)
	{
		while (j>=0 && searchstring2[i]!=searchstring2[j]) 
		{
			j=kmp[j];
			
		}
		i++; j++; 
		kmp[i]=j;
	}	
}
void kmpPreprocesshexback()
{
	int i=searchstring2len-1;
	int j=-1;
	kmpback[i]=j;
	while (i>=0)
	{
		while (j>=0 && searchstring2[i]!=searchstring2[j]) 
		{
			j=kmpback[j];
			
		}
		i--; j++; 
		kmpback[i]=j;
	}	
}
file_position_t searchforwardhex( file_position_t cursorpos,
	                              file_position_t filesize)
{
	unsigned char buffer[524288];
	FILE *writesearchfile = NULL;
	file_position_t cp=cursorpos;
	file_position_t ocp=cursorpos;
	file_position_t t = 0;
	int i=0;
	int j=0;
	int k=sizeof(buffer);
	kmpPreprocesshex();
	if (writesearch==1) 
	{
		writesearchfile=fopen(writesearchfilename,"w");
		fprintf(writesearchfile,"#DHEXSEARCHFILE\n#VERSION 0\n");
		cp=0;
		ocp=0;
	}
	fseek(inputfile,cp,0);
	while (cp<filesize)
	{
		if (k==sizeof(buffer)) 
		{
			if (!feof(inputfile)) fread(buffer,sizeof(buffer),1,inputfile);
			for (i=0;i<chnum;i++)
			{
				if (chpos[i]>=ocp && chpos[i]<=ocp+sizeof(buffer))
				{
					buffer[chpos[i]-ocp]=change[i];
				}
			}
			ocp=cp;
			k=0;
		}
		while (j>=0 && buffer[k]!=searchstring2[j]) j=kmp[j];
		k++; j++; cp++;
		if ((unsigned int) j == searchstring2len && t!=ocp+k-j)
		  
		{
			//GEFUNDEN
			t=ocp+k-j;
			if (writesearch==1) {
			  fprintf( writesearchfile,"%04X%04X%04X%04X\n", 
				       ((int)((t>>48)&65535)),
					   ((int)((t>>32)&65535)),
					   ((int)((t>>16)&65535)),
					   ((int)(t&65535)));
			} else {
				return t;
			}

			j=kmp[j];
		}
	}
	if (writesearch==1) fclose(writesearchfile);
	return cursorpos;
}
file_position_t searchbackwardhex(file_position_t cursorpos,file_position_t filesize,int hexnotasc)
{
  /* remove me */ filesize = 0;
  /* remove me */ hexnotasc = 0;
	kmpPreprocesshexback();
	
	return cursorpos;
}
file_position_t searchbackwardhex2(file_position_t cursorpos,file_position_t filesize)
{
  /* remove me */ filesize = 0;
	return cursorpos;
}
file_position_t searchforwardhex2(file_position_t cursorpos)
{
	file_position_t cp;
	file_position_t ocp = cursorpos;
	unsigned char buffer[256];
	unsigned char p[524288];
	FILE *writesearchfile = NULL;
	unsigned int i;
	int mismatch;

	if (obenanfangen==1 || writesearch==1 || cursorpos==0) readsearchfile=fopen(readsearchfilename,"r");
	obenanfangen=0;	
	if (writesearch==1) 
	{
		writesearchfile=fopen(writesearchfilename,"w");
		fprintf(writesearchfile,"#DHEXSEARCHFILE\n#VERSION 0\n");
	}	
	while (!feof(readsearchfile))
	{
		fgets(p,sizeof(p),readsearchfile);
		if (p[0]!='#')
		{
			cp=stohex(p);
			fseek(inputfile,cp,SEEK_SET);
			fread(buffer,sizeof(buffer),1,inputfile);
			mismatch=0;
			for (i=0;mismatch==0 && i<searchstring2len;i++) if (buffer[i]!=searchstring2[i]) mismatch=1;
			if (mismatch==0 && cp!=ocp)
			{
				ocp=cp;
				if (writesearch==1) fprintf(writesearchfile,"%04X%04X%04X%04X\n",((int)((cp>>48)&65535)),((int)((cp>>32)&65535)),((int)((cp>>16)&65535)),((int)(cp&65535))); 
				else return cp;
			}
		}
	}
	if (writesearch==1) fclose(writesearchfile);
	if (feof(readsearchfile)) 
	{
		fclose(readsearchfile);	
		obenanfangen=1;
	}
	return cursorpos;	
}
int searchfor(WINDOW* parent_window,int hexnotasc)
{
	int wtop;
	int wbot;
	int wleft;
	int wright;
	int ch;
	char* s;
	int m;
	unsigned int i;
	int cursor;
	unsigned int offset;
	int doit=0;
	wtop=LINES/2-6;
	wbot=wtop+12;
	wleft=COLS/2-16;
	wright=wleft+33;
	m=0;
	new_menu(1);

	if(hexnotasc==1)
	menu_item(0,wtop+1,wleft+1,"%%Searchstring (Hex)",'s','S',0);
	else menu_item(0,wtop+1,wleft+1,"%%Searchstring (Asc)",'s','S',0);
	menu_item(1,wtop+4,wleft+1,"Search %Forward",'f','F',0);
	menu_item(2,wtop+5,wleft+1,"Search %Backwards",'b','B',0);
	menu_item(3,wtop+6,wleft+5,"%Write Result to file",'w','W',0);
	menu_item(4,wtop+7,wleft+1," ",0,0,0);
	menu_item(5,wtop+8,wleft+5,"%Read Searchpos. from file",'r','R',0);
	menu_item(6,wtop+9,wleft+1," ",0,0,0);
	menu_item(7,wtop+11,wleft+1,"%%Cancel",0,0,0);

	if (LINES>10 && COLS>32)
	{
		draw_frame(parent_window,wtop,wleft,wbot,wright,' ');
		if (hexnotasc==1) headline(parent_window,wtop,wleft,"SEARCH HEXSTRING"); else headline(parent_window,wtop,wleft,"SEARCH ASCIISTRING");
		while (m!=1 && m!=2 && m!=7)
		{
			wattrset(parent_window,attrs[COLOR_BRACKETS]);
			mvwprintw(parent_window,wtop+2,wleft+1,"[                              ]"); 
			mvwprintw(parent_window,wtop+7,wleft+2,"[                             ]"); 
			mvwprintw(parent_window,wtop+9,wleft+2,"[                             ]"); 
			mvwprintw(parent_window,wtop+6,wleft+1,"( )");
			mvwprintw(parent_window,wtop+8,wleft+1,"( )");
			wattrset(parent_window,attrs[COLOR_TEXT]);
			if (hexnotasc==1) 
			{
				printsearchstring3(parent_window,wtop+2,wleft+2,0);
			} else
			if (strlen(searchstring)<=30) 
			{
				mvwprintw(parent_window,wtop+2,wleft+2,"%s",searchstring);
			} else {
				for (i=0;i<30;i++)
				{
					mvwprintw(parent_window,wtop+2,wright-2-i,"%c",searchstring[strlen(searchstring)-1-i]);
				}
			}	
			if (writesearch==1) mvwprintw(parent_window,wtop+6,wleft+2,"X"); 
			else {
				wattrset(parent_window,attrs[COLOR_BRACKETS]);
				mvwprintw(parent_window,wtop+6,wleft+2," ");
			}
			
			if (strlen(writesearchfilename)<=29) 
			{
				mvwprintw(parent_window,wtop+7,wleft+3,"%s",writesearchfilename);
			} else {
				for (i=0;i<29;i++)
				{
					mvwprintw(parent_window,wtop+7,wright-2-i,"%c",writesearchfilename[strlen(writesearchfilename)-1-i]);
				}
			}	
			wattrset(parent_window,attrs[COLOR_TEXT]);
			if (readsearch==1) mvwprintw(parent_window,wtop+8,wleft+2,"X"); 
			else {
				wattrset(parent_window,attrs[COLOR_BRACKETS]);
				mvwprintw(parent_window,wtop+8,wleft+2," ");

			}
			if (strlen(readsearchfilename)<=29) 
			{
				mvwprintw(parent_window,wtop+9,wleft+3,"%s",readsearchfilename);
			} else {
				for (i=0;i<29;i++)
				{
					mvwprintw(parent_window,wtop+9,wright-2-i,"%c",readsearchfilename[strlen(readsearchfilename)-1-i]);
				}
			}
			m=menu_show(parent_window);
			if (m==7 || m==1 || m==2) 
			{
				wattrset(parent_window,attrs[COLOR_HEXFIELD]);
				erase_frame(parent_window,wtop,wleft,wbot,wright,' ');
				if (hexnotasc==1 && searchstring2len>0) 
				{
					for (i=searchstring3len-1;i>0;i--)
					{
						if (searchstring3[i]>=768) {
							searchstring3len--; 
						} else i=0;
					}
				}
			}
			if (m==7) return 0;
			if (m==0) 
			{
				if (hexnotasc==1) 
				{
					cursor=0;
					offset=0;
					wattrset(parent_window,attrs[COLOR_CURSOR]);
					ch=0;
					while (ch!=13)
					{
						printsearchstring3(parent_window,wtop+2,wleft+2,offset);
						wmove(parent_window,wtop+2,wleft+2+cursor);	
						ch=0;
						while ((ch!=8) && (ch!=KEY_BACKSPACE) &&  (ch!='.') && (ch!=13) && (ch!=KEY_RIGHT) && (ch!=KEY_LEFT) && ((ch<'0') || (ch>'9')) && ((ch<'A') || (ch>'F'))  && ((ch<'a') || (ch>'f'))) 
						ch=getch2();
						if (ch=='.') ch=0; // TODO: how do i handle wildcards with kmp?
						if ((ch==8) || (ch==KEY_BACKSPACE))
						{
							if (cursor/3+offset>0) 
							{
								for (i=(cursor/3+offset);i<searchstring3len;i++)
								{
									searchstring3[i]=searchstring3[i+1];
								}
								cursor=cursor-2;	
								searchstring3len--;
								ch=KEY_LEFT;
							}
							
							
						}
						if (((ch>='0') && (ch<='9')) || ((ch>='a') && (ch<='f')) || ((ch>='A') && (ch<='F')))
						{
							if ((ch>='0') && (ch<='9')) ch=ch-48;
							if ((ch>='A') && (ch<='F')) ch=ch-55;
							if ((ch>='a') && (ch<='f')) ch=ch-87;

							i=cursor/3+offset;
							if ((cursor%3)==0) 
							{
								searchstring3[i]=(searchstring3[i]&527)+(ch<<4);
								if ((unsigned int) (cursor/3+offset)==searchstring3len)
								{
									searchstring3[i]=searchstring3[i]&240;
									searchstring3len++;
								}
							}
							if ((cursor%3)==1) searchstring3[i]=(searchstring3[i]&496)+ch;
							ch=KEY_RIGHT;
						}
						if (ch=='.') 
						{
							i=cursor/3+offset;
							if ((cursor%3)==0) 
							{
								if ((searchstring3[i]&256)!=256) searchstring3[i]=searchstring3[i]|256;
								if ((unsigned int) (cursor/3+offset)==searchstring3len)
								{
									searchstring3[i]=768;
									searchstring3len++;
								}

							}
							if ((cursor%3)==1) if ((searchstring3[i]&512)!=512) searchstring3[i]=searchstring3[i]|512;
							ch=KEY_RIGHT;
						}
						if ((ch==KEY_RIGHT) && ((((unsigned int)cursor/3+offset) <searchstring3len))) {
						  cursor++;
						  if ((cursor%3)==2) cursor++;
						}
						if ((ch==KEY_LEFT)) {
							cursor--;
							if ((cursor%3)==2) cursor--;
						}
						if (cursor>=30) 
						{
							cursor=27;
							offset=offset+1;
							if ( offset >= searchstring3len-9) {
							  if (searchstring3len>9) {
								offset=searchstring3len-9; 
							  } else {
								offset=0;
							  }
							}
						}
						if (cursor<0) 
						{
							cursor=0;
							if (offset) {
								offset--;
							}
						}
					}
					
				} else {
					s=malloc(strlen(searchstring)+1);
					strncpy(s,searchstring,strlen(searchstring)+1);
					free(searchstring);
					searchstring=input2(parent_window,wtop+2,wleft+2,29,s,255,0,0);
					free(s);	
				}
			}
			if (m==1) return KEY_F(5);
			if (m==2) return KEY_F(6);
			if (m==3) writesearch=1-writesearch;
			if (m==4) 
			{
				doit=1;
				while ((strcmp(readsearchfilename,writesearchfilename)==0 && strlen(readsearchfilename)>0) || (doit==1))	
				{
					doit=0;
					s=malloc(strlen(writesearchfilename)+1);
					strncpy(s,writesearchfilename,strlen(writesearchfilename)+1);
					free(writesearchfilename);
					writesearchfilename=input2(parent_window,wtop+7,wleft+3,28,s,255,0,0);
					free(s);
					wattrset(parent_window,attrs[COLOR_TEXT]);
					mvwprintw(parent_window,wtop+10,wleft+2,"Please choose a different name");
				} 
			}
			if (m==5) readsearch=1-readsearch;
			if (m==6) 
			{
				doit=1;
				while ((strcmp(readsearchfilename,writesearchfilename)==0 && strlen(readsearchfilename)>0) || (doit==1))	
				{
					doit=0;
					s=input2(parent_window,wtop+9,wleft+3,28,readsearchfilename,255,0,0);
					free(readsearchfilename);
					readsearchfilename=malloc(strlen(s));
					strncpy(readsearchfilename,s,strlen(s)+1);
					free(s);
					wattrset(parent_window,attrs[COLOR_TEXT]);
					mvwprintw(parent_window,wtop+10,wleft+2,"Please choose a different name");
				}
			}
			if (m==4 || m==6)
			{
				wattrset(parent_window,attrs[COLOR_FRAME]);
				mvwprintw(parent_window,wtop+10,wleft+2,"                              ");
			}
			
		}
		
	}
	return 0;
}
int gotowhere( WINDOW* parent_window,
	           file_position_t ap, 
	           file_position_t ap2,
			   file_position_t filesize)
{
	int wtop;
	int wbot;
	int wleft;
	int wright;
	char* s;
	file_position_t value;
	int m;
	unsigned int i;
	int plus=0;
	int minus=0;
	wtop=LINES/2-3;
	wbot=wtop+5;
	wleft=COLS/2-16;
	wright=wleft+33;
	new_menu(0);

	menu_item(1,wtop+4,wleft+1,"%To:",'t','T',0);
	menu_item(2,wtop+4,wleft+21,"%%Goto",'g','G',0);
	menu_item(0,wtop+4,wright-6,"%%Cancel",'c','C',0);	
	if (LINES>9 && COLS>32)
	{
		draw_frame(parent_window,wtop,wleft,wbot,wright,' ');
		headline(parent_window,wtop,wleft,"GOTO");
		wattrset(parent_window,attrs[COLOR_TEXT]);
		mvwprintw(parent_window,wtop+1,wleft+1,"Goto where? (Enter +x or -x to");
		mvwprintw(parent_window,wtop+2,wleft+1,"jump relative to your position)");
		m=1;
		while (m==1)
		{
			wattrset(parent_window,attrs[COLOR_BRACKETS]);
			mvwprintw(parent_window,wtop+4,wleft+5,"[           ]");	
			wattrset(parent_window,attrs[COLOR_TEXT]);
			mvwprintw(parent_window,wtop+4,wleft+6,"%11X",ap);	
			m=menu_show(parent_window);	
		
			if (m==1)
			{
//				s=input2(parent_window,wtop+4,wleft+6,11,tohex2(ap2,10),11,0,0);// TODO: er zeigt mir hier mist an. warum??
				s=input2(parent_window,wtop+4,wleft+6,11,"\0",11,0,0);// TODO: er zeigt mir hier mist an. warum??
				plus=0;
				minus=0;
				value=(int)stohex(s);
				for (i=0;i<strlen(s);i++)
				{
					if (s[i]=='-' || s[i]=='m') {value=ap2-value;i=20;}
					if (s[i]=='+' || s[i]=='p') {value=ap2+value;i=20;}
				}
				if (value<=filesize) ap=value;
				free(s);
			} else if (m==0) ap=ap2;
		}
		erase_frame(parent_window,wtop,wleft,wbot,wright,' ');
	}
	return ap;	
}
//#ifndef fpos_t
//#define fpos_t file_position_t
//#endif
int main(int argc,char *argv[])
{
	int hexnotasc=1;
	int ch;
	int ch2=0;
	file_position_t p=0;

	file_position_t cp=p;
	file_position_t filesize;
	file_position_t rfilesize;
	file_position_t filesize2 = 0;
	file_position_t rfilesize2;
	file_position_t ap2;
    	fpos_t help;
	unsigned char c1,c2;
	file_position_t tmpp;

	unsigned int i;
	int j;
	
	searchstring=malloc(1);
	writesearchfilename=malloc(1);
	readsearchfilename=malloc(1);
	searchstring[0]=0;
	writesearchfilename[0]=0;
	readsearchfilename[0]=0;
	if (argc<2)
	{
		fprintf(stderr,"Please run with %s [inputfile] or %s [inputfile] [diffile]\n",argv[0],argv[0]);
		exit(1);
	}
	if ((strcmp(argv[1],"-gpl")==0)||(strcmp(argv[1],"-GPL")==0)) {
		print_gpl();	
		exit(0);
	}
	inputfile=fopen(argv[1],"r");
	if (inputfile==NULL) 
	{
		fprintf(stderr,"Error opening inputfile [%s]\n",argv[1]);
		exit(1);
	}
	fseek(inputfile,0,SEEK_END);
	fgetpos(inputfile, &help);
#ifdef LINUX
	filesize = *((file_position_t*) &help);
#else
	filesize=(file_position_t)help;
#endif
//	filesize=100;
	rfilesize=filesize;
	fseek(inputfile,0,SEEK_SET);
	if (argc>=3)
	{
		inputfile2=fopen(argv[2],"r");
		if (inputfile2==NULL) 
		{
			fprintf(stderr,"Error opening diffile [%s]\n",argv[2]);
			exit(1);
		}
		fseek(inputfile2,0,SEEK_END);
		fgetpos(inputfile2,&help);
#ifdef LINUX
		filesize2= *((file_position_t*) &help);
#else
		filesize2= (file_position_t) help;
#endif
		rfilesize2=filesize2;
		fseek(inputfile2,0,SEEK_SET);
		diffnotedit=1;
	}
//	while (!feof(inputfile)) fgets(NULL,1000,inputfile);
	uimain();
	//init();
	wclear(stdscr);
	wrefresh(stdscr);
	
	init_colors();
	wattrset(stdscr,attrs[COLOR_HEXFIELD]);
	for (i=1; (int) i < LINES-1;i++) {
	  for (j=0; j < COLS;j++) {
		mvwprintw(stdscr,i,j," ");
	  }
	}
	
	for (;;)
	{	
		draw_mainheadline(stdscr,0,argv[1]);
		wattrset(stdscr,attrs[COLOR_HEXFIELD]);
		if (diffnotedit==0) {
		  print_hex(stdscr,p,cp,filesize,rfilesize,hexnotasc,ch2); 
		} else {
		  print_hex_diff(stdscr,p,p,filesize,filesize2,argv[2]);
		}
		draw_menu(stdscr);
		ch=getch2();
		if (hexnotasc==1)
		{
			if (ch=='!') ch=KEY_F(1);
			if (ch=='@') ch=KEY_F(2);
			if (ch=='#') ch=KEY_F(3);
			if (ch=='%') ch=KEY_F(5);
			if (ch=='^') ch=KEY_F(6);
			if (ch=='(') ch=KEY_F(9);
			if (ch==')') ch=KEY_F(10);
			if (ch=='h') ch=KEY_LEFT;
			if (ch=='j') ch=KEY_DOWN;
			if (ch=='k') ch=KEY_UP;
			if (ch=='l') ch=KEY_RIGHT;
			if (ch==' ') ch=KEY_NPAGE;
		}
		if (diffnotedit==1 && ch!=KEY_RETURN && ch!=9 && ch!=KEY_BTAB && ch!=KEY_LEFT && ch!=KEY_RIGHT && ch!=KEY_UP && ch!=KEY_DOWN && ch!=KEY_NPAGE && ch!=KEY_PPAGE && ch!=KEY_F(2) && ch!=KEY_F(3) &&  ch!=KEY_F(10)) ch=0;
		if ((hexnotasc==1) && (((ch>='0') && (ch<='9')) || ((ch>='a') && (ch<='f')) || ((ch>='A') && (ch<='F')))) 
		{
			mvwprintw(stdscr,1,1,"h");
			if (ch2==0) 
			{
				
				ch2=ch;
			} else {
				if (ch2<='9') change[chnum]=(ch2-48)<<4;
				else if (ch2<='F') change[chnum]=(ch2-55)<<4;
				else if (ch2<='f') change[chnum]=(ch2-87)<<4;
				if (ch<='9') change[chnum]=change[chnum]+(ch-48);
				else if (ch<='F') change[chnum]=change[chnum]+(ch-55);
				else if (ch<='f') change[chnum]=change[chnum]+(ch-87);
				chpos[chnum]=cp;
				chnum++;
				ch2=0;
				ch=KEY_RIGHT;
				if (cp==filesize) filesize++;
			}
		} else ch2=0;
		if ((hexnotasc==0) && (ch>=32) && (ch<=127)) 
		{
			chpos[chnum]=cp;
			change[chnum]=ch;
			chnum++;
			ch=KEY_RIGHT;
		}
		if (diffnotedit==0 && (ch==KEY_BTAB || ch==9)) hexnotasc=1-hexnotasc;
		if (ch==12 || ch==KEY_F(11) || ch==KEY_REFRESH) {
			wattrset(stdscr,attrs[COLOR_HEXFIELD]);
			wclear(stdscr);
			wrefresh(stdscr);
			wattrset(stdscr,attrs[COLOR_HEXFIELD]);
			for (i=1; (int) i < LINES-1;i++) {
			  for (j=0;j<COLS;j++) {
				mvwprintw(stdscr,i,j," ");
			  }
			}
		}
		if (diffnotedit==0) 
		{
		//	if (ch==KEY_LEFT && cp!=0) {cp--;if (cp<p) p--;}
			if (ch==KEY_DOWN && cp+cols<filesize) {cp=cp+cols;if (cp>=p+rows*cols) p=p+cols;}
			if (ch==KEY_UP && cp>=cols) {
			  cp=cp-cols;
			  if (cp<p) {
				if (p>cols) {
				  p=p-cols;
				} else {
				  p=0;
				}
			  }
			}
			if (ch==KEY_PPAGE && cp>=cols*rows) {
			  cp=cp-cols*rows;
			  if (p>cols*rows) {
				p=p-cols*rows; 
			  } else {
				p = 0;
				if (p) {
					cp = 0;
				}
			  }
			}
			if (ch==KEY_NPAGE && p+cols*rows<=filesize && cp+cols*rows<=filesize+1) {p=p+cols*rows;cp=cp+cols*rows;}
		} else {
			if (ch==KEY_LEFT && p!=0) {p--;}
			if (ch==KEY_DOWN && ((p+cols<filesize) || (p+cols<filesize2))) {p=p+cols;}
			if (ch==KEY_UP && p>=cols) {p=p-cols;}
			if (ch==KEY_PPAGE && p>=cols*rows/2) {p=p-cols*rows/2;}
			if (ch==KEY_NPAGE && ((p+cols*rows/2<=filesize) || (p+cols*rows/2<=filesize2))) {p=p+cols*rows/2;}
			if (ch==KEY_RIGHT && ((p<filesize) || (p<filesize2))) p++;
			if (ch==KEY_BTAB || ch==9 || ch==KEY_RETURN) 
			{
				tmpp=p+1;
			        fseek(inputfile,tmpp,SEEK_SET);
       				fseek(inputfile2,tmpp,SEEK_SET);
				c1=0;
				c2=0;
				while (!feof(inputfile)&&!feof(inputfile2) && (c1==c2))
				{
					fread(&c1,sizeof(char),1,inputfile);
					fread(&c2,sizeof(char),1,inputfile2);
					if (!feof(inputfile)&&!feof(inputfile2)) if (c1==c2) tmpp++;
				}
				if (c1!=c2) p=tmpp;
			}
		}
		if (ch==KEY_F(1))
		{
			ch=searchfor(stdscr,hexnotasc);
		}
		if (ch==KEY_F(2))
		{
			ap2=gotowhere(stdscr,cp,cp,filesize);
			if (ap2<=filesize)
			{
				p=ap2;
				cp=ap2;
			}
//			wclear(stdscr);
			wrefresh(stdscr);
		}
		if (ch==KEY_F(3)) 
		{
			hexcalc(stdscr);
//			wclear(stdscr);
//			wrefresh(stdscr);
		}
		if (ch==KEY_F(5))
		{
			if (hexnotasc==0) 
			{
				searchstring2len=strlen(searchstring);
				for (i=0;i<strlen(searchstring);i++) {
				  searchstring2[i]=(int)searchstring[i];
				}
				
			} else {
				searchstring2len=searchstring3len;
				for (i=0;i<searchstring3len;i++) {
				  searchstring2[i]=searchstring3[i];
				}

			}
			if (readsearch==0) {
			  cp=searchforwardhex(cp,filesize); 
			} else { 
			  cp=searchforwardhex2(cp);
			}
			p=cp;
			if (writesearch==1)
			{
				p=0;
				cp=0;
			}
			ch=KEY_RIGHT;	
		}
		if (ch==KEY_F(6))
		{
			if (hexnotasc==0) 
			{
				searchstring2len=strlen(searchstring);
				for (i=0;i<strlen(searchstring);i++) searchstring2[i]=(int)searchstring[i];
				
			} else {
				searchstring2len=searchstring3len;
				for (i=0;i<searchstring3len;i++) searchstring2[i]=searchstring3[i];

			}
			if (readsearch==0) cp=searchbackwardhex(cp,filesize,hexnotasc); else cp=searchbackwardhex2(cp,filesize);
			p=cp;
			if (writesearch==1)
			{
				p=0;
				cp=0;
			}
			ch=KEY_LEFT;	
			
		}
		if (diffnotedit==0 && ch==KEY_RIGHT && cp<filesize) {cp++; if (cp>=p+rows*cols) p++; }
		if (diffnotedit==0 && ch==KEY_LEFT && cp!=0) {cp--;if (cp<p) p--;}
		if (ch==KEY_F(9))
		{
			if (chnum!=0) 
			{
				if (chpos[chnum-1]>rfilesize) 
				{
					filesize=rfilesize;
					for (i=0; (int) i < chnum-1; i++) {
					  if (filesize<chpos[i]) {
						filesize=chpos[i]; 
					  }
					}
				}
				if (p>chpos[chnum-1] || p+cols*rows<chpos[chnum-1]) p=chpos[chnum-1];
				if (cp>chpos[chnum-1] || cp+cols*rows<chpos[chnum-1]) cp=chpos[chnum-1];
				if (p>filesize) p=filesize;
				if (cp>filesize) cp=filesize;
				chnum--;
			}
		}
		if (ch==KEY_F(10)) 
		{
			if (chnum!=0) exit_yesno(stdscr,argv[1]); else finish(0);
//			wclear(stdscr);
			wrefresh(stdscr);
		}
		
	}
}
