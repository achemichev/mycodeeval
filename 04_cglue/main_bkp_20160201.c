#include <stdio.h>
#include <string.h>

#define DBG

#ifdef DBG
#define MACRODBGOUTPUT(fmt, s) printf("  ...debug  "),     printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf("  ...debug      "), printf(fmt, s), printf("\n")
#else
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)
#endif

#define MAXLINELENGTH 32768
#define MAXNPIECES 1024
#define DELIMITERSTR "|"

////////////////////////////////////////////////////////////////

void glue(int npieces, char **pieces/*[MAXNPIECES]*/) {

    MACRODBGOUTPUT("%s", "Glue func call:");
    MACRODBGOUTPL2("Pieces pointer = 0x%x", pieces);
    MACRODBGOUTPL2("NPieces = %d", npieces);

    int plen = strlen(pieces[0]), textlen = npieces + plen - 1, i, hash;
    char *c;

    MACRODBGOUTPL2("Piece length = %d", plen);
    MACRODBGOUTPL2("Text length = %d", textlen);

    MACRODBGOUTPUT("%s", "Piece hashes:");
    for (i = 0; i < npieces; i++) {
        hash = 0;
        for (c = pieces[i]; *c; c++)
            hash |= (1 << ((*c - 0x20) % 0x20));
        MACRODBGOUTPL2("h = 0x%08x", hash);
    }

    //printf("Glue Release\n");

} // glue

////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[]) {

    FILE *file = fopen(argv[1], "r");
    char line[MAXLINELENGTH];
    int len, npieces;
    char *p;
    char *pieces[MAXNPIECES];

    while (fgets(line, MAXLINELENGTH, file)) {

        if (line[0] == '\n')continue;
        len = strlen(line);
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';
        MACRODBGOUTPUT("Iput string = \"%s\"", line);
        MACRODBGOUTPUT("%s", "Pieces:");

        for (npieces = 0, p = strtok(line, DELIMITERSTR); p != NULL; p = strtok(NULL, DELIMITERSTR)) {
            pieces[npieces] = p;
            npieces++;
            MACRODBGOUTPL2("|%s|", p);
        } // for()
        MACRODBGOUTPUT("Number of pieces = \"%d\"", npieces);

        glue(npieces, pieces);
        MACRODBGOUTPUT("%s", "Input line iteration finished...\n");
    } // while

    return 0;
}