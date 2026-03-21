#ifndef HLMARKERS_H
#define HLMARKERS_H

/**
 * Use -l, -L [highlightfile] on the command line to specify a highlight marker
 * file. The syntax for highlight markers is:
 *
 * COLOR:VALUE
 * COLOR@OFFSET_START[-OFFSET_END]
 *
 * COLOR
 *   One of the colors black, red, green, yellow, blue, magenta, cyan, white.
 * :VALUE
 *   The byte value to highlight.
 * @OFFSET_START[-OFFSET_END]
 *   The byte offset or offset range to highlight.
 *
 * Comments start with '#'. Whitespace are ignored. All strings are
 * case-insensitive. Numerical values are parsed with strtoull(3) and can be
 * decimal (no prefix), hexadecimal (prefix 0x) or octal (prefix 0). If multiple
 * entries apply, only the last entry will be visible.
 *
 * Examples:
 *
 * magenta : 0x70      # byte value hexadecimal
 * white   : 32        # byte value decimal
 * red:010             # byte value octal
 * red @ 2             # offset decimal
 * GREEN	@	 8-11  # offset range decimal
 * blue@0x120-0x12f    # offset range hexadecimal
 * red@010             # offset octal
 *
 */

#include "machine_type.h"
#include "output.h"
#include "datatypes.h"

int parsehlmarkerfile(tHighlight* hlmarkers,uint32_t* num,char* filename);

#endif
