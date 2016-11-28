#include <stdlib.h>           // exit()

#include <ncurses.h>
#include <signal.h>
#include <string.h>
#include "data.h"
#include "ui.h"
#include "gpl.h"

void byebye(int sig)
{
	fprintf(stderr,"*** DHEX %i.%i%i\n",MAJOR_VERSION,MINOR_VERSION,REVISION);
	fprintf(stderr,"*** (C)opyleft 2k4 by Dettus\n");
	fprintf(stderr,"*** dettus@dettus.net (include DHEX somewhere in the subject)\n\n");
	fprintf(stderr,"(start it with dhex -gpl to see the license)\n\n\n");
	ende();
	exit(sig);
}
int printable(int ch)
{
/*	if (ch>='a' && ch<='z') return 1;
	if (ch>='A' && ch<='Z') return 1;
	if (ch>='0' && ch<='9') return 1;
	if (ch=='+' || ch=='.' || ch==',' || ch==';' || ch==':' || ch=='-' || ch=='_' || ch=='$') return 1;
	if (ch==' ') return 1;
*/
	if (ch>=32 && ch<=127) return 1;
	return 0;
}
int getch2()
{
	int ch=getch();
	if (ch==KEY_CANCEL) ch=KEY_F(11);
	if (ch==KEY_HELP) ch=KEY_F(12);
		if (ch==27)
		{
// TODO: dies ist ein quick and dirty hack.
			ch=getch();
			if (ch==91) {
				timeout(0);
				ch=getch();
				if (ch==55) ch=KEY_A1;
				if (ch==56) ch=KEY_C1;
				timeout(-1);	
			}
			timeout(0);
			getch();
			getch();
			getch();
			getch();
			getch();
			getch();
			timeout(-1);
			
		}
	lastkey=ch;
	return ch;
	
}
int contains(const char* buffer,const char* text)
{
        unsigned int i;
        int l=strlen(text);
        char* buffer2=malloc(l+1);
        buffer2[l]=0;
        for (i=0;i<strlen(buffer);i++)
        {
                strncpy(buffer2,buffer+i,l);
                if (strncmp(buffer2,text,l)==0)
                {
                        free(buffer2);
                        return 1;
                }

        }
        free(buffer2);
        return 0;
}

int searchcolor(const char* buffer,int deffg,int defbg,int num)
{
        if (contains(buffer,"FG=BLACK")==1) deffg=COLOR_BLACK;
        if (contains(buffer,"FG=RED")==1) deffg=COLOR_RED;
        if (contains(buffer,"FG=GREEN")==1) deffg=COLOR_GREEN;
        if (contains(buffer,"FG=BROWN")==1) deffg=COLOR_YELLOW;
        if (contains(buffer,"FG=YELLOW")==1) deffg=COLOR_YELLOW;
        if (contains(buffer,"FG=BLUE")==1) deffg=COLOR_BLUE;
        if (contains(buffer,"FG=PURPLE")==1) deffg=COLOR_MAGENTA;
        if (contains(buffer,"FG=MAGENTA")==1) deffg=COLOR_MAGENTA;
        if (contains(buffer,"FG=CYAN")==1) deffg=COLOR_CYAN;
        if (contains(buffer,"FG=WHITE")==1) deffg=COLOR_WHITE;

        if (contains(buffer,"FG=LIGHTBLACK")==1) deffg=COLOR_BLACK;
        if (contains(buffer,"FG=LIGHTRED")==1) deffg=COLOR_RED;
        if (contains(buffer,"FG=LIGHTGREEN")==1) deffg=COLOR_GREEN;
        if (contains(buffer,"FG=LIGHTBROWN")==1) deffg=COLOR_YELLOW;
        if (contains(buffer,"FG=LIGHTYELLOW")==1) deffg=COLOR_YELLOW;
        if (contains(buffer,"FG=LIGHTBLUE")==1) deffg=COLOR_BLUE;
        if (contains(buffer,"FG=LIGHTPURPLE")==1) deffg=COLOR_MAGENTA;
        if (contains(buffer,"FG=LIGHTMAGENTA")==1) deffg=COLOR_MAGENTA;
        if (contains(buffer,"FG=LIGHTCYAN")==1) deffg=COLOR_CYAN;
        if (contains(buffer,"FG=LIGHTWHITE")==1) deffg=COLOR_WHITE;

        if (contains(buffer,"BG=BLACK")==1) defbg=COLOR_BLACK;
        if (contains(buffer,"BG=RED")==1) defbg=COLOR_RED;
        if (contains(buffer,"BG=GREEN")==1) defbg=COLOR_GREEN;
        if (contains(buffer,"BG=BROWN")==1) defbg=COLOR_YELLOW;
        if (contains(buffer,"BG=YELLOW")==1) defbg=COLOR_YELLOW;
        if (contains(buffer,"BG=BLUE")==1) defbg=COLOR_BLUE;
        if (contains(buffer,"BG=PURPLE")==1) defbg=COLOR_MAGENTA;
        if (contains(buffer,"BG=MAGENTA")==1) defbg=COLOR_MAGENTA;
        if (contains(buffer,"BG=CYAN")==1) defbg=COLOR_CYAN;
        if (contains(buffer,"BG=WHITE")==1) defbg=COLOR_WHITE;

	if (contains(buffer,"BG=LIGHTBLACK")==1) defbg=COLOR_BLACK;
        if (contains(buffer,"BG=LIGHTRED")==1) defbg=COLOR_RED;
        if (contains(buffer,"BG=LIGHTGREEN")==1) defbg=COLOR_GREEN;
        if (contains(buffer,"BG=LIGHTBROWN")==1) defbg=COLOR_YELLOW;
        if (contains(buffer,"BG=LIGHTYELLOW")==1) defbg=COLOR_YELLOW;
        if (contains(buffer,"BG=LIGHTBLUE")==1) defbg=COLOR_BLUE;
        if (contains(buffer,"BG=LIGHTPURPLE")==1) defbg=COLOR_MAGENTA;
        if (contains(buffer,"BG=LIGHTMAGENTA")==1) defbg=COLOR_MAGENTA;
        if (contains(buffer,"BG=LIGHTCYAN")==1) defbg=COLOR_CYAN;
        if (contains(buffer,"BG=LIGHTWHITE")==1) defbg=COLOR_WHITE;

        init_pair(num,deffg,defbg);
        return COLOR_PAIR(num);
}

int searchattrs(const char* buffer)
{
	int defattr=0;
        if (contains(buffer,"UNDERLINE")==1) defattr=defattr|A_UNDERLINE;
        if (contains(buffer,"REVERSE")==1) defattr=defattr|A_REVERSE;
        if (contains(buffer,"BLINK")==1) defattr=defattr|A_BLINK;
        if (contains(buffer,"DIM")==1) defattr=defattr|A_DIM;
        if (contains(buffer,"BOLD")==1) defattr=defattr|A_BOLD;
        if (contains(buffer,"FG=LIGHT")==1) defattr=defattr|A_BOLD;
        if (contains(buffer,"BG=LIGHT")==1) defattr=defattr|A_BLINK;
        return defattr;
}

void init_colors()
{
    FILE *f;
    unsigned char buffer[524288];
	unsigned char *b2;
	unsigned char b3[1032];
    unsigned int i;
    buffer[0]=0;
    attrs[COLOR_BRACKETS]=searchcolor(buffer,COLOR_BLACK,COLOR_BLACK,COLOR_BRACKETS)+A_BOLD;
    attrs[COLOR_HEXFIELD]=searchcolor(buffer,COLOR_BLACK,COLOR_BLACK,COLOR_HEXFIELD)+A_BOLD;
    attrs[COLOR_INPUT]=searchcolor(buffer,COLOR_BLACK,COLOR_WHITE,COLOR_INPUT);
    attrs[COLOR_CURSOR]=searchcolor(buffer,COLOR_BLACK,COLOR_WHITE,COLOR_CURSOR);
    attrs[COLOR_TEXT]=searchcolor(buffer,COLOR_WHITE,COLOR_BLACK,COLOR_TEXT)+A_BOLD;
    attrs[COLOR_MENU]=searchcolor(buffer,COLOR_CYAN,COLOR_BLACK,COLOR_MENU);
    attrs[COLOR_MENU_HI]=searchcolor(buffer,COLOR_BLACK,COLOR_CYAN,COLOR_MENU_HI);
    attrs[COLOR_MENU_HOTKEY]=searchcolor(buffer,COLOR_YELLOW,COLOR_BLACK,COLOR_MENU_HOTKEY)+A_BOLD;
    attrs[COLOR_MENU_HOTKEY_HI]=searchcolor(buffer,COLOR_YELLOW,COLOR_CYAN,COLOR_MENU_HOTKEY_HI)+A_BOLD;
    attrs[COLOR_FRAME]=searchcolor(buffer,COLOR_BLUE,COLOR_BLACK,COLOR_FRAME)+A_BOLD;
    attrs[COLOR_DIFF]=searchcolor(buffer,COLOR_YELLOW,COLOR_BLACK,COLOR_DIFF)+A_BOLD;
    attrs[COLOR_DIFF_CURSOR]=searchcolor(buffer,COLOR_YELLOW,COLOR_WHITE,COLOR_DIFF_CURSOR)+A_BOLD;
    attrs[COLOR_HEADLINE]=searchcolor(buffer,COLOR_BLACK,COLOR_CYAN,COLOR_HEADLINE);
	b2=getenv("HOME");
	for (i=0;i<strlen(b2);i++) {
	  b3[i]=b2[i];
	}
	b3[i++]='/';
	b3[i++]='.';
	b3[i++]='d';
	b3[i++]='h';
	b3[i++]='e';
	b3[i++]='x';
	b3[i++]='r';
	b3[i++]='c';
	b3[i++]=0;

    f=fopen(b3,"r");
	if (f!=NULL)
	{
	while (!feof(f))
       	{
       	        fgets(buffer,sizeof(buffer),f);
       	        if (buffer[0]!='#')
                {
                        for (i=0;i<strlen(buffer) && i<sizeof(buffer);i++) if (buffer[i]>='a' && buffer[i]<='z') buffer[i]=buffer[i]-32;
                        if (contains(buffer,"BRACKETS")==1) attrs[COLOR_BRACKETS]=searchcolor(buffer,COLOR_BLACK,COLOR_BLACK,COLOR_BRACKETS)+searchattrs(buffer);
                        if (contains(buffer,"HEXFIELD")==1) attrs[COLOR_HEXFIELD]=searchcolor(buffer,COLOR_BLACK,COLOR_BLACK,COLOR_HEXFIELD)+searchattrs(buffer);
                        if (contains(buffer,"INPUT")==1) attrs[COLOR_INPUT]=searchcolor(buffer,COLOR_BLACK,COLOR_WHITE,COLOR_INPUT)+searchattrs(buffer);
                        if (contains(buffer,"CURSOR:")==1) attrs[COLOR_CURSOR]=searchcolor(buffer,COLOR_BLACK,COLOR_WHITE,COLOR_CURSOR)+searchattrs(buffer);
                        if (contains(buffer,"TEXT")==1) attrs[COLOR_TEXT]=searchcolor(buffer,COLOR_WHITE,COLOR_BLACK,COLOR_TEXT)+searchattrs(buffer);
                        if (contains(buffer,"MENU_NORMAL")==1) attrs[COLOR_MENU]=searchcolor(buffer,COLOR_CYAN,COLOR_BLACK,COLOR_MENU)+searchattrs(buffer);
                        if (contains(buffer,"MENU_HI")==1) attrs[COLOR_MENU_HI]=searchcolor(buffer,COLOR_BLACK,COLOR_CYAN,COLOR_MENU_HI)+searchattrs(buffer);
                        if (contains(buffer,"MENU_HOTKEY:")==1) attrs[COLOR_MENU_HOTKEY]=searchcolor(buffer,COLOR_YELLOW,COLOR_BLACK,COLOR_MENU_HOTKEY)+searchattrs(buffer);
                        if (contains(buffer,"MENU_HOTKEY_HI")==1) attrs[COLOR_MENU_HOTKEY_HI]=searchcolor(buffer,COLOR_YELLOW,COLOR_CYAN,COLOR_MENU_HOTKEY_HI)+searchattrs(buffer);
                        if (contains(buffer,"FRAME")==1) attrs[COLOR_FRAME]=searchcolor(buffer,COLOR_BLUE,COLOR_BLACK,COLOR_FRAME)+searchattrs(buffer);
                        if (contains(buffer,"NORMAL_DIFF")==1) attrs[COLOR_DIFF]=searchcolor(buffer,COLOR_YELLOW,COLOR_BLACK,COLOR_DIFF)+searchattrs(buffer);
                        if (contains(buffer,"CURSOR_DIFF")==1) attrs[COLOR_DIFF_CURSOR]=searchcolor(buffer,COLOR_YELLOW,COLOR_WHITE,COLOR_DIFF_CURSOR)+searchattrs(buffer);
                        if (contains(buffer,"HEADLINE")==1) attrs[COLOR_HEADLINE]=searchcolor(buffer,COLOR_BLACK,COLOR_CYAN,COLOR_HEADLINE)+searchattrs(buffer);

                }
	}
	fclose(f);
	} else {
		f=fopen(b3,"w");
		if (f!=NULL) 
		{
			fprintf(f,"#DHEXCOLORSCHEME\n#VERSION 0\n#possible colors are: BLACK,RED,GREEN,YELLOW,BLUE,MAGENTA,CYAN,WHITE\n#possible extra flags are: UNDERLINE,REVERSE,BLINK,DIM,BOLD\n\n");
			fprintf(f,"BRACKETS:       FG=BLACK,BG=BLACK,BOLD\n");
			fprintf(f,"HEXFIELD:       FG=BLACK,BG=BLACK,BOLD\n");
			fprintf(f,"INPUT:          FG=BLACK,BG=WHITE\n");
			fprintf(f,"CURSOR:         FG=BLACK,BG=WHITE\n");
			fprintf(f,"TEXT:           FG=WHITE,BG=BLACK,BOLD\n");
			fprintf(f,"MENU_NORMAL:    FG=CYAN,BG=BLACK\n");
			fprintf(f,"MENU_HIGHLIGHT: FG=BLACK,BG=CYAN\n");
			fprintf(f,"MENU_HOTKEY:    FG=YELLOW,BG=BLACK,BOLD\n");
			fprintf(f,"MENU_HOTKEY_HI: FG=YELLOW,BG=CYAN,BOLD\n");
			fprintf(f,"FRAME:          FG=BLUE,BG=BLACK,BOLD\n");
			fprintf(f,"NORMAL_DIFF:    FG=YELLOW,BG=BLACK,BOLD\n");
			fprintf(f,"CURSOR_DIFF:    FG=YELLOW,BG=WHITE,BOLD\n");
			fprintf(f,"HEADLINE:       FG=BLACK,BG=CYAN\n");

			fclose(f);
		}
		
	}
}
char *input2(WINDOW *parent_window,int y,int x, unsigned int len,const char *text, unsigned int max,int special,int firstch)
{
	unsigned int cursorpos=0;
	unsigned int offset=0;
	int l=0;
	char *newtext;
	char *textfield;
	char *spaces;
	char *tmp;
	unsigned int i;
	int ch=0;
	WINDOW *inputfield=newpad(1,len);
	WINDOW *fulltext=newpad(1,max);
	wattrset(inputfield,attrs[COLOR_INPUT]);
	wattrset(parent_window,attrs[COLOR_INPUT]);


	l=strlen(text)+1;
		
	newtext=malloc(l);
	strncpy(newtext,text,l);
	spaces=malloc(max+1);
	for (i=0;i<max;i++)
	{
		spaces[i]=' ';
	}
	spaces[max]=0;
	textfield=malloc(l);
	if (strlen(newtext) < len)
	{
		offset=0;
		cursorpos=strlen(newtext);
	} else {
		cursorpos=len;
		offset=strlen(newtext)-len;
	}
	while (ch!=KEY_ESC && ch!=KEY_RETURN)
	{
		free(textfield);	
		textfield=malloc(max+1);
		strncpy(textfield,spaces,max+1);
		strncpy(textfield,newtext,strlen(newtext));

		wattrset(fulltext,attrs[COLOR_INPUT]);
		mvwprintw(fulltext,0,0,"%s",textfield);
		wrefresh(fulltext);	
		
//		copywin(fulltext,parent_window,0,offset,y,x,y,x+len,1);
		wmove(parent_window,y,x);
		for (i=0;i<max;i++)
		{
			if ( i >= offset && i <= offset+len) {
			  waddch(parent_window,textfield[i]);
			}
		}

		wmove(parent_window,y,x+cursorpos);
		wrefresh(inputfield);
		wrefresh(parent_window);
		if (firstch==0) ch=getch2(); else {ch=firstch;firstch=0;}
		if (special==1 && (ch==KEY_UP || ch==KEY_DOWN || ch==KEY_LEFT || ch==KEY_RIGHT || ch==KEY_A1 || ch==KEY_C1)) {ch=KEY_RETURN;}
		if (ch==KEY_A1)
		{
			cursorpos=0;
			offset=0;
			ch=0;
		}
		if (ch==KEY_C1)
		{
			if (strlen(newtext)<len)
			{
				offset=0;
				cursorpos=strlen(newtext);
			} else {
				cursorpos=len;
				offset=strlen(newtext)-len;
			}
	
		}
	
		if (ch==KEY_BACKSPACE && cursorpos+offset!=0) {
			tmp=malloc(strlen(newtext));
			for (i=0;i<=strlen(newtext);i++)
			{
				if (i<cursorpos+offset) tmp[i]=newtext[i];
				else tmp[i-1]=newtext[i];
			}
			tmp[strlen(newtext)-1]=0;
			free(newtext);
			newtext=malloc(strlen(tmp)+1);
			strncpy(newtext,tmp,strlen(tmp)+1);
			free(tmp);
			ch=KEY_LEFT;			
		}
		if (ch==KEY_DELETE && cursorpos+offset!=strlen(newtext)) {
			tmp=malloc(strlen(newtext));
			for (i=0;i<=strlen(newtext);i++)
			{
				if (i<=cursorpos+offset) tmp[i]=newtext[i];
				else tmp[i-1]=newtext[i];
			}
			tmp[strlen(newtext)-1]=0;
			free(newtext);
			newtext=malloc(strlen(tmp)+1);
			strncpy(newtext,tmp,strlen(tmp)+1);
//			ch=KEY_LEFT;			
		}
		if (printable(ch) && strlen(newtext)<max)
		{	
			tmp=malloc(strlen(newtext)+2);
			for (i=0;i<strlen(newtext);i++)
			{
				if (i<cursorpos+offset) tmp[i]=newtext[i];
				else tmp[i+1]=newtext[i];
			}
			tmp[cursorpos+offset]=ch;
			tmp[strlen(newtext)+1]=0;
			free(newtext);
			newtext=malloc(strlen(tmp)+1);
			strncpy(newtext,tmp,strlen(tmp)+1);			
			free(tmp);	
			ch=KEY_RIGHT;
		}
		if (ch==KEY_LEFT) {
			if (cursorpos) {
				cursorpos--;
			} else {
				if (offset) {
					offset--;
				}
			}
		}
		if (ch==KEY_RIGHT && cursorpos+offset<strlen(newtext)) {
			cursorpos++;
			if (cursorpos > len) {	
				cursorpos=len;
				offset++;
				if (offset > strlen(newtext)-len) {
				  offset=strlen(newtext)-len;
				}
			}
		}
		if (ch!=KEY_ESC && ch!=KEY_RETURN) ch=0;
	}
	if (ch==KEY_ESC)
	{
		free(newtext);
		newtext=malloc(strlen(text)+1);
		strncpy(newtext,text,strlen(text)+1);
	}	
	
	wattrset(parent_window,attrs[COLOR_TEXT]);
	wmove(parent_window,0,0);
	mvwaddch(parent_window,0,0,'=');
#if 0	
	for (i=0;i<max;i++)
	{
		if (i>=0 && i<=len) mvwaddch(parent_window,y,x+i,' ');
	}
	
	wrefresh(parent_window);
	mvwaddch(parent_window,0,0,' ');
	wrefresh(parent_window);
	mvwaddch(parent_window,1,1,'=');
#endif	
	
	for (i=0;i<max;i++) {
		if (i<=len) {
		  mvwaddch(parent_window,y,x+i,textfield[i]);
		}
	}
	wrefresh(parent_window);
	mvwaddch(parent_window,0,0,' ');
	wmove(parent_window,0,0);
	wrefresh(parent_window);
	wmove(parent_window,y,x);
	wmove(parent_window,y,x);
	wmove(parent_window,y,x);
	wmove(parent_window,y,x);
	wmove(parent_window,y,x);
	wmove(parent_window,y,x);
	wmove(parent_window,y,x);
	wmove(parent_window,y,x);
	wmove(parent_window,y,x);
	wmove(parent_window,y,x);
	return newtext;

}
char *input(WINDOW *parent_window,int y,int x, unsigned int len,const char *text, unsigned int max)
{
	return input2(parent_window,y,x,len,text,max,0,0);
}

void new_menu(int updown_nleftright)
{
	MENU_num=0;
	MENU_pos=0;
	MENU_updown_nleftright=updown_nleftright;
}
void menu_item(unsigned int pos,int y,int x,char *text, char hotkey1,char hotkey2,char hotkey3)
{
	if (pos >= MENU_num) MENU_num=pos+1;
	MENU_xpos[pos]=x;
	MENU_ypos[pos]=y;
	free(MENU_text[pos]);
	MENU_text[pos]=malloc(strlen(text)+1);
	strncpy(MENU_text[pos],text,strlen(text)+1);
	MENU_hotkey1[pos]=hotkey1;
	MENU_hotkey2[pos]=hotkey2;
	MENU_hotkey3[pos]=hotkey3;
}
int menu_show(WINDOW *parent_window)
{
	unsigned int i;
	unsigned int j;
	int old_pos=0;	
	int ch=0;
	for (i=0; i < MENU_num; i++) {
		wattrset(parent_window,attrs[COLOR_MENU]);	
		wmove(parent_window,MENU_ypos[i],MENU_xpos[i]);
		for (j=0; j < strlen(MENU_text[i]); j++)
		{
			
			if (j!=0) {
			  if (MENU_text[i][j-1]=='%') {
				wattrset(parent_window,attrs[COLOR_MENU_HOTKEY]); 
			  }
			}
			if (MENU_text[i][j]!='%') {
			  waddch(parent_window,MENU_text[i][j]);
			}
			if (j!=0) {
			  if (MENU_text[i][j-1]=='%') {
				wattrset(parent_window,attrs[COLOR_MENU]); 
			  }
			}
		}		
	}
	while (ch!=KEY_RETURN)
	{
		i=old_pos;
		wattrset(parent_window,attrs[COLOR_MENU]);	
		wmove(parent_window,MENU_ypos[i],MENU_xpos[i]);
		for (j=0;j<strlen(MENU_text[i]);j++)
		{
			
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent_window,attrs[COLOR_MENU_HOTKEY]); 
			if (MENU_text[i][j]!='%') waddch(parent_window,MENU_text[i][j]);
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent_window,attrs[COLOR_MENU]); 
		
				
		}		
		i=MENU_pos;
		wattrset(parent_window,attrs[COLOR_MENU_HI]);	
		wmove(parent_window,MENU_ypos[i],MENU_xpos[i]);
		for (j=0;j<strlen(MENU_text[i]);j++)
		{
			
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent_window,attrs[COLOR_MENU_HOTKEY_HI]); 
			if (MENU_text[i][j]!='%') waddch(parent_window,MENU_text[i][j]);
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent_window,attrs[COLOR_MENU_HI]); 
		}		
		old_pos=MENU_pos;
		wrefresh(parent_window);
		ch=getch2();
		if ((ch==KEY_UP && MENU_updown_nleftright==1) || (ch==KEY_LEFT && MENU_updown_nleftright==0)) {
			if (MENU_pos) {
			  MENU_pos--;
			} else {
			  if (MENU_num) {
				  MENU_pos=MENU_num-1;
			  }
			}
		}
		if ((ch==KEY_DOWN && MENU_updown_nleftright==1) || (ch==KEY_RIGHT && MENU_updown_nleftright==0)) {
			MENU_pos++;
			if (MENU_pos >= MENU_num) {
			  MENU_pos=0;
			}
		}
		for (i=0; i < MENU_num; i++)
		{
			if (ch==MENU_hotkey1[i] || ch==MENU_hotkey2[i] || MENU_hotkey3[i]) {
			  ch=KEY_RETURN;
			  MENU_pos=i;
			}
		
		}
	}	
	i=old_pos;
	wattrset(parent_window,attrs[COLOR_MENU]);	
	wmove(parent_window,MENU_ypos[i],MENU_xpos[i]);
	for (j=0;j<strlen(MENU_text[i]);j++)
	{
		
		if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent_window,attrs[COLOR_MENU_HOTKEY]); 
		if (MENU_text[i][j]!='%') waddch(parent_window,MENU_text[i][j]);
		if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent_window,attrs[COLOR_MENU]); 
	
			
	}		
	i=MENU_pos;
	wattrset(parent_window,attrs[COLOR_MENU_HI]);	
	wmove(parent_window,MENU_ypos[i],MENU_xpos[i]);
	for (j=0;j<strlen(MENU_text[i]);j++)
	{
		
		if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent_window,attrs[COLOR_MENU_HOTKEY_HI]); 
		if (MENU_text[i][j]!='%') waddch(parent_window,MENU_text[i][j]);
		if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent_window,attrs[COLOR_MENU_HI]); 
	}		
	wrefresh(parent_window);
	return MENU_pos;	
}
void draw_frame(WINDOW *parent_window,int y1,int x1,int y2,int x2,char fillcharacter)
{
	int z;
	int i;
	if (x2<x1) { z=x1; x1=x2; x2=z; }
	if (y2<y1) { z=y1; y1=y2; y2=z; }
	wattrset(parent_window,attrs[COLOR_FRAME]);
	mvwaddch(parent_window,y1,x1,ACS_ULCORNER);
	mvwaddch(parent_window,y2,x1,ACS_LLCORNER);
	mvwaddch(parent_window,y1,x2,ACS_URCORNER);
	mvwaddch(parent_window,y2,x2,ACS_LRCORNER);
	wmove(parent_window,y1,x1+1);
	for (z=x1+1;z<x2;z++) waddch(parent_window,ACS_HLINE);	
	wmove(parent_window,y2,x1+1);
	for (z=x1+1;z<x2;z++) waddch(parent_window,ACS_HLINE);	
	for (z=y1+1;z<y2;z++)
	{
		wmove(parent_window,z,x1);
		waddch(parent_window,ACS_VLINE);
		if (fillcharacter!=0) {
			for (i=x1+1;i<x2;i++) waddch(parent_window,fillcharacter);	
		} else {
			wmove(parent_window,z,x1);
		}
		waddch(parent_window,ACS_VLINE);

	
	}	
}
void erase_frame(WINDOW *parent_window,int y1,int x1,int y2,int x2,char fillcharacter)
{
	int z;
	int i;
	if (x2<x1) { z=x1; x1=x2; x2=z; }
	if (y2<y1) { z=y1; y1=y2; y2=z; }
	wattrset(parent_window,attrs[COLOR_FRAME]);
	mvwaddch(parent_window,y1,x1,ACS_ULCORNER);
	mvwaddch(parent_window,y2,x1,ACS_LLCORNER);
	mvwaddch(parent_window,y1,x2,ACS_URCORNER);
	mvwaddch(parent_window,y2,x2,ACS_LRCORNER);
	wmove(parent_window,y1,x1+1);
	for (z=y1;z<=y2;z++)
	{
		wmove(parent_window,z,x1);
		if (fillcharacter!=0) {
			for (i=x1;i<=x2;i++) waddch(parent_window,fillcharacter);	
		} else {
			wmove(parent_window,z,x1);
		}
	}	
}
void headline(WINDOW *parent_window,int y,int x,const char* text)
{
	wattrset(parent_window,attrs[COLOR_HEADLINE]);
	mvwprintw(parent_window,y,x+2,"%s",text);
	wattrset(parent_window,attrs[COLOR_BRACKETS]);
	wmove(parent_window,y,x+1);
	waddch(parent_window,'[');
	wmove(parent_window,y,x+2+strlen(text));
	waddch(parent_window,']');
}
void draw_mainheadline(WINDOW *parent_window,int y,char *text)
{
	int z;
	wmove(parent_window,y,0);
	wattrset(parent_window,attrs[COLOR_FRAME]);
	for (z=0;z<COLS;z++) waddch(parent_window,ACS_HLINE);	
	headline(parent_window,y,COLS-strlen(text)-4,text);	

}

void uimain(void)
{
	(void) signal(SIGINT, finish);
	(void) initscr();
//	setupterm("xterm-color",1,(int *)0);
	keypad (stdscr,TRUE);
	(void) nonl();
	(void) cbreak();
	(void) noecho();
	if (has_colors())
	{
		start_color();	
	}
	init_colors();
//	} else {
//		endwin();
//		fprintf(stderr,"sorry... i need colors... (try using export TERM=xterm-color)\n");	
//		byebye(1);
//	}
	wclear(stdscr);	
	wrefresh(stdscr);
}

void finish(int sig)
{
	endwin();
	byebye(sig);
}
