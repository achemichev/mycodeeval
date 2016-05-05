#include <stdio.h>
#include <string.h>

//#define DBGOUTPUT

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

inline void build_lcs(const char *p,
                      const char *q,
                      const int plen,
                      const int qlen,
                      int csmap[plen + 1][qlen + 1],
                      char *lcs) {

    int i = 0, j = 0, k = 0;

    while (i < plen && j < qlen)
        if (p[i] == q[j]) {
            lcs[k] = p[i];
            i++;
            j++;
            k++;
        }
        else if (csmap[i + 1][j] >= csmap[i][j + 1]) i++;
        else j++;

#ifdef DBGOUTPUT
    printf("\t...debug\t\tbuild_lcs() done, lcs = \"%s\"\n", lcs);
#endif

} // build_lcs()

inline void print_lcs(const char *p, const char *q) {

    int plen = strlen(p);
    int qlen = strlen(q);

    int csmap[plen + 1][qlen + 1];

    build_csmap(p, q, plen, qlen, csmap);

    int lcslen = csmap[0][0];
    char lcs[lcslen + 1];
    lcs[lcslen] = '\0';

    build_lcs(p, q, plen, qlen, csmap, lcs);

    printf("%s\n", lcs);

} // print_lcs()

int main(int argc, const char *argv[]) {

    FILE *file = fopen(argv[1], "r");

    char line[1024];
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

        str2 = strstr(line, ";");
        *str2 = '\0';
        str2++;

#ifdef DBGOUTPUT
        printf("\t...debug\t\tString1 = \"%s\"\n", line);
        printf("\t...debug\t\tString2 = \"%s\"\n", str2);
#endif

        print_lcs(line, str2);
    } // while

    if (file != NULL)fclose(file);

    return 0;
} // main()