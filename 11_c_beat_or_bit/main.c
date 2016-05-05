#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
//#include "macrodef.h"
//#include "readbuf.h"
//#include "beatorbit.h"

#ifndef INC_09_C_CODE_LIKE_HUFFMAN_MACRODEF_H
#define INC_09_C_CODE_LIKE_HUFFMAN_MACRODEF_H

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

#endif //INC_09_C_CODE_LIKE_HUFFMAN_MACRODEF_H

////////////////////////////////////////////////////////////////////////////////

#ifndef INC_09_C_CODE_LIKE_HUFFMAN_READBUF_H
#define INC_09_C_CODE_LIKE_HUFFMAN_READBUF_H

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

#endif //INC_09_C_CODE_LIKE_HUFFMAN_READBUF_H

////////////////////////////////////////////////////////////////////////////////

#ifndef INC_11_C_BEATORBIT_H
#define INC_11_C_BEATORBIT_H

////////////////////////////////////////////////////////////////////////////////

typedef enum {
    false, true
} bool;

////////////////////////////////////////////////////////////////////////////////

typedef struct node_struct node_t;

////////////////////////////////////////////////////////////////////////////////

struct node_struct {
    char *mp_chars;
    unsigned char mn_prio;
    bool mb_has_parent;
    unsigned char mn_code;
    node_t *mp_left_child;
    node_t *mp_right_child;
};

////////////////////////////////////////////////////////////////////////////////

inline void beatorbit_entry(char *pp_line);

////////////////////////////////////////////////////////////////////////////////

#endif //INC_11_C_BEATORBIT_H

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <limits.h>
//#include "macrodef.h"
//#include "beatorbit.h"

////////////////////////////////////////////////////////////////////////////////

#define ELEMDELIMSTR "|"

////////////////////////////////////////////////////////////////////////////////

inline unsigned char todec(unsigned char pn_code) {
    pn_code = pn_code ^ (pn_code >> 16);
    pn_code = pn_code ^ (pn_code >> 8);
    pn_code = pn_code ^ (pn_code >> 4);
    pn_code = pn_code ^ (pn_code >> 2);
    pn_code = pn_code ^ (pn_code >> 1);
    return pn_code;
} // todec

////////////////////////////////////////////////////////////////////////////////

inline void beatorbit_entry(char *pp_line) {
    unsigned char ln_code;
    char *lp_char, *lp_cur;
    bool lb_printdelim = false;

    if (!pp_line || !pp_line[0]) return;

    MACRODBGOUTPUT("pp_line = %s", pp_line);

    for (lp_char = strtok(pp_line, ELEMDELIMSTR); lp_char != NULL; lp_char = strtok(NULL, ELEMDELIMSTR)) {
        for (ln_code = 0, lp_cur = lp_char; lp_cur < lp_char + strlen(lp_char); lp_cur++) {
            if (*lp_cur != '0' && *lp_cur != '1') continue;
            ln_code <<= 1;
            ln_code |= *lp_cur - '0';
        }
        if (lb_printdelim) printf(" | ");
        printf("%d", todec(ln_code));
        if (!lb_printdelim) lb_printdelim = true;
    } // for

    printf("\n");
} // beatorbit_entry()

////////////////////////////////////////////////////////////////////////////////

//#include <stdio.h>
//#include <stdlib.h>
//#include "macrodef.h"
//#include "readbuf.h"
//#include "beatorbit.h"

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
            beatorbit_entry(lo_readbuf.mp_buf);
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

////////////////////////////////////////////////////////////////////////////////