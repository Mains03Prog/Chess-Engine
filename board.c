#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct bitboards {
    int64_t whitepieces; // all the white pieces on the board
    int64_t blackpieces; // all the black pieces on the board
    int64_t pawns;       // all the pawns on the board
    int64_t rooks;       // all the rooks on the board
    int64_t knights;     // all the knights on the board
    int64_t bishops;     // all the bishops on the board
    int64_t queens;      // all the queens on the board
    int64_t kings;       // all the kings on the board
} bitboards;

typedef struct castlestate {
    bool queenside; // represents whether the colour in question can
                    // castle queen-side
    bool kingside;  // represents whether the colour in question can
                    // castle king-side
} castlestate;

typedef struct boardstate {
    castlestate whitecastlestate;
    castlestate blackcastlestate;
    int64_t enpassanttargetsquare;
    int halfmoveclock;
    int fullmovecounter;
} boardstate;

typedef struct board {
    bitboards bitboards;   // board representation
    boardstate boardstate; // state of the board - castling, en passant,
                           // etc
} board;

// returns whether the character on a rank on the piece placement
// part of the FEN is a valid character
bool isfenpieceplacementcharactervalid(char c) {
    if (c == '0' || c == '9')
        return false;
    if (isdigit(c))
        return true;
    char *validpieces = "PNBRQKpnbrqk";
    for (int i=0; validpieces[i] != '\0'; ++i) {
        if (c == validpieces[i])
            return true;
    }
    return false;
}

bool isfenpieceplacementrankvalid(char *rank) {
    int filecount = 0; // number of files in the rank
    bool expectpiece = false;
    for (int i=0; rank[i] != '\0'; ++i) {
        if (!isfenpieceplacementcharactervalid(rank[i]))
            return false;
        if (expectpiece && isdigit(rank[i]))
            return false;
        // increase filecount appropriately
        // and update expectpiece
        if (isdigit(rank[i])) {
            filecount += rank[i] - '0';
            expectpiece = true;
        } else {
            ++filecount;
            expectpiece = false;
        }
    }
    return (filecount == 8); // should be exactly 8 files
}

// returns whether the piece placement ranks are valid
// also stores the index of the end of the piece placement
// section for efficiency purposes
bool arefenpieceplacementranksvalid(char *fen, int *pieceplacementend) {
    int rankcount = 0; // number of ranks in the FEN
    char rank[9]; // used to store each rank in turn for checks
                  // extra space for null-terminator
    int i=0;
    for (; rankcount < 8; ++i) {
        // there should be a character and no more than 8 ranks
        if (fen[i] == '\0')
            return false;
        int rankindex = 0; // index of rank string
        char endcharacter = '/';
        if (rankcount == 7)
            endcharacter = ' ';
        for (; fen[i] != endcharacter; ++i) {
            // there should be a character and
            // no more than 7 characters in the rank
            if (fen[i] == '\0' || rankindex > 7)
                return false;
            rank[rankindex++] = fen[i];
        }
        rank[rankindex] = '\0';
        if (!isfenpieceplacementrankvalid(rank))
            return false;
        ++rankcount;
    }
    if (rankcount != 8) // should be exactly 8 ranks
        return false;
    *pieceplacementend = i-1; // store index of the end of the piece
                              // placement section
    return true;
}

// used to check there is exactly one king of each colour on the board
// pieceplacementend stores the index of the end of the piece placement
// section
bool isfenpieceplacementnumkingsvalid(char *fen, int pieceplacementend) {
    int numwhitekings = 0;
    int numblackkings = 0;
    // count the number of kings of each colour
    for (int i=0; i<pieceplacementend; ++i) {
        if (fen[i] == 'K')
            ++numwhitekings;
        else if (fen[i] == 'k')
            ++numblackkings;
    }
    // return whether the number of kings of each colour is correct
    return (numwhitekings == 1 && numblackkings == 1);
}

// returns whether the piece placement section is valid
// pieceplacementend is set to the end index of the piece
// placement section once the function has finished
// TODO: account for both kings being in check
bool isfenpieceplacementvalid(char *fen, int *pieceplacementend) {
    // check the ranks are valid
    if (!arefenpieceplacementranksvalid(fen, pieceplacementend))
        return false;
    // check the number of kings is valid
    if (!isfenpieceplacementnumkingsvalid(fen, *pieceplacementend))
        return false;
    return true;
}

bool isfensidetomovecharactervalid(char c) {
    return (c == 'w' || c == 'b');
}

// returns whether the side to move section is valid
// i stores the start index of the side to move section
// and stores the end index of the side to move section
// once the function has finished
bool isfensidetomovevalid(char *fen, int *i) {
    if (fen[*i] == '\0')
        return false;
    if (!isfensidetomovecharactervalid(fen[*i]))
        return false;
    ++(*i);
    return true;
}

bool checkfencastlingabilitycharacters(char *fen, int *i,
        bool *whitecancastlekingside, bool *whitecancastlequeenside,
        bool *blackcancastlekingside, bool *blackcancastlequeenside) {
    // initialise the pointers
    *whitecancastlekingside = *whitecancastlequeenside = false;
    *blackcancastlekingside = *blackcancastlequeenside = false;
    
}

bool isfencastlingabilitywhitekinge1(char *fen, int pieceplacementend) {

}

bool isfencastlingabilityblackkinge8(char *fen, int pieceplacement) {

}

bool checkfencastlingabilitypieceplacement(char *fen, int pieceplacementend,
        bool *whitecancastlekingside, bool *whitecancastlequeenside,
        bool *blackcancastlekingside, bool *blackcancastlequeenside) {
    if (*whitecancastlekingside || *whitecancastlequeenside) {
        if (!isfencastlingabilitywhitekinge1(fen, pieceplacementend))
            return false;
    }
    if (*blackcancastlekingside || *blackcancastlequeenside) {
        if (!isfencastlingabilityblackkinge8(fen, pieceplacementend))
            return false;
    }
    
}

// returns whether the castling ability section is valid
// i stores the start index of the castling ability section
// and stores the end index of the castling ability section
// once the function has finished
bool isfencastlingabilityvalid(char *fen, int *i, int pieceplacementend) {
    if (fen[*i] == '\0')
        return false;
    if (fen[*i] == '-') {
        ++(*i);
        return true;
    }
    bool whitecancastlekingside, whitecancastlequeenside;
    bool blackcancastlekingside, blackcancastlequeenside;
    if (!checkfencastlingabilitycharacters(fen, i,
            &whitecancastlekingside, &whitecancastlequeenside,
            &blackcancastlekingside, &blackcancastlequeenside))
        return false;
    
}

bool isfenenpassanttargetsquarevalid(char *fen, int *i) {

}

bool isfenhalfmoveclockvalid(char *fen, int *i) {

}

bool isfenfullmovecountervalid(char *fen, int *i) {

}

// macro to check if there is a space at index i in the specified FEN
// returns false from the function if there isn't a space
#define FEN_CHECK_SPACE(fen,i) if (fen[i] != ' ') return false

// returns whether the specified FEN is valid
bool isfenvalid(char *fen) {
    int i=0;
    if (!isfenpieceplacementvalid(fen, &i))
        return false;
    int pieceplacementend = i;
    FEN_CHECK_SPACE(fen, i++);
    if (!isfensidetomovevalid(fen, &i))
        return false;
    FEN_CHECK_SPACE(fen, i++);
    if (!isfencastlingabilityvalid(fen, &i, pieceplacementend))
        return false;
    FEN_CHECK_SPACE(fen, i++);
    if (!isfenenpassanttargetsquarevalid(fen, &i))
        return false;
    FEN_CHECK_SPACE(fen, i++);
    if (!isfenhalfmoveclockvalid(fen, &i))
        return false;
    FEN_CHECK_SPACE(fen, i++);
    if (!isfenfullmovecountervalid(fen, &i))
        return false;
    if (fen[i] != '\0')
        return false;
    return true;
}

typedef struct fenpieceplacement {
    char *pieceplacementrow[8]; // rows of the board
} fenpieceplacement;

typedef struct fencomponents {
    fenpieceplacement pieceplacement; // where the pieces should go
    char sidetomove;                  // colour to move
    char *castlingability;            // info about castling state
    char *enpassanttargetsquare;      // en passant target square
    char *halfmoveclock;
    char *fullmovecounter;
} fencomponents;

// creates a fencomponents instance from the specified FEN
// assumes the FEN is valid
fencomponents createfencomponents(char *fen) {

}

board *createboardfromfencomponents(fencomponents *components) {

}

void freefenpieceplacement(fenpieceplacement *pieceplacement) {
    for (int i=0; i<8; ++i)
        free(pieceplacement->pieceplacementrow[i]);
}

void freefencomponents(fencomponents *components) {
    freefenpieceplacement(&(components->pieceplacement));
    free(components->castlingability);
    free(components->enpassanttargetsquare);
    free(components->halfmoveclock);
    free(components->fullmovecounter);
}

board *createboardfromfen(char *fen) {
    if (!isfenvalid(fen))
        return NULL;
    fencomponents fencomponents = createfencomponents(fen);
    board *board = createboardfromfencomponents(&fencomponents);
    freefencomponents(&fencomponents);
    return board;
}

#ifdef TEST
void testisfenpieceplacementcharactervalid();
void testisfenpieceplacementrankvalid();
void testarefenpieceplacementranksvalid();
void testisfenpieceplacementnumkingsvalid();
void testisfenpieceplacementvalid();
void testisfensidetomovecharactervalid();
void testisfensidetomovevalid();
void testcheckfencastlingabilitycharacters();
void testisfencastlingabilitywhitekinge1();
void testisfencastlingabilityblackkinge8();
void testcheckfencastlingabilitypieceplacement();
void testisfencastlingabilityvalid();
void testisfenenpassanttargetsquarevalid();
void testisfenhalfmoveclockvalid();
void testisfenfullmovecountervalid();
void testisfenvalid();
void testcreatefencomponents();
void testcreateboardfromfencomponents();
void testcreateboardfromfen();

void testboard() {
    testisfenpieceplacementcharactervalid();
    testisfenpieceplacementrankvalid();
    testarefenpieceplacementranksvalid();
    testisfenpieceplacementnumkingsvalid();
    testisfenpieceplacementvalid();
    testisfensidetomovecharactervalid();
    testisfensidetomovevalid();
    testcheckfencastlingabilitycharacters();
    testisfencastlingabilitywhitekinge1();
    testisfencastlingabilityblackkinge8();
    testcheckfencastlingabilitypieceplacement();
    testisfencastlingabilityvalid();
    testisfenenpassanttargetsquarevalid();
    testisfenhalfmoveclockvalid();
    testisfenfullmovecountervalid();
    testisfenvalid();
    testcreatefencomponents();
    testcreateboardfromfencomponents();
    testcreateboardfromfen();
    printf("All tests pass.\n");
}

void testfailed(char *testname) {
    fprintf(stderr, "Test '%s' failed.\n", testname);
    exit(EXIT_FAILURE);
}

void testpassed(char *testname) {
    printf("Test '%s' passed.\n", testname);
}

void outputtestresult(bool pass, char *testname) {
    if (!pass)
        testfailed(testname);
    else
        testpassed(testname);
}

void testisfenpieceplacementcharactervalid() {
    bool pass = true;
    // should fail
    if (isfenpieceplacementcharactervalid('0'))
        pass = false;
    // should fail
    if (isfenpieceplacementcharactervalid('9'))
        pass = false;
    // should pass
    char *digits = "12345678";
    for (int i=0; pass && digits[i] != '\0'; ++i) {
        if (!isfenpieceplacementcharactervalid(digits[i]))
            pass = false;
    }
    // should pass
    char *pieces = "PNBRQKpnbrqk";
    for (int i=0; pass && pieces[i] != '\0'; ++i) {
        if (!isfenpieceplacementcharactervalid(pieces[i]))
            pass = false;
    }
    // should fail
    if (isfenpieceplacementcharactervalid('x'))
        pass = false;
    // output result
    char *testname = "isfenpieceplacementcharactervalid";
    outputtestresult(pass, testname);
}

void testisfenpieceplacementrankvalid() {
    bool pass = true;
    // following tests should pass
    if (!isfenpieceplacementrankvalid("8"))
        pass = false;
    if (!isfenpieceplacementrankvalid("pppppppp"))
        pass = false;
    if (!isfenpieceplacementrankvalid("PPPPPPPP"))
        pass = false;
    if (!isfenpieceplacementrankvalid("rnbqkbnr"))
        pass = false;
    if (!isfenpieceplacementrankvalid("RNBQKBNR"))
        pass = false;
    if (!isfenpieceplacementrankvalid("pRpq2P1"))
        pass = false;
    // following tests should fail
    if (isfenpieceplacementrankvalid("")) // no rank given
        pass = false;
    if (isfenpieceplacementrankvalid("44")) // expect piece after digit
        pass = false;
    if (isfenpieceplacementrankvalid("ppphpppp")) // invalid character
        pass = false;
    if (isfenpieceplacementrankvalid("9")) // too many files
        pass = false;
    if (isfenpieceplacementrankvalid("8p")) // too many files
        pass = false;
    if (isfenpieceplacementrankvalid("6")) // too few files
        pass = false;
    if (isfenpieceplacementrankvalid("PPPPbnPPP")) // too many files
        pass = false;
    if (isfenpieceplacementrankvalid("Kq")) // too few files
        pass = false;
    // output result
    char *testname = "isfenpieceplacementrankvalid";
    outputtestresult(pass, testname);
}

void testarefenpieceplacementranksvalid() {
    bool pass = true;
    int pieceplacementend;
    if (!arefenpieceplacementranksvalid("8/8/8/8/8/8/8/8 ", &pieceplacementend))
        pass = false;
    if (!arefenpieceplacementranksvalid("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR ", &pieceplacementend))
        pass = false;
    if (pieceplacementend != 43)
        pass = false;
    if (arefenpieceplacementranksvalid("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", &pieceplacementend)) // missing space at end
        pass = false;
    if (arefenpieceplacementranksvalid("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/ ", &pieceplacementend)) // extra /
        pass = false;
    if (arefenpieceplacementranksvalid("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQKBNR ", &pieceplacementend)) // missing rank
        pass = false;
    if (arefenpieceplacementranksvalid("rnbqkbnr/ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR ", &pieceplacementend)) // too few pieces on one rank
        pass = false;
    if (arefenpieceplacementranksvalid("rnbqkbnr/pppppppp/8/8/8/8/PPxPPPPP/RNBQKBNR ", &pieceplacementend)) // invalid rank character
        pass = false;
    if (arefenpieceplacementranksvalid("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPPP/RNBQKBNR ", &pieceplacementend)) // too many pieces on one rank
        pass = false;
    if (arefenpieceplacementranksvalid("rnbqkbnr/ppp3pppp/8/8/8/8/PPPPPPPP/RNBQKBNR ", &pieceplacementend)) // too many files on a rank
        pass = false;
    // output result
    char *testname = "arefenpieceplacementranksvalid";
    outputtestresult(pass, testname);
}

void testisfenpieceplacementnumkingsvalid() {
    bool pass = true;
    // following tests should pass
    if (!isfenpieceplacementnumkingsvalid("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR ", 43)) // two kings
        pass = false;
    if (!isfenpieceplacementnumkingsvalid("k7/8/8/8/8/8/8/K7 ", 17)) // two kings
        pass = false;
    // following tests should fail
    if (isfenpieceplacementnumkingsvalid("8/8/8/8/8/8/8/8 ", 15)) // no kings
        pass = false;
    if (isfenpieceplacementnumkingsvalid("rnbqpbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQPBNR ", 43)) // no kings
        pass = false;
    if (isfenpieceplacementnumkingsvalid("k7/8/8/8/8/8/8/8 ", 16)) // one king
        pass = false;
    if (isfenpieceplacementnumkingsvalid("8/8/8/8/8/8/2K5/8 ", 16)) // one king
        pass = false;
    if (isfenpieceplacementnumkingsvalid("8/8/3k4/8/8/8/8/k7 ", 18)) // two kings of same colour
        pass = false;
    if (isfenpieceplacementnumkingsvalid("K7/8/8/8/1K6/8/8/8 ", 18)) // two kings of same colour
        pass = false;
    // output result
    char *testname = "isfenpieceplacementnumkingsvalid";
    outputtestresult(pass, testname);
}

void testisfenpieceplacementvalid() {

}

void testisfensidetomovecharactervalid() {

}

void testisfensidetomovevalid() {

}

void testcheckfencastlingabilitycharacters() {

}

void testisfencastlingabilitywhitekinge1() {

}

void testisfencastlingabilityblackkinge8() {

}

void testcheckfencastlingabilitypieceplacement() {

}

void testisfencastlingabilityvalid() {

}

void testisfenenpassanttargetsquarevalid() {

}

void testisfenhalfmoveclockvalid() {

}

void testisfenfullmovecountervalid() {

}

void testisfenvalid() {

}

void testcreatefencomponents() {

}

void testcreateboardfromfencomponents() {

}

void testcreateboardfromfen() {

}

#ifdef TEST_BOARD
int main() {
    testboard();
    return 0;
}
#endif
#endif