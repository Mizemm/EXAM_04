#include <stdio.h>
#include <stdlib.h>

void    print(int *set, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d", set[i]);
        if (i < n-1)
            printf(" ");
    }
    printf("\n");
}


void sort(int *set, int n)
{
    for (int i = 0; i < n -1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (set[j] > set[j+1])
            {
                int tmp = set[j];
                set[j] = set[j+1];
                set[j+1] = tmp;
            }
        }
    }
}

void find_set(int *set, int set_size, int target_sum, int *subset, int subset_size, int index)
{
    int sum = 0;
    for (int i = 0; i < subset_size; i++)
        sum += subset[i];
    if (sum == target_sum)
        print(subset, subset_size);

    for (int i = index; i < set_size; i++)
    {
        if (i < index && set[i-1] == set[i])
            continue;
        subset[subset_size] = set[i];
        find_set(set, set_size, target_sum, subset, subset_size+1, i+1);
    }
}


int main(int ac, char **av)
{
    if (ac < 3)
        return 0;


    int target_sum = atoi(av[1]);
    int set_size = ac -2;
    int *set = malloc(sizeof(int) * set_size);
    if (!set)
        exit(0);


    for (int i = 0; i < set_size; i++)
        set[i] = atoi(av[i+2]);
    sort(set, set_size);
    int *subset = malloc(sizeof(int) * set_size);
    if (!subset)
    {
        free(set);
        exit (0);
    }


    find_set(set, set_size, target_sum, subset, 0,0);
    free(set);
    free(subset);
}