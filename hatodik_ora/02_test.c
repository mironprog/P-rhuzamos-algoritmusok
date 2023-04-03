#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/* compile with gcc 02_test.c -o 02_test.exe -fopenmp */

int main(int argc, char** argv)
{
    int i = 0;
    int size = 20;
    int* a = (int*) calloc(size, sizeof(int));
    int* b = (int*) calloc(size, sizeof(int));
    int* c;

    for ( i = 0; i < size; i++ )
    {
        a[i] = i;
        b[i] = size-i;
        printf("[BEFORE] At %d: a=%d, b=%d\n", i, a[i], b[i]);
    }

    #pragma omp parallel shared(a,b) private(c,i)
    {
        c = (int*) calloc(3, sizeof(int));

        #pragma omp for
        for ( i = 0; i < size; i++ )
        {
            c[0] = 5*a[i];
            c[1] = 2*b[i];
            c[2] = -2*i;
            a[i] = c[0]+c[1]+c[2];

            c[0] = 4*a[i];
            c[1] = -1*b[i];
            c[2] = i;
            b[i] = c[0]+c[1]+c[2];
        }

        free(c);
    }

    for ( i = 0; i < size; i++ )
    {
        printf("[AFTER] At %d: a=%d, b=%d\n", i, a[i], b[i]);
    }
}