#ifndef BUFFERS_H
#define	BUFFERS_H

#include <stdio.h>
#include <stdlib.h>
#include "machine_type.h"
#include "config.h"
typedef	struct	_tChange
{
	tUInt64	pos;
	tUInt8	before;
	tUInt8	after;
} tChange;

typedef struct _tBuffer
{
	tBool	valid;		
	tUInt8* dataptr;	// the pointer to where the buffer data is stored
	tUInt64 filesize;	// the size of the actual file
	tUInt64 bufsize;	// the size of the buffer, in case some bytes are added at the end
	tUInt64 bufferpos;	// the offset from where this chunk has been read
	tBool	fresh;		// =1 if there is no data in this buffer
	tFptr	file;		// the file pointer
	char	filename[512];	// the file name
	tChange* changes;	
	tUInt32	changesnum;	// number of changes
	tBool	nibble;
	tUInt8	nexthex;
	tUInt64	changepos;
} tBuffer;


// opens a buffer, bufnum has to be either 1 or 2
tUInt32 openbuf(tBuffer* hBuf,tUInt8 bufnum,char* filename);

// reads into the buffer, if the need arises
tUInt32 readbuf(tBuffer* hBuf,tInt64 pos);

// get the index of the file position within the buffer
tInt32	getbufferidx(tBuffer* hBuf,tInt64 pos);


tInt8   savechanges(tBuffer* hBuf);

#endif
