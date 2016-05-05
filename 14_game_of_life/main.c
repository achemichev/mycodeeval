#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
//#include "macrodef.h"

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

#ifndef INC_THISPROJECT_H
#define INC_THISPROJECT_H

/*
typedef enum {
    false, true
} bool;
*/

inline void bit_set_alive(unsigned char *pp_grid, int pn_cur_bit);

#endif //INC_THISPROJECT_H

////////////////////////////////////////////////////////////////////////////////
// thisproject.c

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <limits.h>
//#include "macrodef.h"
//#include "thisproject.h"

////////////////////////////////////////////////////////////////////////////////

#define ITERSTOP 10
#define NUMNEIGHBORSHIFTS 8
#define UNEXPECTEDN -1

////////////////////////////////////////////////////////////////////////////////

inline unsigned char bit_shift(int pn_cur_bit) {
    return (unsigned char) (1 << (CHAR_BIT - pn_cur_bit % CHAR_BIT - 1));
} // bit_shift()

////////////////////////////////////////////////////////////////////////////////

inline void bit_set_alive(unsigned char *pp_grid, int pn_cur_bit) {
    pp_grid[pn_cur_bit / CHAR_BIT] |= bit_shift(pn_cur_bit);
} // bit_set_alive

////////////////////////////////////////////////////////////////////////////////

inline unsigned char bit_is_alive(unsigned char *pp_grid, int pn_cur_bit) {
    return pp_grid[pn_cur_bit / CHAR_BIT] & bit_shift(pn_cur_bit);
} // bit_is_alive()

////////////////////////////////////////////////////////////////////////////////

inline void grid_output(unsigned int pn, unsigned char *pp_grid) {
    unsigned int ln_sqrpn;
    int ln_cur_bit;
    char *ls_output_line;
    size_t ln_len;

    ln_sqrpn = pn * pn;
    ln_len = pn + 1;
    ls_output_line = (char *) malloc(ln_len);
    ls_output_line[ln_len - 1] = '\0';
    memset(ls_output_line, '.', ln_len - 1);
    MACRODBGOUTPUT("%s", "pp_grid...");
#ifdef DBG
    char ls_tabfmt[] = "\t%d:\t";
    printf(ls_tabfmt, 0);
#endif
    for (ln_cur_bit = 0; ln_cur_bit < ln_sqrpn; ln_cur_bit++) {
        if (bit_is_alive(pp_grid, ln_cur_bit)) {
            ls_output_line[ln_cur_bit % pn] = '*';
        }
        if (((ln_cur_bit + 1) % pn) == 0 && (ln_cur_bit + 1) != ln_sqrpn) { // eol
            printf("%s\n", ls_output_line);
            memset(ls_output_line, '.', ln_len - 1);
#ifdef DBG
            printf(ls_tabfmt, (ln_cur_bit + 1) / pn);
#endif
        } // eol
    } // for
    printf("%s\n", ls_output_line);
    free(ls_output_line);
} // grid_output()

////////////////////////////////////////////////////////////////////////////////

inline void init_shifts(int *pp_neighbor_shifts, unsigned int pn) {
    pp_neighbor_shifts[0] = 0 - pn - 1; // top left
    pp_neighbor_shifts[1] = 0 - pn; // top
    pp_neighbor_shifts[2] = 0 - pn + 1; // top right
    pp_neighbor_shifts[3] = 0 - 1; // left
    pp_neighbor_shifts[4] = 1; // right
    pp_neighbor_shifts[5] = pn - 1; // bottom left
    pp_neighbor_shifts[6] = pn; // bottom
    pp_neighbor_shifts[7] = pn + 1; // bottom right
} // init_shifts()

////////////////////////////////////////////////////////////////////////////////

inline unsigned int count_live_neighbors(unsigned int pn, unsigned char *pp_grid, int pn_cur_bit,
                                         int *pp_neighbor_shifts) {
    unsigned int k, ln_sqrpn, ln_cnt_live_neighbors;
    int ln_neighbor;

    ln_sqrpn = pn * pn;
    ln_cnt_live_neighbors = 0;
    for (k = 0; k < NUMNEIGHBORSHIFTS; k++) {
        if (k == 2 || k == 4 || k == 7) if ((pn_cur_bit + 1) % pn == 0) continue; // right neighbors border
        if (k == 0 || k == 3 || k == 5) if ((pn_cur_bit % pn) == 0) continue; // left neighbors border
        ln_neighbor = pn_cur_bit + pp_neighbor_shifts[k];
        if (k < 3 && ln_neighbor < 0) continue; // top neighbors border
        if (k > 4 && ln_neighbor > ln_sqrpn - 1) continue; // bottom neighbors border
        if (bit_is_alive(pp_grid, ln_neighbor)) {
            ln_cnt_live_neighbors++;
        } // live neighbor
    }
    return ln_cnt_live_neighbors;

} // count_live_neighbors()

////////////////////////////////////////////////////////////////////////////////

inline void thisproject_entry(unsigned int pn, unsigned char *pp_grid, size_t pn_grid_size) {
    unsigned char *lp_next_grid, *lp_next_grid_original, *lp_gridtmp;
    unsigned int i, ln_sqrpn, ln_cnt_live_neighbors;
    int ln_cur_bit;
    int lp_neighbor_shifts[NUMNEIGHBORSHIFTS];
    unsigned char lb_cur_live; // bool

    ln_sqrpn = pn * pn;
    MACRODBGOUTPUT("pn = %d", pn);
    lp_next_grid = (unsigned char *) malloc(pn_grid_size);
    lp_next_grid_original = lp_next_grid;
    init_shifts(lp_neighbor_shifts, pn);
#ifdef DBG
    grid_output(pn, pp_grid);
#endif
    for (i = 0; i < ITERSTOP; i++) {
        memset(lp_next_grid, 0, pn_grid_size);
        for (ln_cur_bit = 0; ln_cur_bit < ln_sqrpn; ln_cur_bit++) {
            ln_cnt_live_neighbors = count_live_neighbors(pn, pp_grid, ln_cur_bit, lp_neighbor_shifts);
            lb_cur_live = bit_is_alive(pp_grid, ln_cur_bit); // cur cell is alive
            if (ln_cnt_live_neighbors == 3 || (ln_cnt_live_neighbors == 2 && lb_cur_live)) {
                bit_set_alive(lp_next_grid, ln_cur_bit);
            }
        } // for ln_cur_bit
        lp_gridtmp = pp_grid;
        pp_grid = lp_next_grid;
        lp_next_grid = lp_gridtmp;
#ifdef DBG
        grid_output(pn, pp_grid);
#endif
    } // iter
    // release output:
    grid_output(pn, pp_grid);
    free(lp_next_grid_original);

} // thisproject_entry()

// thisproject.c
////////////////////////////////////////////////////////////////////////////////
// main.c

//#include <stdio.h>
//#include <stdlib.h>
//#include "macrodef.h"
//#include "thisproject.h"

////////////////////////////////////////////////////////////////////////////////

inline unsigned int detect_n(FILE *pp_file) {
    int ln_char;
    unsigned int ln_cur_bit;

    ln_cur_bit = 0;
    do {
        ln_char = fgetc(pp_file);
        if (ln_char == '\n' || ln_char == '\r') {
            if (ln_cur_bit > 0) return ln_cur_bit;
        }
        else if (ln_char == '*' || ln_char == '.') {
            ln_cur_bit++;
        }
        else if (feof(pp_file)) {
            MACRODBGOUTPUT("%s", "Unexpected eof...");
            return (unsigned int) UNEXPECTEDN;
        }
        else {
            MACRODBGOUTPUT("%s", "Unexpected symbol...");
            return (unsigned int) UNEXPECTEDN;
        }
    } while (1);
} // detect_n()

////////////////////////////////////////////////////////////////////////////////

inline int scan_to_eof(FILE *pp_file, unsigned int pn, unsigned char *pp_grid, size_t pn_grid_size) {
    int ln_char, ln_cur_bit;

    ln_cur_bit = 0;
    do {
        ln_char = fgetc(pp_file);
        if (feof(pp_file)) {
            thisproject_entry(pn, pp_grid, pn_grid_size);
            break;
        }
        else if (ln_char == '\n' || ln_char == '\r') {
            continue; // OK
        }
        else if (ln_char == '*' || ln_char == '.') {
            if (ln_char == '*') {
                bit_set_alive(pp_grid, ln_cur_bit);
            }
            ln_cur_bit++;
        }
        else {
            MACRODBGOUTPUT("%s", "Unexpected symbol...");
            return ERREXITCODE;
        }
    } while (1);

    return SUCCESSEXITCODE;
} // scan_to_eof()

////////////////////////////////////////////////////////////////////////////////

int main(int pn_argc, const char *pp_argv[]) {

    FILE *lp_file;
    unsigned int ln;
    unsigned char *lp_grid;
    size_t ln_grid_size;

    lp_file = fopen(pp_argv[1], "r");
    if (lp_file == NULL) return ERREXITCODE;

    ln = detect_n(lp_file);
    if (ln == UNEXPECTEDN) {
        fclose(lp_file);
        return ERREXITCODE;
    }

    rewind(lp_file);

    ln_grid_size = ln * ln / CHAR_BIT + 1;
    lp_grid = (unsigned char *) malloc(ln_grid_size);
    memset(lp_grid, 0, ln_grid_size);

    if (scan_to_eof(lp_file, ln, lp_grid, ln_grid_size) == ERREXITCODE) {
        fclose(lp_file);
        free(lp_grid);
        return ERREXITCODE;
    }

    fclose(lp_file);
    free(lp_grid);
    return SUCCESSEXITCODE;
} // main()

// main.c
////////////////////////////////////////////////////////////////////////////////