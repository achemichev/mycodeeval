#include <stdlib.h>
#include <stdio.h>

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

void process_line(FILE *f, int start, int end, int n)
{
    int i, j, k;
    char c, cprev = ' ';
    int *a = malloc(n*sizeof(int));
    int cur = ftell(f);
    fseek(f, start, SEEK_SET);
    
    for (i = -1; start < end; start++, cprev = c)
        if (is_digit(c = fgetc(f)))
        {
            if (is_digit(cprev))
                a[i] = 10*a[i] + (c-'0');
            else
                a[++i] = c-'0';
        }
   
    for (i = 0; i < n-1; i++)
        for (j = i+1; j < n; j++)
            if (a[i] == a[j])
            {
                for (k = i; k < j; k++)
                    printf("%d ", a[k]);
                printf("\n");
                goto EXIT;
            }
EXIT:
    free(a);
    fseek(f, cur, SEEK_SET);
}

int main(int argc, char *argv[])
{    
    int n = 0, start = 0, end;
    char c, cprev = ' ';
    FILE *file = fopen(argv[1], "r");
    do
    {
        c = fgetc(file);
        if (feof(file) || c == '\n')
            if (n > 0 )
            {
                end = ftell(file);
                process_line(file, start, end, n);
                start = end;
                n = 0;
            }
        if (!is_digit(cprev) && is_digit(c) )
            n++;
        cprev = c;
    } while (!feof(file));

    fclose(file);
    return (0);
}
