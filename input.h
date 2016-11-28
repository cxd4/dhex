#ifndef INPUT_H
#define	INPUT_H
#include <stdio.h>
#include <stdlib.h>
#include "machine_type.h"

#define KEYF1           0x0100
#define KEYF2           0x0101
#define KEYF3           0x0102
#define KEYF4           0x0103
#define KEYF5           0x0104
#define KEYF6           0x0105
#define KEYF7           0x0106
#define KEYF8           0x0107
#define KEYF9           0x0108
#define KEYF10          0x0109
#define KEYESC          0x010a
#define KEYBACKSPACE    0x010b
#define KEYDEL          0x010c
#define KEYENTER        0x010d
#define KEYTAB          0x010e
#define KEYUP           0x010f
#define KEYDOWN         0x0110
#define KEYLEFT         0x0111
#define KEYRIGHT        0x0112
#define KEYPGUP         0x0113
#define KEYPGDOWN       0x0114
#define KEYHOME         0x0115
#define KEYEND          0x0116

#define NUM_SPECIALKEYS	0x17

// this is for the special keys
// i was not satisfied with how ncurses handled the function keys
typedef struct  _tKeyTab
{
        tInt16	retval;
        tBool	allowed_in_inputfield;
        tUInt8	seqlen;
        unsigned char seq[8];		// which sequence will be returned when this key is pressed
        unsigned char config[16];	// what string is written in the config-file
} tKeyTab;

tInt16 getkey(tBool inputfield);
tInt16 hexinput(WINDOW* win,tInt16 y,tInt16 x,tUInt64* val,tInt16 len);
tInt16 stringinput(WINDOW* win,tInt16 y,tInt16 x,char* s,tInt16 len);
int configkeytab(char* line);
void keyboardsetup(WINDOW* win,char* configfilename);

#endif
