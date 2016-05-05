////////////////////////////////////////////////////////////////////////////////
// macrodef.h

#ifndef INC_MACRODEF_H
#define INC_MACRODEF_H

////////////////////////////////////////////////////////////////////////////////

//#define DBGFLAG

////////////////////////////////////////////////////////////////////////////////

#ifdef DBGFLAG

#define DBGPREFIX "  ...debug  "
#define DBGPREFIXL2 "  ...debug      "
#define MACRODBGOUTPUT(fmt, s) printf(DBGPREFIX),   printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf(DBGPREFIXL2), printf(fmt, s), printf("\n")

#else //DBGFLAG

#define DBGPREFIX
#define DBGPREFIXL2
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)

#endif //DBGFLAG

////////////////////////////////////////////////////////////////////////////////

#define SUCCESSEXITCODE 0
#define ERREXITCODE 1

////////////////////////////////////////////////////////////////////////////////

#endif //INC_MACRODEF_H

// macrodef.h
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// readbuf.h

#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////

#ifndef INC_READBUF_H
#define INC_READBUF_H

////////////////////////////////////////////////////////////////////////////////

typedef struct readbuf_struct {
    char *mp_buf;
    char *mp_cur;
    size_t mn_size;
} readbuf_t;

////////////////////////////////////////////////////////////////////////////////

int readbuf_init(readbuf_t *);

void readbuf_reset(readbuf_t *);

void readbuf_destroy(readbuf_t *);

int readbuf_putc(readbuf_t *, char);

////////////////////////////////////////////////////////////////////////////////

#endif //INC_READBUF_H

// readbuf.h
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// challenge.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

////////////////////////////////////////////////////////////////////////////////

#ifndef INC_CHALLENGE_H
#define INC_CHALLENGE_H

////////////////////////////////////////////////////////////////////////////////

typedef enum {
    false, true
} bool;

////////////////////////////////////////////////////////////////////////////////

int challenge_main(char *pp_line, char *pp_end);

////////////////////////////////////////////////////////////////////////////////

#endif //INC_CHALLENGE_H

// challenge.h
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// readbuf.c

//#include "macrodef.h"
//#include "readbuf.h"

////////////////////////////////////////////////////////////////////////////////

#define INITSIZE 16
#define EXTENTSIZE 4

////////////////////////////////////////////////////////////////////////////////

int readbuf_init(readbuf_t *pp_readbuf) {
    pp_readbuf->mn_size = INITSIZE;
    pp_readbuf->mp_buf = malloc((size_t) pp_readbuf->mn_size * sizeof(char));
    if (!pp_readbuf->mp_buf) return ERREXITCODE;
    pp_readbuf->mp_cur = pp_readbuf->mp_buf;
    *(pp_readbuf->mp_cur) = '\0';
    return SUCCESSEXITCODE;
}

////////////////////////////////////////////////////////////////////////////////

void readbuf_reset(readbuf_t *pp_readbuf) {
    pp_readbuf->mp_cur = pp_readbuf->mp_buf;
    *(pp_readbuf->mp_cur) = '\0';
}

////////////////////////////////////////////////////////////////////////////////

void readbuf_destroy(readbuf_t *pp_readbuf) {
    free(pp_readbuf->mp_buf);
    pp_readbuf->mp_buf = pp_readbuf->mp_cur = NULL;
    pp_readbuf->mn_size = 0;
}

////////////////////////////////////////////////////////////////////////////////

size_t readbuf_datalen(readbuf_t *pp_readbuf) {
    return pp_readbuf->mp_cur - pp_readbuf->mp_buf;
}

////////////////////////////////////////////////////////////////////////////////

int readbuf_extend(readbuf_t *pp_readbuf) {
    size_t ln_datalen;
    char *lp_newbuf;

    pp_readbuf->mn_size += EXTENTSIZE;
    lp_newbuf = malloc((size_t) pp_readbuf->mn_size * sizeof(char));
    if (!lp_newbuf) return ERREXITCODE;
    strcpy(lp_newbuf, pp_readbuf->mp_buf);
    ln_datalen = readbuf_datalen(pp_readbuf);
    free(pp_readbuf->mp_buf);
    pp_readbuf->mp_buf = lp_newbuf;
    pp_readbuf->mp_cur = pp_readbuf->mp_buf + ln_datalen;

    return SUCCESSEXITCODE;
}

////////////////////////////////////////////////////////////////////////////////

int readbuf_putc(readbuf_t *pp_readbuf, char pn_char) {
    if (readbuf_datalen(pp_readbuf) + 1 == pp_readbuf->mn_size) {
        if (readbuf_extend(pp_readbuf) == ERREXITCODE) return ERREXITCODE;
    }
    *(pp_readbuf->mp_cur++) = pn_char;
    *(pp_readbuf->mp_cur) = '\0';
    return SUCCESSEXITCODE;
}

// readbuf.c
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// challenge.c

//#include "macrodef.h"
//#include "challenge.h"

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
////////////////////////////////////////////////////////////////////////////////
// main.c

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////

//#include "macrodef.h"
//#include "readbuf.h"
//#include "challenge.h"

////////////////////////////////////////////////////////////////////////////////

int main(int pn_argc, const char *pp_argv[]) {

    FILE *lp_file;
    int ln_char;
    readbuf_t lo_readbuf;

    if (readbuf_init(&lo_readbuf) == ERREXITCODE) return ERREXITCODE;
    lp_file = fopen(pp_argv[1], "r");
    if (lp_file == NULL) return ERREXITCODE;

    do {
        ln_char = fgetc(lp_file);
        if (feof(lp_file) || ln_char == '\n') {
            if (challenge_main(lo_readbuf.mp_buf, lo_readbuf.mp_cur) == ERREXITCODE) {
                readbuf_destroy(&lo_readbuf);
                fclose(lp_file);
                return ERREXITCODE;
            }
            readbuf_reset(&lo_readbuf);
        }
        else {
            if (readbuf_putc(&lo_readbuf, (char) ln_char) == ERREXITCODE) {
                readbuf_destroy(&lo_readbuf);
                fclose(lp_file);
                return ERREXITCODE;
            }
        }
        if (feof(lp_file)) break;
    } while (1);

    readbuf_destroy(&lo_readbuf);
    fclose(lp_file);
    return SUCCESSEXITCODE;
} // main()

// main.c
////////////////////////////////////////////////////////////////////////////////
