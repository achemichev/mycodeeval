#include <stdlib.h>
#include <stdio.h>

//#define DBGFLAG

int is_alpha(char c)
{
    if ((c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z'))
    {
        return (1);
    }
    return (0);
}

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

void print(int *m, int n, char nl)
{
    int i;
    for (i = 0; i < n*n; i++)
    {
        printf("%d", m[i]);
        if ((i+1)%n)
            printf(" ");
        else if ((i+1)%(n*n))
            printf("%c", nl);
        else
            printf("\n");
    }
}

int shift_needed(int *r, int n)
{
    int i;
    for (i = 1; i < n; i++)
        if (r[i-1] && !r[i])
            return 1;
    return 0;
}

void shift(int *r, int n)
{
    int i, j;
    while (shift_needed(r, n))
        for (i = 0; i < n-1; i++)
            if (r[i+1] == 0)
            {
                for (j = i; j >= 0; j--)
                    r[j+1] = r[j];
                r[0] = 0;
            }
}

void shift_collide(int *r, int n)
{
    int i;
    shift(r, n);
    for (i= n-1; i > 0; i--)
        if (r[i-1] == r[i])
        {
            r[i] *= 2;
            r[i-1] = 0;
            i--;
        }
    shift(r, n);
}

void inverse(int *m, int n)
{
    int i, j, t, *r;
#ifdef DBGFLAG
    printf("====\nm(befere inverse):\n");
    print(m, n, '\n');
#endif
    for (i = 0; i < n; i++)
    {
        r = m + i*n;
        for (j = 0; j < n/2; j++)
        {
            t = r[j];
            r[j] = r[n-j-1];
            r[n-j-1] = t;
        }
    }
#ifdef DBGFLAG
    printf("m(after inverse):\n");
    print(m, n, '\n');
    printf("====\n");
#endif
}
 
void transpose_d(int *m, int n)
{
    int i, j, t;
#ifdef DBGFLAG
    printf("====\nm(befere transpose_d):\n");
    print(m, n, '\n');
#endif
    for (i = 0; i < n; i++)
        for (j = i+1; j < n; j++)
            {
                t = m[i*n + j];
                m[i*n + j] = m[j*n + i];
                m[j*n + i] = t; 
            }
#ifdef DBGFLAG
    printf("m(after transpose_d):\n");
    print(m, n, '\n');
    printf("====\n");
#endif
}

void transpose_u(int *m, int n)
{
#ifdef DBGFLAG
    printf("====\nm(before transpose_u):\n");
    print(m, n, '\n');
#endif
    inverse(m, n);
    transpose_d(m ,n);
    inverse(m, n);
#ifdef DBGFLAG
    printf("m(after transpose_u):\n");
    print(m, n, '\n');
    printf("====\n");
#endif
}

void process_line(FILE *f, int start, int end, int num)
{
    int i, n, fcur, fbkp = ftell(f);
    char c, cprev;
    char s[8], *cur;
    int *m, *r, *a = malloc(num*sizeof(int)); 
    
    for (fseek(f, (fcur = start), SEEK_SET), i = -1, cprev = ' ';
        fcur < end;
        fcur++, cprev = c)
    {
        if (is_digit(c = fgetc(f)))
        {
            if (is_digit(cprev))
                a[i] = 10*a[i] + (c-'0');
            else
                a[++i] = c-'0';
        }
    }
    n = a[0];

    for (fseek(f, (fcur = start), SEEK_SET), cur = s;
        fcur < end;
        fcur++)
    {
        if (is_alpha(c = fgetc(f)))
            *cur++ = c;
    }
    *cur = 0;
    m = a+1;

#ifdef DBGFLAG
    printf("\n\n==== NEW CASE ====\ndir=%s\nn=%d\n====\nm(initial):\n", s, n);
    print(m, n, '\n');
    printf("====\n");
#endif

    switch(s[0])
    {
        case 'L' :
            inverse(m, n);
            break;
        case 'D' :
            transpose_d(m, n);
            break;
        case 'U' :
            transpose_u(m, n);
            break;
    }

    for (i = 0; i < n; i++)
    {
        r = m + i*n;
        shift_collide(r, n);
    }

    switch(s[0])
    {
        case 'L' :
            inverse(m, n);
            break;
        case 'D' :
            transpose_d(m, n);
            break;
        case 'U' :
            transpose_u(m, n);
            break;
    }

    /* release print */
#ifdef DBGFLAG
    printf("\n====\nRESULT:\n");
#endif
    print(m, n, '|');

    free(a);
    fseek(f, fbkp, SEEK_SET);
}

int main(int argc, char *argv[])
{    
    int num = 0, start = 0, end;
    char c, cprev = ' ';
    FILE *file = fopen(argv[1], "r");
 
    do
    {
        c = fgetc(file);
        if (feof(file) || c == '\n')
            if (num > 0 )
            {
                end = ftell(file);
                process_line(file, start, end, num);
                start = end;
                num = 0;
            }
        if (!is_digit(cprev) && is_digit(c) )
            num++;
        cprev = c;
    } while (!feof(file));

    fclose(file);
    return (0);
}
