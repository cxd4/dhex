#ifndef MARKERS_H
#define	MARKERS_H
#include "machine_type.h"
#include "output.h"

#define	NUMMARKERS	10

typedef struct	_tMarkers
{
	char	relative[NUMMARKERS];
	tUInt64	cursorpos[NUMMARKERS];	
} tMarkers;
void* initmarkers();
int	parsemarkerfile(tMarkers* markers,char* filename);
tInt8 gotomask(tOutput* output,tMarkers* markers,tUInt64* cursorpos);


#endif

