#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define NMAX (6+1)
#define ND 112
#define STRSZ (ND+1)

int mult10n2a(char *a, int n)
{
    int i;
    if (n == 0)
        return (0);
    for (i = 0; i < n; i++)
        if (a[i] != '0')
        {
            printf("mult10n2a overflow:\n");
            printf("a=%s\n", a);
            printf("n=%d\n", n);
            return (-1);
        }
    for (i = 0; i < ND-n; i++)
        a[i] = a[i+n];
    for (i = ND-n; i < ND; i++)
        a[i] = '0';
    return (0);
}

int sum2a(char *a, char *b)
{
    int i, sum, next = 0;

    for (i = ND-1; i >= 0; i--)
        if (next || b[i] > '0')
        {
            sum = a[i] - '0' + b[i] - '0' + next;
            a[i] = '0' + sum % 10;
            next = sum / 10;
        }
    if (next > 0)
    {
        printf("sum2a overflow...\n");
        return (-1);
    }
    return (0);
}

int mult2a(char *a, char* b)
{
    int i, k, m, next;

    char mult[STRSZ];

    char sum[STRSZ];
    memset(sum, '0', STRSZ);
    sum[STRSZ-1] = 0;

    for (k = ND-1; k >= 0; k--)
        if (b[k] > '0')
        {
            memset(mult, '0', STRSZ);
            mult[STRSZ-1] = 0;

            next = 0;
            for (i = ND-1; i >= 0; i--)
            {
                m = (a[i] - '0') * (b[k] - '0') + next;
                mult[i] = '0' + m % 10;
                next = m / 10;
            }
            if (next > 0)
            {
                printf("mult2a overflow...\n");
                goto ERROR;
            }
            if (mult10n2a(mult,ND-1-k) || sum2a(sum, mult))
                goto ERROR;
        }
    memcpy(a, sum, STRSZ-1);
    return (0);

ERROR:
    printf("mult2a: a=%s\n", a);
    printf("mult2a: b=%s\n", b);
    return (-1);
}

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

char ***alloc_d()
{
    int k, n;
    char ***d = malloc(NMAX*sizeof(char**));
    for (n = 0; n < NMAX; n++)
    {
        d[n] = malloc((9*n+1) * sizeof(char*));
        for (k = 0; k < 9*n+1; k++)
        {
            d[n][k] = malloc(STRSZ);
            memset(d[n][k], '0', STRSZ);
            d[n][k][STRSZ-1] = 0;
        }
    }
    return d;
}

void generate_d(char ***d)
{
    int n, k, j;
    d[0][0][ND-1] = '1';
    for (n = 1; n < NMAX; n++)
        for (k = 0; k < (9*n + 1); k++)
            for (j = 0; j < 10; j++)
                if (k >= j && k < j + 9*(n-1) + 1)
                    sum2a(d[n][k], d[n-1][k-j]);
}

void print_d(char ***d)
{
    int k, n;
    char *s;
    for (n = 0; n < NMAX; n++, printf("\n"))
        for (k = 0; k < 9*n+1; k++)
        {
            s = d[n][k];
            while(*s == '0')
                s++;
            printf("%s ", s);
        }
}

void free_d(char ***d)
{
    int k, n;
    for (n = 0; n < NMAX; n++)
    {
        for (k = 0; k < 9*n+1; k++)
            free(d[n][k]);
        free(d[n]);
    }
    free(d);
}

void process_line(FILE *f, int start, int end, int num, char ***d)
{
    int i, k, n, doublen, bkp = ftell(f);
    char c, cprev = ' ';
    int *a = &doublen; 
    /* char *sum; */

    fseek(f, start, SEEK_SET);
    for (i = -1; start < end; start++, cprev = c)
        if (is_digit(c = fgetc(f)))
        {
            if (is_digit(cprev))
                a[i] = 10*a[i] + (c-'0');
            else
                a[++i] = c-'0';
        }
    /* printf("2n = %d\n", doublen); */ 
    /* return; */

    char sum[STRSZ];
    memset(sum, '0', STRSZ);
    sum[STRSZ-1] = 0;
    
    char sqr[STRSZ];

    for (n = doublen / 2, k = 0; k <= 9*n; k++)
    {
        memcpy(sqr, d[n][k], STRSZ);
        sqr[STRSZ-1] = 0;
        if (mult2a(sqr, d[n][k]) || sum2a(sum, sqr))
            goto ERROR;
    }
    
    char *s = sum;
    while(*s == '0')
        s++;
    printf("%s\n", s);

    goto EXIT;
ERROR:
    printf("process_line: n=%d\n", n);
EXIT:
    /* free(a); */
    fseek(f, bkp, SEEK_SET);
}

int main(int argc, char *argv[])
{    
    int num = 0, start = 0, end;
    char c, cprev = ' ';
    FILE *file = fopen(argv[1], "r");
    char ***d = alloc_d();
    generate_d(d);
    print_d(d);

    do
    {
        c = fgetc(file);
        if (feof(file) || c == '\n')
            if (num > 0 )
            {
                end = ftell(file);
                process_line(file, start, end, num, d);
                start = end;
                num = 0;
            }
        if (!is_digit(cprev) && is_digit(c) )
            num++;
        cprev = c;
    } while (!feof(file));

    free_d(d);
    fclose(file);
    return (0);
}
