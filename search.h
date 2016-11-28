#ifndef	SEARCH_H
#define	SEARCH_H
#include "machine_type.h"
#include "buffers.h"

typedef	struct	_tSearch
{
	char	writelogfilename[64];
	char	readlogfilename[64];
	tBool	writesearchlog;
	tBool	readsearchlog;
	tBool	forwardnotbackward;
	char	searchstring[32];
	tInt8	searchlen;
	tInt64	occurancesfound;
	tInt64	lastoccurance;
	tInt64	lastsearchlogpos;
} tSearch;

void	clearsearch(tSearch* search);
tInt8	searchfor(tSearch* search,tBuffer* buf,tUInt64* cursorpos,tBool nextnotprev);

#endif
