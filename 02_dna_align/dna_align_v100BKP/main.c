#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define DBGOUTPUT

const char base[] = "ACGT";

////////////////////////////////////////////////////////////////

char *trim(char *s) {
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

inline void build_csmap(const char *p,
                        const char *q,
                        const int plen,
                        const int qlen,
                        int csmap[plen + 1][qlen + 1]) {

    int i, j;

    for (i = plen; i >= 0; i--)
        for (j = qlen; j >= 0; j--)
            if (i == plen || j == qlen) csmap[i][j] = 0;
            else if (p[i] == q[j]) csmap[i][j] = csmap[i + 1][j + 1] + 1;
            else
                csmap[i][j] = (csmap[i + 1][j] >= csmap[i][j + 1]) ? csmap[i + 1][j]
                                                                   : csmap[i][j + 1];

#ifdef DBGOUTPUT
    printf("\t...debug\t\tbuild_csmap() done, lcs length = %i\n", csmap[0][0]);
#endif

} // build_csmap()

inline void build_lcs_align(const char *p,
                            const char *q,
                            const int plen,
                            const int qlen,
                            int csmap[plen + 1][qlen + 1],
                            const int n,
                            const int lcs_len,
                            char lcs[n][lcs_len + 1]) {

    int i = 0, j = 0, k = 0;

    while (i < plen && j < qlen) {
        if (p[i] == q[j]) {
            lcs[0][k] = lcs[1][k] = p[i];
            lcs[2][k] = '+';
            i++;
            j++;
        }
        else if (csmap[i + 1][j] >= csmap[i][j + 1]) {
            lcs[0][k] = p[i];
            lcs[1][k] = '?';
            lcs[2][k] = '?';
            i++;
        }
        else {
            lcs[0][k] = '?';
            lcs[1][k] = q[j];
            lcs[2][k] = '?';
            j++;
        }
        k++;
    } // while

#ifdef DBGOUTPUT
    printf("\t...debug\t\build_lcs_align() done, lcs_align = \"%s\"\n", lcs);
#endif

} // build_lcs_align()

inline void print_lcs_spesial(const int n,
                              const int lcs_align_len,
                              char lcs_align[n][lcs_align_len + 1]) {

    char i, j, k;
    int confl_start = -1, sum0, sum1, max, min;

    for (i = 0; i < lcs_align_len; i++)
        if (lcs_align[2][i] == '+') {
            if (confl_start > 0) {
                sum0 = 0;
                sum1 = 0;
                for (j = confl_start; j < i; j++) {
                    if (lcs_align[0][j] != '?')sum0++;
                    if (lcs_align[1][j] != '?')sum1++;
                }
                if (sum0 >= sum1) {
                    max = sum0;
                    min = sum1;
                } else {
                    max = sum1;
                    min = sum0;
                }
                for (k = 0; k < min; k++)printf("%c", 'm');
                for (k = 0; k < max - min; k++)printf("%c", '-');
                for (k = max; k < i - confl_start; k++)printf("%c", ' ');
                confl_start = -1;
            }
            if (lcs_align[0][i] == 'x' || lcs_align[0][i] == 'y') printf("%c", ' ');
            else printf("%c", lcs_align[2][i]);
            //else printf("%c", lcs_align[0][i]);
        }
        else if (confl_start < 0) confl_start = i;
} // print_lcs_spesial()

inline void print_lcs_align(const char *str1, const char *str2) {

    int s1len = strlen(str1);
    int s2len = strlen(str2);

    int plen = s1len + 2;
    int qlen = s2len + 2;

    char p[plen + 1];
    char q[qlen + 1];

    *p = *q = 'x';
    strncpy(p + 1, str1, s1len);
    strncpy(q + 1, str2, s2len);
    p[plen - 1] = q[qlen - 1] = 'y';
    p[plen] = q[qlen] = '\0';

    int csmap[plen + 1][qlen + 1];

    build_csmap(p, q, plen, qlen, csmap);

    int lcs_align_len = plen + qlen - csmap[0][0];
    char lcs_align[3][lcs_align_len + 1];
    lcs_align[0][lcs_align_len] = lcs_align[1][lcs_align_len] = lcs_align[2][lcs_align_len] = '\0';

    build_lcs_align(p, q, plen, qlen, csmap, 3, lcs_align_len, lcs_align);

    printf("%s\n", lcs_align[0]);
    printf("%s\n", lcs_align[1]);
    printf("%s\n", lcs_align[2]);

    print_lcs_spesial(3, lcs_align_len, lcs_align);

} // print_lcs_align()

////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[]) {

    FILE *file = fopen(argv[1], "r");

    char line[1024];
    char *str1 = NULL;
    char *str2 = NULL;
    int len = 0;

    while (fgets(line, 1024, file)) {
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
        print_lcs_align(str1, str2);
    } // while

    if (file != NULL)fclose(file);

    return 0;
} // main()