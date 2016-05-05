#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define N 100
#define K (9*N+1)
#define ND 99
#define STRSZ (ND+1)

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
    return (0);
}

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

char **alloc_d(int m)
{
    int k;
    char **d;
    d = malloc(m * sizeof(char*));
    for (k = 0; k < m; k++)
    {
        d[k] = malloc(STRSZ);
        /*
        memset(d[k], '0', STRSZ);
        d[k][STRSZ-1] = 0;
        */
    }
    return d;
}

void generate_dr(char **d, char **r, int m, int q)
{
    int i, j, k;

    for (k = 0; k < m; k++)
    {
        memset(d[k], '0', STRSZ);
        d[k][STRSZ-1] = 0;
    }
    
    for (j = 0; j < 10; j++)
        d[j][ND-1] = '1';
    
    for (i = 2; i <= 2*q; i++)
    {
        for (k = 9*i; k >= 0; k--)
            for (j = k-9; j < k; j++)
                if (j >= 0 && j <= 9*(i-1))
                    sum2a(d[k], d[j]);
        if (!(i%2))
        {
            memcpy(r[i/2-1], d[9*i/2], STRSZ);
            r[i/2-1][STRSZ-1] = 0;
        }
    }
}

void print_d(char **d, int m)
{
    int k;
    char *s;
    for (k = 0; k < m; k++)
    {
        s = d[k];
        while(*s == '0')
            s++;
        printf("%s ", *s ? s : "-");
    }
    printf("\n");
}

void free_d(char **d, int m)
{
    int k;
    for (k = 0; k < m; k++)
        free(d[k]);
    free(d);
}

void process_line(FILE *f, int start, int end, int num, char **r)
{
    int i, n, bkp = ftell(f);
    char c, cprev = ' ';
    int *a = &n; 

    fseek(f, start, SEEK_SET);
    for (i = -1; start < end; start++, cprev = c)
        if (is_digit(c = fgetc(f)))
        {
            if (is_digit(cprev))
                a[i] = 10*a[i] + (c-'0');
            else
                a[++i] = c-'0';
        }
    
    char *s = r[n/2-1];
    while(*s == '0')
        s++;
    printf("%s\n", s);
    fseek(f, bkp, SEEK_SET);
}

int main(int argc, char *argv[])
{    
    int num = 0, start = 0, end;
    char c, cprev = ' ';
    FILE *file = fopen(argv[1], "r");
    char **d = alloc_d(K);
    char **r = alloc_d(N/2);

    generate_dr(d, r, K, N/2);
    /* print_d(d, K); */
    /* print_d(r, N/2); */
 
    do
    {
        c = fgetc(file);
        if (feof(file) || c == '\n')
            if (num > 0 )
            {
                end = ftell(file);
                process_line(file, start, end, num, r);
                start = end;
                num = 0;
            }
        if (!is_digit(cprev) && is_digit(c) )
            num++;
        cprev = c;
    } while (!feof(file));

    free_d(d, K);
    free_d(r, N/2);
    fclose(file);
    return (0);
}
