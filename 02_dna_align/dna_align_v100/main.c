#include <stdio.h>
#include <string.h>
#include <ctype.h>

// #define DBGOUTPUT

#define MAX(a, b) (((a)>(b))?(a):(b))

#define MAXSEQLENGTH 512
#define MAXLINELENGTH 1024
//#define MAXSCOREMATRIXSIZE MAXSEQLENGTH

#define NEGATIVEINFINITY -99999
#define SEQSTARTSYMB '?'

#define SCOREMATCH 3
#define SCOREMISMATCH -3
#define SCOREINDELOPEN -8
#define SCOREINDELEXT -1

const char base[] = "ACGT";

////////////////////////////////////////////////////////////////

inline char *trim(char *s) {
    // leading
    while (isspace(*s)) s++;
    if (*s == '\0') return s;

    // trailing
    char *c = s + strlen(s) - 1;
    while (c > s && isspace(*c)) c--;
    *(c + 1) = '\0';

    return s;
}

inline char *trim_upper_check(char *s) {
    s = trim(s);
    if (*s == '\0') return s;
    char *c;
    for (c = s; *c; c++) {
        *c = toupper(*c);
        if (!strchr(base, *c))return NULL;
    }
    return s;
} // trim_upper_check()

////////////////////////////////////////////////////////////////

inline void print_best_score(char *str1, char *str2) {

    int len1 = strlen(str1), len2 = strlen(str2);
    if (len1 > MAXSEQLENGTH - 2 || len2 > MAXSEQLENGTH - 2) return;

    len1++, len2++;
    char seq1[len1 + 1], seq2[len2 + 1];
    seq1[0] = SEQSTARTSYMB, seq1[1] = '\0', strcat(seq1, str1);
    seq2[0] = SEQSTARTSYMB, seq2[1] = '\0', strcat(seq2, str2);

    int M[len1][len2], H[len1][len2], V[len1][len2], i, j, m, h, v, maxmh;
    M[0][0] = H[0][0] = V[0][0] = 0;

    for (i = 0; i < len1; i++)
        for (j = 0; j < len2; j++) {
            if (i != 0 && j == 0) M[i][0] = M[i - 1][0] + SCOREINDELOPEN;
            if (j != 0 && i == 0) M[0][j] = M[0][j - 1] + SCOREINDELOPEN;

            m = (i == 0 ? NEGATIVEINFINITY : M[i - 1][j]);
            h = (i == 0 ? NEGATIVEINFINITY : H[i - 1][j]);
            H[i][j] = MAX(m + SCOREINDELOPEN, h + SCOREINDELEXT);

            m = (j == 0 ? NEGATIVEINFINITY : M[i][j - 1]);
            v = (j == 0 ? NEGATIVEINFINITY : V[i][j - 1]);
            V[i][j] = MAX(m + SCOREINDELOPEN, v + SCOREINDELEXT);

            if (!(i == 0 && j == 0)) {
                m = ((i == 0 || j == 0) ? NEGATIVEINFINITY : M[i - 1][j - 1]);
                m += (seq1[i] == seq2[j] ? SCOREMATCH : SCOREMISMATCH);
                maxmh = MAX(m, H[i][j]);
                M[i][j] = MAX(maxmh, V[i][j]);
            }
        }

#ifdef DBGOUTPUT
    printf("\t...debug\t\tM:\n");
    printf("\t...debug\t\t   ");
    for (j = 0; j < len2; j++)
        printf("%4c  ", seq2[j]);
    printf("\n");
    for (i = 0; i < len1; i++) {
        printf("\t...debug\t\t%c  ", seq1[i]);
        for (j = 0; j < len2; j++)
            printf("%+4d  ", M[i][j]);
        printf("\n");
    }
    printf("\t...debug\t\tscore = %d\n", M[len1 - 1][len2 - 1]);
#endif

    printf("%d\n", M[len1 - 1][len2 - 1]);

} // print_best_score

////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[]) {

    FILE *file = fopen(argv[1], "r");

    char line[MAXLINELENGTH];
    char *str1 = NULL;
    char *str2 = NULL;
    int len = 0;

    while (fgets(line, MAXLINELENGTH, file)) {
        if (line[0] == '\n')continue;
        len = strlen(line);
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';
#ifdef DBGOUTPUT
        printf("\t...debug\tIput string = \"%s\"\n", line);
#endif
        str1 = line;
        str2 = strchr(line, '|');
        *str2 = '\0';
        str2++;

        str1 = trim_upper_check(str1);
        if (str1 == NULL || *str1 == '\0') continue;
        str2 = trim_upper_check(str2);
        if (str2 == NULL || *str2 == '\0') continue;
#ifdef DBGOUTPUT
        printf("\t...debug\t\tString1 = \"%s\"\n", str1);
        printf("\t...debug\t\tString2 = \"%s\"\n", str2);
#endif
        print_best_score(str1, str2);
    } // while

    if (file != NULL)fclose(file);

    return 0;
} // main()