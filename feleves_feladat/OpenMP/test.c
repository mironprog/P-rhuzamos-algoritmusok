#include <omp.h>
#include <stdio.h>

int main(int *argc, char atgv[])
{
    
        #pragma omp parallel
        {
        printf("Hello!");
        }

    return 0;
}