#ifndef _BOARD_H
#define _BOARD_H

#include <inttypes.h>

typedef struct board board;

// creates a board instance from the specified FEN
// returns NULL if the FEN is invalid
board *createboardfromfen(char *fen);

#ifdef TEST
void testboard();
#endif

#endif