#include <omp.h>
#include <stdlib.h>
#include <stdio.h>



int main(int argc, char* argv[])
{

#pragma omp parallel
{
    printf("Hello");
    
    #pragma omp barrier
    printf("World!");
}


}