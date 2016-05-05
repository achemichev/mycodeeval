#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "macrodef.h"
#include "huffman.h"

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