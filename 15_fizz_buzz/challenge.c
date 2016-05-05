
////////////////////////////////////////////////////////////////////////////////
// challenge.c

#include "macrodef.h"
#include "challenge.h"

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
