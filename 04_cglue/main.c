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

#define MAXLINELENGTH SHRT_MAX // 32767 ?
#define DELIMITERSTR "|"
#define DELIMITERCHAR '|'

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
    false, true
} bool;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// globs

short N = 0;
short n = 0;
short t = 0;
char **P = NULL;
char *S = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline char *truncate_input(char *d) {
    size_t len;
    char *c;
    if (*d == DELIMITERCHAR) d++;
    len = strlen(d);
    for (c = &d[len - 1]; *c != DELIMITERCHAR; c--)
        *c = '\0';
    if (*c == DELIMITERCHAR) *c = '\0';
    MACRODBGOUTPUT("input string shifted and truncated, d = \"%s\"", d);
    return d;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline short count_pieces(char *d) {
    short k;
    char *c;
    for (c = d, k = 0; *c; c++)
        if (*c == DELIMITERCHAR)
            k++;
    MACRODBGOUTPUT("number of pieces, N = %d", k + 1);
    return (short) (k + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void alloc_pieces_pointers() {
    P = (char **) malloc((size_t) (N * sizeof(char *)));
    MACRODBGOUTPUT("pieces pointer, P = 0x%x", (unsigned int) P);
    MACRODBGOUTPUT("memory allocated for P (bytes), mem2alloc = %d", mem2alloc);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void set_pieces_pointers(char *d) {
    short i;
    char *p;
    for (i = 0, p = strtok(d, DELIMITERSTR); p != NULL; p = strtok(NULL, DELIMITERSTR)) {
        P[i] = p;
        i++;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline size_t count_piece_length() {
    MACRODBGOUTPUT("piece length, n = %d", strlen(P[0]));
    return strlen(P[0]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline size_t count_solution_text_length() {
    MACRODBGOUTPUT("solution text length, t = %d", N + n - 1);
    return (size_t) (N + n - 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void alloc_solution_text_string() {
    S = (char *) malloc((size_t) (t + 1));
    S[0] = '\0';
    MACRODBGOUTPUT("solution text pointer, S = 0x%x", (unsigned int) S);
    MACRODBGOUTPUT("memory allocated for S (bytes), mem2alloc = %d", (t + 1));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline char *detect_start_left_piece() {
    short i, j;
    char *start_left_piece, **pclone_right_parts;
    size_t mem2alloc;

    mem2alloc = (size_t) (N * sizeof(char *));
    pclone_right_parts = (char **) malloc(mem2alloc);
    memcpy(pclone_right_parts, P, mem2alloc);

    start_left_piece = NULL;
    for (i = 0; i < N && !start_left_piece; i++)
        for (start_left_piece = P[i], j = 0; j < N && start_left_piece; j++)
            if (!pclone_right_parts[j] || i == j) continue;
            else if (!strncmp(pclone_right_parts[j] + 1, start_left_piece, (size_t) (n - 1)))
                start_left_piece = pclone_right_parts[j] = NULL; // exclude

#ifdef DBG
    if (start_left_piece)
        MACRODBGOUTPUT("start left piece found (use only first n-1 chars), start_left_piece = %s", start_left_piece);
#endif

    free(pclone_right_parts);
    return start_left_piece;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void glue_recur(char *prev, char *new2set) {
    short i;
    char *new_, *best, *prev_plus_char, *pexclude;
    size_t prev_len, best_len, new_len;
    bool *char_tested;

    prev_len = strlen(prev);
    if (prev_len == t) {
        MACRODBGOUTPUT("previous solution of size t found, prev = %s", prev);
        strcpy(new2set, prev);
        return;
    }

    char_tested = (bool *) malloc(sizeof(bool) * UCHAR_MAX);
    memset(char_tested, false, sizeof(bool) * UCHAR_MAX);
    new_ = (char *) malloc((size_t) (t + 1));
    new_[0] = '\0';
    best = (char *) malloc((size_t) (t + 1));
    best[0] = '\0';
    prev_plus_char = (char *) malloc((size_t) (prev_len + 2));
    strcpy(prev_plus_char, prev);

    best_len = 0;

    for (i = 0; i < N; i++)
        if (P[i] && !char_tested[(int)*(P[i] + n - 1)] && !strncmp(prev + prev_len - n + 1, P[i], (size_t) (n - 1))) {
            prev_plus_char[prev_len] = *(P[i] + n - 1);
            prev_plus_char[prev_len + 1] = '\0';
            char_tested[(int)*(P[i] + n - 1)] = true;
            pexclude = P[i];
            P[i] = NULL;
            glue_recur(prev_plus_char, new_);
            P[i] = pexclude;
            new_len = strlen(new_);
            if (new_len > best_len) {
                strcpy(best, new_);
                best_len = new_len;
            }
            if (best_len == t) break;
        }

    if (best_len == 0) strcpy(new2set, prev);
    else strcpy(new2set, best);

    free(new_);
    free(best);
    free(prev_plus_char);
    free(char_tested);

} // glue_recur()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void glue(char *d) {
    char *start_left, *start_left_p;
    d = truncate_input(d);
    N = count_pieces(d);
    alloc_pieces_pointers();
    set_pieces_pointers(d);
    n = (short) count_piece_length();
    t = (short) count_solution_text_length();
    alloc_solution_text_string();

    start_left_p = detect_start_left_piece();
    if (start_left_p != NULL) {
        start_left = (char *) malloc((size_t) n);
        strncpy(start_left, start_left_p, (size_t) (n - 1));
        start_left[n - 1] = '\0';
        glue_recur(start_left, S);
        printf("%s\n", S); // release output
        free(start_left);
    }

    free(P);
    free(S);

    MACRODBGOUTPUT("%s", "\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[MAXLINELENGTH];
    while (fgets(line, MAXLINELENGTH, file))
        glue(line);
    fclose(file);
    return 0;
}