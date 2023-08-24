#include "psort.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int comp(const void *elem1, const void *elem2) 
{
    uint32_t f = *((uint32_t*)elem1);
    uint32_t s = *((uint32_t*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

void print_arr(uint32_t* array, uint32_t n) {
    for (uint32_t i = 0; i < n; i++)
    {
        printf("%d, ", array[i]);
    }
    printf("\n");
    
}

int find(uint32_t *S, int p, uint32_t x) {
    int l = 0;
    int r = p-1;
    while (l < r) {
        int m = l + (r - l) / 2;
        if (S[m] == x)
            return m;
        if (S[m] < x)
            l = m + 1;
        else
            r = m;
    }
    return r;

}

void ParallelSort(uint32_t *data, uint32_t n, int p) {
    if (n <= p*p)
    {
        qsort(data, n, sizeof(uint32_t), comp);
        return;
    }
    uint32_t q = n / p;
    uint32_t r = n % p;
    
    uint32_t *m = malloc(p * sizeof(*m));
    uint32_t *c = malloc(p * sizeof(*c));
    uint32_t *h = malloc(p * sizeof(*h));
    uint32_t *R = malloc(p*p * sizeof(*R));
    uint32_t *S = malloc((p-1) * sizeof(*S));
    // print_arr(data, n);
    clock_t start = clock();
    #pragma omp parallel
    {
        #pragma omp single
        {
            start = clock();
            uint32_t a = 0;
            for (uint32_t i = 0; i < p; i++)
            {
                uint32_t size;
                if (i < r)
                {
                    size = q+1;
                }
                else
                {
                    size = q;
                }
                

                #pragma omp task
                qsort(data + a, size, sizeof(uint32_t), comp);
                a += size;
            }
            #pragma omp taskwait
            printf("A: %f\n", (float)(clock() - start) / CLOCKS_PER_SEC);start = clock();
            // print_arr(data, n);
            uint32_t k = 0;
            a = 0;
            for (uint32_t i = 0; i < p; i++)
            {
                uint32_t size;
                if (i < r)
                {
                    size = q+1;
                }
                else
                {
                    size = q;
                }
                

                for (uint32_t j = a; j < a + size; j+= size/p)
                {
                    R[k] = data[j];
                    k++;
                }
                
                a += size;
            }

            qsort(R, p*p, sizeof(uint32_t), comp);
            for (uint32_t j = 0; j < p-1; j++)
            {
                S[j] = R[(j+1)*p];
            }
            for (uint32_t j = 0; j < p; j++)
            {
                #pragma omp task
                {
                    m[j] = 0;
                    for (uint32_t i = 0; i < n; i++)
                    {
                        if ((j == 0 || S[j-1] < data[i]) && (j == p-1 || data[i] <= S[j]))
                        {
                            m[j]++;
                        }
                    }
                }
            }
            #pragma omp taskwait
            c[0] = 0;
            h[0] = 0;
            for (uint32_t i = 1; i < p; i++)
            {
                c[i] = c[i-1] + m[i-1];
                h[i] = c[i];
            }
            uint32_t i = 0;
            while (i < p)
            {
                while (h[i] < c[i]+m[i])
                {
                    int j = find(S, p, data[h[i]]);
                    if (j == i)
                    {
                        h[i]++;
                    }
                    else
                    {
                        uint32_t temp = data[h[i]];
                        data[h[i]] = data[h[j]];
                        data[h[j]] = temp;
                        h[j]++;
                    }
                    
                }
                i++;
            }
            

            for (uint32_t j = 0; j < p; j++)
            {
                #pragma omp task 
                qsort(data +c[j], m[j], sizeof(uint32_t), comp);
            }
            #pragma omp taskwait
            // printf("final:\n");
            // print_arr(data, n);
        }
        

    }
}

