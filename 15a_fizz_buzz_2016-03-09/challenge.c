////////////////////////////////////////////////////////////////////////////////
// challenge.c

#include "macrodef.h"
#include "challenge.h"

////////////////////////////////////////////////////////////////////////////////

#define ELEMDELIMSTR  " "

////////////////////////////////////////////////////////////////////////////////

int challenge_main(char *pp_line, char *pp_end) {
    int ln_X, ln_Y, ln_N, i;

    MACRODBGOUTPUT("pp_line = %s", pp_line);

    if(pp_line == NULL || pp_line[0] == '\0') return SUCCESSEXITCODE;

    sscanf(strtok(pp_line, ELEMDELIMSTR), "%d", &ln_X);
    sscanf(strtok(NULL, ELEMDELIMSTR), "%d", &ln_Y);
    sscanf(strtok(NULL, ELEMDELIMSTR), "%d", &ln_N);

    MACRODBGOUTPUT("ln_X = %d", ln_X);
    MACRODBGOUTPUT("ln_Y = %d", ln_Y);
    MACRODBGOUTPUT("ln_N = %d", ln_N);

    for (i = 1; i <= ln_N; i++) {
        if (i % ln_X == 0 && i % ln_Y == 0) printf("FB");
        else if (i % ln_X == 0) printf("F");
        else if (i % ln_Y == 0) printf("B");
        else printf("%d", i);
        if(i < ln_N) printf("%c", ' ');
    }
    printf("\n");
    return SUCCESSEXITCODE;
} // challenge_main()

// challenge.c
////////////////////////////////////////////////////////////////////////////////
