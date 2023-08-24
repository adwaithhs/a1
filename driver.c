#include <stdio.h>
#include "psort.h"
#include <stdlib.h>
#include <time.h>

int comp2(const void *elem1, const void *elem2) 
{
    uint32_t f = *((uint32_t*)elem1);
    uint32_t s = *((uint32_t*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        printf("Insufficient Arguments\n");
        return 0;
    }
    FILE *fs = fopen(argv[1], "r");
    uint32_t n, p;
    fscanf(fs, "%d", &n);
    fscanf(fs, "%d", &p);
    uint32_t* data = malloc(n*sizeof(*data));
    uint32_t* data2 = malloc(n*sizeof(*data2));
    for (uint32_t i = 0; i < n; i++)
    {
        uint32_t a;
        fscanf(fs, "%d", &a);
        data[i] = a;
        data2[i] = a;
    }
    clock_t start = clock();
    ParallelSort(data, n, p);
    printf("%f\n", (float)(clock() - start) / CLOCKS_PER_SEC);
    start = clock();
    qsort(data2, n, sizeof(uint32_t), comp2);
    printf("%f\n", (float)(clock() - start) / CLOCKS_PER_SEC);
    int flag = 1;
    for (uint32_t i = 0; i < n; i++)
    {
        if (data[i] != data2[i])
        {
            flag = 0;
            break;
        }
    }
    if (flag)
    {
        printf("Success\n");
    }
    else
    {
        printf("Failure\n");
    }
    
    return 0;
}
