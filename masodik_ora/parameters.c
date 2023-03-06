#include <stdio.h>
 
// Command Line Arg
void main(int argc, char *argv[])   
{
    int i;
    for (i = 0; i < argc; i++)
    {
        // Printing all the Arguments
        printf("%s ", argv[i]);       
    }
    printf("\n");
}