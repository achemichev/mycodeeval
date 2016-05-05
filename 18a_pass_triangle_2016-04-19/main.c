#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int sz, n, c, cprev, i, j, k, rows;
    char *s = NULL, *token;
    int *a = NULL;
    FILE *file = NULL;

    if (!(file = fopen(argv[1], "r")))
    {
        printf("file is not opened...\n");
        goto ERROR;
    }

    fseek(file, 0, SEEK_END);
    sz = ftell(file);
    rewind(file);
    // printf("sz = %d\n", sz);

    for (i = 0, n = 0; i < sz; i++)
    {
        if (i)
            cprev = c;
        if ((c = fgetc(file)) == ' ' ||
            c == '\n' ||
            c == '\r' ||
            c == '\t' ||
            i == sz - 1)
        {
            if (i &&
                cprev != ' ' &&
                cprev != '\n' &&
                cprev != '\r' &&
                cprev != '\t')
            {
                n++;
            }
        } 
    }
    // printf("n = %d\n", n);

    rewind(file);
    if ( !(s = malloc(sz + 1)) )
    {
        printf("mem for s is not allocated...\n");
        goto ERROR;
    }

    for (i = 0; i < sz; i++)
    {
        c = fgetc(file);
        if (c == '\n' ||
            c == '\r' ||
            c == '\t')
        {
            c = ' ';
        }
        s[i] = c;
    }
    s[i] = 0;
    // printf("s = %s\n", s);

    if ( !(a = malloc(n * sizeof(int))))
    {
        printf("mem for a is not allocated...\n");
        goto ERROR;
    }

    for (i = 0; i < n; i++)
    {
        if ( !(token = strtok(i ? NULL : s, " ")))
        {
            printf("NULL token while i = %d...\n", i);
            goto ERROR;
        }
        sscanf(token, "%d", &a[i]);
    }
    // for (i = 0; i < n; i++)
    //     printf("%d\n", a[i]);

    for (i = j = 1; i <= n; i += j, j++);
    rows = j-1; // = (sqrt(8*n + 1) - 1) / 2
    // printf("rows = %d\n", rows);

    for (i = rows-1, k = n-rows-1; i > 0; i--)
        for (j = 0; j < i; j++, k--)
            a[k] += (a[k+i] >= a[k+i+1] ? a[k+i] : a[k+i+1]);

    // printf("k = %d\n", k);
    printf("%d\n", a[0]);

    free(s);
    free(a);
    fclose(file);
    return (0);

ERROR:
    if (s)
        free(s);
    if (a)
        free(a);
    if (file)
        fclose(file);
    return (-1);

} // main()
