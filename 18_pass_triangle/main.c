#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "r");
    if (!file)
        return -1;

    fseek(file, 0, SEEK_END);
    int sz = ftell(file);
    rewind(file);
    printf("sz = %d\n", sz);

    int c, i, n = 0;
    for (i = 0; i < sz; i++)
        if ((c = fgetc(file)) == ' ' ||
            c == '\n' ||
            i == sz - 1)
        {
            if (i)
                n++;
        }
    printf("n = %d\n", n);

    rewind(file);
    char *s = malloc(sz + 1);
    for (i = 0; i < sz; i++)
        s[i] = (c = fgetc(file)) == '\n' ? ' ' : c;
    s[i] = 0;
    // printf("s = %s\n", s);

    int *a = malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        sscanf(strtok(i ? NULL : s, " "), "%d", &a[i]);
    // for (i = 0; i < n; i++)
    //     printf("%d\n", a[i]);

    int j, sum, prev;
    for (i = 0, j = 0, sum = 0, prev = 0; i < n; i += ++j)
    {
        if (j > 0 && a[i + prev] < a[i + prev + 1])
            prev++;
        sum += a[i + prev];
        printf("sum = %d\n", sum);
    }

    printf("%d\n", sum);

    free(s);
    free(a);
    fclose(file);
    return (0);

} // main()
