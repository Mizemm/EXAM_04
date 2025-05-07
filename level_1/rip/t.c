#include <stdio.h>

void    solve(char *str, int pos, int open, int to_remove, char *tmp)
{
    if (!str[pos])
    {
        if (open == 0 && to_remove == 0)
        {
            tmp[pos] = '\0';
            puts(tmp);
        }
    }
    if (to_remove > 0)
    {
        tmp[pos] = ' ';
        solve(str, pos+1, open, to_remove - 1, tmp);
    }



    if (str[pos] == '(')
    {
        tmp[pos] = '(';
        solve(str, pos+1, open+1, to_remove, tmp);
    }
    else if (str[pos] == ')')
    {
        if (open > 0)
        {
            tmp[pos] = ')';
            solve(str, pos+1, open-1, to_remove, tmp);
        }
    }
    else
    {
        tmp[pos] = str[pos];
        solve(str, pos+1, open, to_remove, tmp);
    }
}
int toremoval(char *str)
{
    int close = 0;
    int open = 0;
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '(' )
            open++;
        el
    }
}