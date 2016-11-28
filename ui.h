#include <ncurses.h>
#include <signal.h>
#include <string.h>


#define KEY_ESC 27
#define KEY_RETURN 13
#define KEY_DELETE 127
#define COLOR_BRACKETS 1
#define COLOR_HEXFIELD 2
#define COLOR_INPUT 3
#define COLOR_CURSOR 4
#define COLOR_TEXT 5
#define COLOR_MENU 6
#define COLOR_MENU_HI 7
#define COLOR_MENU_HOTKEY 8
#define COLOR_MENU_HOTKEY_HI 9
#define COLOR_FRAME 10
#define COLOR_DIFF 11
#define COLOR_DIFF_CURSOR 12
#define COLOR_HEADLINE 13

int lastkey;
int attrs[255];

void finish(int sig);
#define MENU_MAX 64
int MENU_xpos[MENU_MAX];
int MENU_ypos[MENU_MAX];
char *MENU_text[MENU_MAX];
char MENU_hotkey1[MENU_MAX];
char MENU_hotkey2[MENU_MAX];
char MENU_hotkey3[MENU_MAX];
unsigned int MENU_num;
unsigned int MENU_pos;
unsigned int MENU_updown_nleftright;

int printable(int ch);
int getch2(void);
void init_colors(void);
char *input2(WINDOW *parent,int y,int x, unsigned int len,const char *text, unsigned int max,int special,int lastkey);
char *input(WINDOW *parent,int y,int x, unsigned int len,const char *text, unsigned int max);

void new_menu(int updown_nleftright);
void menu_item(unsigned int pos,int y,int x,char *text, char hotkey1,char hotkey2,char hotkey3);
int menu_show(WINDOW *parent);
void draw_frame(WINDOW *parent,int y1,int x1,int y2,int x2,char fillcharacter);
void erase_frame(WINDOW *parent,int y1,int x1,int y2,int x2,char fillcharacter);
void headline(WINDOW *parent,int y,int x,const char* text);
void main_headline(WINDOW *parent,char* text);
void draw_mainheadline(WINDOW *parent_window,int y,char *text) ;
void uimain(void);




