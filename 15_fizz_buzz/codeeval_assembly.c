
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

inline int readbuf_init(readbuf_t *);

inline void readbuf_reset(readbuf_t *);

inline void readbuf_destroy(readbuf_t *);

inline size_t readbuf_datalen(readbuf_t *);

inline int readbuf_extend(readbuf_t *);

inline int readbuf_putc(readbuf_t *, char);

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

inline int challenge_main(char *pp_line, char *pp_end);

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

inline int readbuf_init(readbuf_t *pp_readbuf) {
    pp_readbuf->mn_size = INITSIZE;
    pp_readbuf->mp_buf = malloc((size_t) pp_readbuf->mn_size * sizeof(char));
    if (!pp_readbuf->mp_buf) return ERREXITCODE;
    pp_readbuf->mp_cur = pp_readbuf->mp_buf;
    *(pp_readbuf->mp_cur) = '\0';
    return SUCCESSEXITCODE;
}

////////////////////////////////////////////////////////////////////////////////

inline void readbuf_reset(readbuf_t *pp_readbuf) {
    pp_readbuf->mp_cur = pp_readbuf->mp_buf;
}

////////////////////////////////////////////////////////////////////////////////

inline void readbuf_destroy(readbuf_t *pp_readbuf) {
    free(pp_readbuf->mp_buf);
    pp_readbuf->mp_buf = pp_readbuf->mp_cur = NULL;
    pp_readbuf->mn_size = 0;
}

////////////////////////////////////////////////////////////////////////////////

inline size_t readbuf_datalen(readbuf_t *pp_readbuf) {
    return pp_readbuf->mp_cur - pp_readbuf->mp_buf;
}

////////////////////////////////////////////////////////////////////////////////

inline int readbuf_extend(readbuf_t *pp_readbuf) {
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

inline int readbuf_putc(readbuf_t *pp_readbuf, char pn_char) {
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

#define ELEMDELIMCHAR ' '
#define ELEMMAXSIZE 5

////////////////////////////////////////////////////////////////////////////////

inline char *get_element(char *lp_cur, unsigned char *pp_n) {
    char *lp_el;
    int ln_int_buf;
    while ((*lp_cur) && (*lp_cur) == ELEMDELIMCHAR) lp_cur++;
    lp_el = lp_cur;
    while ((*lp_cur) && (*lp_cur) != ELEMDELIMCHAR && (*lp_cur) != '\n' && (*lp_cur) != '\r') lp_cur++;
    if ((*lp_cur) == ELEMDELIMCHAR) *lp_cur = '\0';
    sscanf(lp_el, "%d", &ln_int_buf);
    (*pp_n) = (unsigned char) ln_int_buf;
    return lp_cur + 1;
} // get_element()

////////////////////////////////////////////////////////////////////////////////

inline int challenge_main(char *pp_line, char *pp_end) {
    char *lp_cur;
    //char *ls_output_line;
    char ls_output_line[100 * (ELEMMAXSIZE - 1) + 1];
    char ls_elem[ELEMMAXSIZE];
    unsigned char ln_X, ln_Y, ln_N, i;
    MACRODBGOUTPUT("pp_line = %s", pp_line);


    lp_cur = pp_line;
    lp_cur = get_element(lp_cur, &ln_X);
    lp_cur = get_element(lp_cur, &ln_Y);
    get_element(lp_cur, &ln_N);

    MACRODBGOUTPUT("ln_X = %d", ln_X);
    MACRODBGOUTPUT("ln_Y = %d", ln_Y);
    MACRODBGOUTPUT("ln_N = %d", ln_N);

    //ls_output_line = (char *) malloc((size_t)(ln_N * (ELEMMAXSIZE - 1) + 1));
    if (!ls_output_line) return ERREXITCODE;
    ls_output_line[0] = '\0';

    for (i = 1; i <= ln_N; i++) {
        if (i % ln_X == 0 && i % ln_Y == 0) sprintf(ls_elem, "FB ");
        else if (i % ln_X == 0) sprintf(ls_elem, "F ");
        else if (i % ln_Y == 0) sprintf(ls_elem, "B ");
        else sprintf(ls_elem, "%d ", i);
        strcat(ls_output_line, ls_elem);
    }
    ls_output_line[strlen(ls_output_line) - 1] = '\0';
    // release output:
    printf("%s\n", ls_output_line);
    //free(ls_output_line);
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
