#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
//#include "macrodef.h"
//#include "readbuf.h"

#ifndef INC_MACRODEF_H
#define INC_MACRODEF_H

//#define DBG

#ifdef DBG
#define DBGPREFIX "  ...debug  "
#define DBGPREFIXL2 "  ...debug      "
#define MACRODBGOUTPUT(fmt, s) printf(DBGPREFIX),   printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf(DBGPREFIXL2), printf(fmt, s), printf("\n")
#else
#define DBGPREFIX
#define DBGPREFIXL2
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)
#endif

#define SUCCESSEXITCODE 0
#define ERREXITCODE 1

#endif //INC_MACRODEF_H

////////////////////////////////////////////////////////////////////////////////

#ifndef INC_READBUF_H
#define INC_READBUF_H

typedef struct readbuf_struct {
    char *mp_buf;
    char *mp_cur;
    size_t mn_size;
} readbuf_t;

inline int readbuf_init(readbuf_t *);

inline void readbuf_reset(readbuf_t *);

inline void readbuf_destroy(readbuf_t *);

inline size_t readbuf_datalen(readbuf_t *);

inline int readbuf_extend(readbuf_t *);

inline int readbuf_putc(readbuf_t *, char);

#endif //INC_READBUF_H

////////////////////////////////////////////////////////////////////////////////

#ifndef INC_THISPROJECT_H
#define INC_THISPROJECT_H

typedef enum {
    false, true
} bool;

#endif //INC_THISPROJECT_H

////////////////////////////////////////////////////////////////////////////////
// readbuf.c

//#include <stdlib.h>
//#include <string.h>
//#include "macrodef.h"
//#include "readbuf.h"

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
// thisproject.c

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <limits.h>
//#include "macrodef.h"
//#include "thisproject.h"

////////////////////////////////////////////////////////////////////////////////

#define ELEMDELIMCHAR ' '

////////////////////////////////////////////////////////////////////////////////

inline void thisproject_entry(char *pp_line, char *pp_end) {
    unsigned char ln_code;
    char *lp_cur;
    int ln;

    if (!pp_line || !pp_line[0]) return;

    MACRODBGOUTPUT("pp_line = %s", pp_line);

    lp_cur = pp_end;
    while ((*lp_cur) != ELEMDELIMCHAR)
        lp_cur--;
    sscanf(lp_cur + 1, "%d", &ln);
    (*lp_cur) = '\0';
    lp_cur--;

    while (lp_cur >= pp_line) {
        if ((*lp_cur) == ELEMDELIMCHAR || lp_cur == pp_line) {
            ln--;
            if (ln == 0) {
                if (lp_cur == pp_line) printf("%s\n", lp_cur);
                else printf("%s\n", lp_cur + 1);
                break;
            }
            (*lp_cur) = '\0';
        }
        lp_cur--;
    } // while

} // thisproject_entry()

// thisproject.c
////////////////////////////////////////////////////////////////////////////////
// main.c

//#include <stdio.h>
//#include <stdlib.h>
//#include "macrodef.h"
//#include "readbuf.h"
//#include "thisproject.h"

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
            thisproject_entry(lo_readbuf.mp_buf, lo_readbuf.mp_cur);
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