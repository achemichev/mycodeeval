#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
//#include "macrodef.h"
//#include "readbuf.h"
//#include "huffman.h"

#ifndef INC_09_C_CODE_LIKE_HUFFMAN_MACRODEF_H
#define INC_09_C_CODE_LIKE_HUFFMAN_MACRODEF_H

// #define DBG

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

#ifndef INC_09_C_CODE_LIKE_HUFFMAN_HUFFMAN_H
#define INC_09_C_CODE_LIKE_HUFFMAN_HUFFMAN_H

////////////////////////////////////////////////////////////////////////////////

#define CHAROFFSET 'a'
#define MAXALPHABETSIZE 'z' - CHAROFFSET + 1

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

inline void huffman_entry(char *pp_line);

////////////////////////////////////////////////////////////////////////////////

#endif //INC_09_C_CODE_LIKE_HUFFMAN_HUFFMAN_H

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
//#include "huffman.h"

////////////////////////////////////////////////////////////////////////////////

#define BINBUFSIZE 32

////////////////////////////////////////////////////////////////////////////////

inline unsigned char count_freq(unsigned char *pp_freq, char *pp_line, unsigned char *pn_maxprio) {
    unsigned char ln_cnt;
    (*pn_maxprio) = ln_cnt = 0;
    while (*pp_line) {
        (*pn_maxprio)++;
        if (!pp_freq[*pp_line++ - CHAROFFSET]++)
            ln_cnt++;
    }
    return ln_cnt;
} // count_freq()

////////////////////////////////////////////////////////////////////////////////

inline void init_tree(node_t **pp_tree, unsigned char *pp_freq, unsigned char pn_alphabetsize) {
    unsigned char i, k;
    for (i = 0, k = 0; i < MAXALPHABETSIZE; i++)
        if (pp_freq[i]) {
            pp_tree[k] = malloc((size_t) sizeof(node_t));
            memset(pp_tree[k], 0, sizeof(node_t));

            pp_tree[k]->mp_chars = malloc((size_t) (pn_alphabetsize + 1));
            memset(pp_tree[k]->mp_chars, 0, pn_alphabetsize + 1);
            pp_tree[k]->mp_chars[0] = (char) (i + CHAROFFSET);

            pp_tree[k]->mn_prio = pp_freq[i];
            k++;
        }
} // init_tree()

////////////////////////////////////////////////////////////////////////////////

// not inlined
int treeel_cmp(const void *a, const void *b) {
    node_t *lp_a, *lp_b;
    unsigned char ln_prio_a, ln_prio_b, ln_charslen_a, ln_charslen_b;

    lp_a = *((node_t **) a);
    lp_b = *((node_t **) b);
    ln_prio_a = lp_a->mn_prio;
    ln_prio_b = lp_b->mn_prio;
    if (lp_a->mb_has_parent) ln_prio_a = UCHAR_MAX;
    if (lp_b->mb_has_parent) ln_prio_b = UCHAR_MAX;
    ln_charslen_a = strlen(lp_a->mp_chars);
    ln_charslen_b = strlen(lp_b->mp_chars);

    if (ln_prio_a == ln_prio_b) {
        if (ln_charslen_a == ln_charslen_b)
            return strcmp(lp_a->mp_chars, lp_b->mp_chars);
        return ln_charslen_b - ln_charslen_a;
    }
    return ln_prio_a - ln_prio_b;
} // treeel_cmp()

////////////////////////////////////////////////////////////////////////////////

inline void build_tree(node_t **pp_tree, unsigned char pn_alphabetsize) {
    unsigned char ln_numoftreeel, ln_numofroots, i;
    ln_numoftreeel = ln_numofroots = pn_alphabetsize;
    while (ln_numofroots > 1) {
        qsort(pp_tree, ln_numoftreeel, sizeof(node_t *), treeel_cmp);

        pp_tree[ln_numoftreeel] = malloc((size_t) sizeof(node_t));
        memset(pp_tree[ln_numoftreeel], 0, sizeof(node_t));
        pp_tree[ln_numoftreeel]->mp_chars = malloc((size_t) (pn_alphabetsize + 1));
        memset(pp_tree[ln_numoftreeel]->mp_chars, 0, pn_alphabetsize + 1);

        strcpy(pp_tree[ln_numoftreeel]->mp_chars, pp_tree[0]->mp_chars);
        strcat(pp_tree[ln_numoftreeel]->mp_chars, pp_tree[1]->mp_chars);
        pp_tree[ln_numoftreeel]->mn_prio = pp_tree[0]->mn_prio + pp_tree[1]->mn_prio;

        pp_tree[ln_numoftreeel]->mp_left_child = pp_tree[0];
        pp_tree[ln_numoftreeel]->mp_right_child = pp_tree[1];
        pp_tree[0]->mb_has_parent = true;
        pp_tree[1]->mb_has_parent = true;

#ifdef DBG
        printf("%sTree:\n", DBGPREFIX);
        for (i = 0; i < ln_numoftreeel + 1; i++)
            if (pp_tree[i])
                printf("%sEloffset=%d, chars=%s, hasparent=%d, left=0x%x, right=0x%x, prio=%d, code=%d\n", DBGPREFIX,
                       i,
                       pp_tree[i]->mp_chars,
                       pp_tree[i]->mb_has_parent, pp_tree[i]->mp_left_child, pp_tree[i]->mp_right_child,
                       pp_tree[i]->mn_prio, pp_tree[i]->mn_code);
#endif

        ln_numoftreeel++;
        ln_numofroots--;
    }
} // build_tree()

////////////////////////////////////////////////////////////////////////////////

// not inlined
void tree_recur(node_t *pp_root, int pn_code) {
    if (pp_root->mp_left_child) {
        tree_recur(pp_root->mp_left_child, pn_code << 1);
        tree_recur(pp_root->mp_right_child, (pn_code << 1) + 1);
    }
    else pp_root->mn_code = pn_code;
} // tree_recur()

////////////////////////////////////////////////////////////////////////////////

// not inlined
int treeel_cmp_simple(const void *a, const void *b) {
    node_t *lp_a, *lp_b;
    unsigned char ln_prio_a, ln_prio_b, ln_charslen_a, ln_charslen_b;

    ln_prio_a = 0;
    ln_prio_b = 0;
    lp_a = *((node_t **) a);
    lp_b = *((node_t **) b);
    ln_charslen_a = strlen(lp_a->mp_chars);
    ln_charslen_b = strlen(lp_b->mp_chars);
    if (ln_charslen_a > 1) ln_prio_a = UCHAR_MAX;
    if (ln_charslen_b > 1) ln_prio_b = UCHAR_MAX;

    if (ln_prio_a == ln_prio_b) {
        if (ln_charslen_a == ln_charslen_b)
            return strcmp(lp_a->mp_chars, lp_b->mp_chars);
        return ln_charslen_b - ln_charslen_a;
    }
    return ln_prio_a - ln_prio_b;
} // treeel_cmp_simple()

////////////////////////////////////////////////////////////////////////////////

inline void free_tree(node_t **pp_tree, unsigned char pn_numoftreeel) {
    unsigned char i;
    for (i = 0; i < pn_numoftreeel; i++)
        if (pp_tree[i]) {
            free(pp_tree[i]->mp_chars);
            free(pp_tree[i]);
        }
    free(pp_tree);
} // free_tree()

////////////////////////////////////////////////////////////////////////////////

inline void huffman_entry(char *pp_line) {
    unsigned char la_freq[
            MAXALPHABETSIZE];
    unsigned char ln_alphabetsize, ln_maxnumoftreeel, ln_maxprio, i, ln_code;
    node_t **lp_tree;
    char ls_binbuf[BINBUFSIZE];
    char *lp_bincur;

    MACRODBGOUTPUT("pp_line = %s", pp_line);

    memset(la_freq, 0, MAXALPHABETSIZE);
    ln_alphabetsize = count_freq(la_freq, pp_line, &ln_maxprio);
    MACRODBGOUTPUT("ln_alphabetsize = %d", ln_alphabetsize);
    MACRODBGOUTPUT("ln_maxprio = %d", ln_maxprio);

    ln_maxnumoftreeel = (unsigned char) (2 * ln_alphabetsize - 1);
    lp_tree = malloc((size_t) (ln_maxnumoftreeel * sizeof(node_t *)));
    memset(lp_tree, 0, ln_maxnumoftreeel * sizeof(node_t *));

    init_tree(lp_tree, la_freq, ln_alphabetsize);
    build_tree(lp_tree, ln_alphabetsize);

    tree_recur(lp_tree[ln_maxnumoftreeel - 1], 1);

    qsort(lp_tree, ln_maxnumoftreeel, sizeof(node_t *), treeel_cmp_simple);

    MACRODBGOUTPUT("%s", "RELEASE OUTPUT:");
    ls_binbuf[BINBUFSIZE - 1] = '\0';
    for (i = 0; i < ln_alphabetsize; i++)
        if (lp_tree[i]) {
            // itoa(lp_tree[i]->mn_code, ls_binbuf, 2);
            lp_bincur = ls_binbuf + BINBUFSIZE - 2;
            ln_code = lp_tree[i]->mn_code;
            while (ln_code) {
                *lp_bincur = '0' + (ln_code & 1);
                lp_bincur--;
                ln_code >>= 1;
            }
            printf("%s: %s; ", lp_tree[i]->mp_chars, lp_bincur + 2);
        }
    printf("\n");
    // a: 1000; c: 1001; d: 1010; e: 01; i: 1011; l: 110; o: 111; v: 00;

    free_tree(lp_tree, ln_maxnumoftreeel);
} // huffman_entry()

////////////////////////////////////////////////////////////////////////////////

//#include <stdio.h>
//#include <stdlib.h>
//#include "macrodef.h"
//#include "readbuf.h"
//#include "huffman.h"

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
            huffman_entry(lo_readbuf.mp_buf);
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