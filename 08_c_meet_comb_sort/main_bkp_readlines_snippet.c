#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBG

#ifdef DBG
#define MACRODBGOUTPUT(fmt, s) printf("  ...debug  "),     printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf("  ...debug      "), printf(fmt, s), printf("\n")
#else
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)
#endif

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
            memset(pp_lines[i],0,ln_mem2alloc);
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

inline void process_line(char **pp_line) {
    MACRODBGOUTPUT("%s", "process_line func:");
    if (!(*pp_line) || !(*pp_line[0])) return;
    MACRODBGOUTPL2("*pp_line =  %s", *pp_line);
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

