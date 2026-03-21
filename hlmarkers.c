#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

#include "datatypes.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

const char* COLORS_STR[] = {
    "black", "red", "green", "yellow", "blue","magenta", "cyan", "white"
};

const uicolors COLORS_UI[] = {
    COLOR_HL_BLACK,
    COLOR_HL_RED,
    COLOR_HL_GREEN,
    COLOR_HL_YELLOW,
    COLOR_HL_BLUE,
    COLOR_HL_MAGENTA,
    COLOR_HL_CYAN,
    COLOR_HL_WHITE};

const uicolors COLORS_UIDIFF[] = {
    COLOR_HL_BLACKD,
    COLOR_HL_REDD,
    COLOR_HL_GREEND,
    COLOR_HL_YELLOWD,
    COLOR_HL_BLUED,
    COLOR_HL_MAGENTAD,
    COLOR_HL_CYAND,
    COLOR_HL_WHITED};


static int parseLine(tHighlight* marker, const char* line)
{
    for (int cIdx = 0; cIdx < ARRAY_SIZE(COLORS_STR); ++cIdx)
    {
        if (strncasecmp(line, COLORS_STR[cIdx], strlen(COLORS_STR[cIdx])) != 0)
        {
            continue;
        }
        const char* typeStr = line + strlen(COLORS_STR[cIdx]);
        typeStr += strspn(typeStr, "\t "); ;
        if (*typeStr == '@')
        {
            marker->type = HLMARKER_POS;
        }
        else if (*typeStr == ':')
        {
            marker->type = HLMARKER_VALUE;
        }
        else
        {
            return RETNOK;
        }
        const char* numAstr = typeStr + 1;
        char* numBstr = NULL;
        marker->numA = -1;
        marker->numB = -1;
        errno = 0;
        marker->numA = strtoull(numAstr, &numBstr, 0);
        marker->numB = marker->numA;
        if (errno != 0 || numBstr == numAstr)
        {
            return RETNOK;
        }
        if (*typeStr == '@' && *numBstr == '-')
        {
            numBstr += 1;
            char* endPtr = NULL;
            marker->numB = strtoull(numBstr, &endPtr, 0);
            if (errno != 0 || endPtr == numBstr)
            {
                return RETNOK;
            }
        }
        // printf("hlmarkers: color=%-10s  type=%c numA=%-8lld numB=%-8lld \n",
        //        COLORS_STR[cIdx], *typeStr, marker->numA, marker->numB);
        marker->color = COLORS_UI[cIdx];
        marker->colord = COLORS_UIDIFF[cIdx];
        return RETOK;
    }
    return RETNOK;
}

int parsehlmarkerfile(tHighlight* marker, uint32_t* num, const char* filename)
{
    FILE* f = fopen(filename, "r");
    char line[0xff];
    *num = 0;
    while (!feof(f) &&
           fgets(line, sizeof(line), f) > 0 &&
           *num < NUMHLMARKERS)
    {
        const char* cfgLine = line + strspn(line, "\f\n\r\t\v ");
        if (cfgLine[0] == '#' || !isalpha(cfgLine[0]))
        {
            continue;
        }
        if (parseLine(&marker[*num], cfgLine) != RETOK)
        {
            fprintf(stderr, "error reading hl marker file '%s'\n"
                    "invalid entry: %s", filename, line);
            return RETNOK;
        }
        *num += 1;
    }
    fclose(f);
    // getchar();
    return RETOK;
}
