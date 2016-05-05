#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

//#define DBG

#ifdef DBG
#define MACRODBGOUTPUT(fmt, s) printf("  ...debug  "),     printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf("  ...debug      "), printf(fmt, s), printf("\n")
#else
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// #define SCHAR_MAX	127 // see <limits.h>
#define MAXLINELENGTH 2048
#define ROWDELIMSTR "|"
#define ROWDELIMCHAR '|'
#define ELEMDELIMSTR " "
#define ELEMDELIMCHAR ' '

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
    false, true
} bool;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// globs

char gs_input_line[MAXLINELENGTH];
short gn_size = 0;
unsigned char *ga_matrix = NULL;
bool gb_suppress_first_nl = true;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline short count_size() {
    short ln_size;
    char *lp_char;

    ln_size = 1;
    for (lp_char = gs_input_line; *lp_char; lp_char++)
        if (*lp_char == ROWDELIMCHAR)
            ln_size++;

    MACRODBGOUTPUT("number of rows, k = %d", ln_size);
    return ln_size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void alloc_matrix() {
    size_t ln_alloc_size;

    ln_alloc_size = (size_t) (gn_size * gn_size * sizeof(unsigned char));
    ga_matrix = (unsigned char *) malloc(ln_alloc_size);
    memset(ga_matrix, 0, ln_alloc_size);

    MACRODBGOUTPUT("matrix pointer, ga_matrix = 0x%x", (unsigned int) ga_matrix);
    MACRODBGOUTPUT("memory allocated for ga_matrix (bytes), ln_alloc_size  = %d", ln_alloc_size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void init_matrix() {
    short i, j;
    int ln_int_buf;
    char *lp_char, **la_rows;

    la_rows = (char **) malloc((size_t) (gn_size * sizeof(char *)));

    MACRODBGOUTPUT("rows pointer, la_rows = 0x%x", (unsigned int) la_rows);
    MACRODBGOUTPUT("memory allocated for la_rows (bytes), gn_size * sizeof(char *) = %d", gn_size * sizeof(char *));

    i = 0;
    for (lp_char = strtok(gs_input_line, ROWDELIMSTR); lp_char != NULL; lp_char = strtok(NULL, ROWDELIMSTR)) {
        la_rows[i] = lp_char;
        i++;
    }

#ifdef DBG
    if (i != gn_size) {
        MACRODBGOUTPUT("%s", "unexpected format, error...");
        free(la_rows);
        return;
    }
#endif

    for (i = 0; i < gn_size; i++) {
        j = 0;
        for (lp_char = strtok(la_rows[i], ELEMDELIMSTR); lp_char != NULL; lp_char = strtok(NULL, ELEMDELIMSTR)) {
            sscanf(lp_char, "%d", &ln_int_buf);
            ga_matrix[i + gn_size * j] = (unsigned char) (ln_int_buf + SCHAR_MAX);
            j++;
        }
#ifdef DBG
        if (j != gn_size) {
            MACRODBGOUTPUT("%s", "unexpected format error...");
            free(la_rows);
            return;
        }
#endif
    } // for

    free(la_rows);
} // init_matrix

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int cmp_func(const void *pp_row1, const void *pp_row2) {
    return memcmp(pp_row1, pp_row2, gn_size * sizeof(ga_matrix[0]));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void sortcols() {
    short i, j;
    int ln_int_buf;

    gn_size = count_size();
    alloc_matrix(); // free(ga_matrix) needed
    init_matrix();
    qsort(ga_matrix, gn_size, gn_size * sizeof(ga_matrix[0]), cmp_func);

    // release output
    if (!gb_suppress_first_nl)
        printf("\n");
    for (i = 0; i < gn_size; i++) {
        for (j = 0; j < gn_size; j++) {
            ln_int_buf = (int) ga_matrix[i + gn_size * j];
            printf("%d", ln_int_buf - SCHAR_MAX);
            if (j < gn_size - 1) printf("%c", ELEMDELIMCHAR);
        }
        if (i < gn_size - 1) printf("%c%c%c", ELEMDELIMCHAR, ROWDELIMCHAR, ELEMDELIMCHAR);
    }
    gb_suppress_first_nl = false;

    free(ga_matrix);
} // sortcols

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int pn_argc, const char *ps_argv[]) {
    FILE *lf_file = fopen(ps_argv[1], "r");
    while (fgets(gs_input_line, MAXLINELENGTH, lf_file))
        sortcols();
    fclose(lf_file);
    return 0;
}