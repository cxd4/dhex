#ifndef UI_H
#define	UI_H
#include "machine_type.h"
#include "buffers.h"
#include "search.h"
tInt8 gotomask(WINDOW* win,tUInt64* cursorpos);
void searchmask(WINDOW* win,tSearch* search,tBuffer* buf,tUInt64* cursorpos);
tInt8 savedialog(WINDOW* win,tBuffer* buf);
#endif
