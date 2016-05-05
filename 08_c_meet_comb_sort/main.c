#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DBG

#ifdef DBG
#define MACRODBGOUTPUT(fmt, s) printf("  ...debug  "),     printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf("  ...debug      "), printf(fmt, s), printf("\n")
#else
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)
#endif

////////////////////////////////////////////////////////////////////////////////

typedef enum {
    false, true
} bool;

////////////////////////////////////////////////////////////////////////////////

#define ELEMDELIMSTR " "
#define ELEMDELIMCHAR ' '

////////////////////////////////////////////////////////////////////////////////

inline int get_numoflines(const char *ps_filename) {
    FILE *lp_file;
    int ln_numoflines;
    int ln_char;

    MACRODBGOUTPUT("%s", "get_numoflines func:");
    lp_file = fopen(ps_filename, "r");

    ln_numoflines = 0;
    do {
        ln_char = fgetc(lp_file);
        if (feof(lp_file) || ln_char == '\n') ln_numoflines++;
        if (feof(lp_file)) break;
    } while (1);
    fclose(lp_file);

    MACRODBGOUTPUT("ln_numoflines =  %d", ln_numoflines);
    return ln_numoflines;
} // get_numoflines()

////////////////////////////////////////////////////////////////////////////////

inline void alloc_lines(const char *ps_filename, char **pp_lines) {
    FILE *lp_file;
    int i, ln_curlinelen, ln_char;
    size_t ln_mem2alloc;

    MACRODBGOUTPUT("%s", "alloc_lines func:");
    lp_file = fopen(ps_filename, "r");
    i = 0;
    ln_curlinelen = 0;

    do {
        ln_char = fgetc(lp_file);
        if (feof(lp_file) || ln_char == '\n') {
            ln_mem2alloc = (size_t) ((ln_curlinelen + 1) * sizeof(char));
            pp_lines[i] = (char *) malloc(ln_mem2alloc);
            memset(pp_lines[i], 0, ln_mem2alloc);
            MACRODBGOUTPUT("line number=  %d", i);
            MACRODBGOUTPL2("ln_curlinelen =  %d", ln_curlinelen);
            MACRODBGOUTPL2("ln_mem2alloc =  %d", ln_mem2alloc);
            i++;
            ln_curlinelen = 0;
        }
        else ln_curlinelen++;
        if (feof(lp_file)) break;
    } while (1);

    fclose(lp_file);
} // alloc_lines()

////////////////////////////////////////////////////////////////////////////////

inline void set_lines(const char *ps_filename, char **pp_lines) {
    FILE *lp_file;
    int i, ln_curoffset, ln_char;

    MACRODBGOUTPUT("%s", "set_lines func:");
    lp_file = fopen(ps_filename, "r");
    i = 0;
    ln_curoffset = 0;

    do {
        ln_char = fgetc(lp_file);
        if (feof(lp_file) || ln_char == '\n') {
            *(pp_lines[i] + ln_curoffset) = '\0';
            MACRODBGOUTPUT("line number=  %d", i);
            MACRODBGOUTPL2("pp_lines[i] =  %s", pp_lines[i]);
            i++;
            ln_curoffset = 0;
        }
        else {
            *(pp_lines[i] + ln_curoffset) = ln_char;
            ln_curoffset++;
        }
        if (feof(lp_file)) break;
    } while (1);

    fclose(lp_file);
} // set_lines()

////////////////////////////////////////////////////////////////////////////////

inline short count_size(char **pp_line) {
    short ln_size;
    char *lp_char;

    ln_size = 1;
    for (lp_char = *pp_line; *lp_char; lp_char++)
        if (*lp_char == ELEMDELIMCHAR)
            ln_size++;

    MACRODBGOUTPUT("number of elements, ln_size = %d", ln_size);
    return ln_size;
}

////////////////////////////////////////////////////////////////////////////////

inline void process_line(char **pp_line) {
    int *la;
    int i, t, ln_len, ln_numofiter, ln_offset;
    char *lp_char;
    size_t ln_mem2alloc;
    const float lf_factor = 1.25;
    bool lb_swap;

    MACRODBGOUTPUT("%s", "process_line func:");
    if (!(*pp_line) || !(*pp_line[0])) return;

    ln_len = count_size(pp_line);
    ln_mem2alloc = (size_t) (ln_len * sizeof(int));
    la = (int *) malloc(ln_mem2alloc);
    memset(la, 0, ln_mem2alloc);

    i = 0;
    for (lp_char = strtok(*pp_line, ELEMDELIMSTR); lp_char != NULL; lp_char = strtok(NULL, ELEMDELIMSTR)) {
        sscanf(lp_char, "%d", &la[i]);
        i++;
    }

    ln_offset = ln_len;
    ln_numofiter = 0;
    lb_swap = true;
    while (ln_offset > 1 || lb_swap) {
        ln_offset /= lf_factor;
        lb_swap = false;
        for (i = 0; i + ln_offset < ln_len; i++)
            if (la[i] > la[i + ln_offset]) {
                t = la[i];
                la[i] = la[i + ln_offset];
                la[i + ln_offset] = t;
                lb_swap = true;
            }
        if (ln_offset > 1 || lb_swap) ln_numofiter++;
    }

    // release output
    printf("%d\n", ln_numofiter);

    free(la);
} // process_line()

////////////////////////////////////////////////////////////////////////////////

inline void process_lines(int pn_numoflines, char **pp_lines) {
    int i;
    for (i = 0; i < pn_numoflines; i++)
        if (pp_lines[i])
            process_line(&pp_lines[i]);
} // process_lines()

////////////////////////////////////////////////////////////////////////////////

int main(int pn_argc, const char *pp_argv[]) {

    int i, ln_numoflines;
    size_t ln_mem2alloc;
    char **lp_lines;

    ln_numoflines = get_numoflines(pp_argv[1]);
    ln_mem2alloc = (size_t) (ln_numoflines * sizeof(char *));
    lp_lines = (char **) malloc(ln_mem2alloc);
    memset(lp_lines, 0, ln_mem2alloc);
    alloc_lines(pp_argv[1], lp_lines);
    set_lines(pp_argv[1], lp_lines);
    process_lines(ln_numoflines, lp_lines);

    for (i = 0; i < ln_numoflines; i++)
        if (lp_lines[i])
            free(lp_lines[i]);
    free(lp_lines);
    return (0);
}

