#include 


void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


float calc_path_len(float (*array[2]), int *perm ssize_t size)