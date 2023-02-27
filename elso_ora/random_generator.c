#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void printRandoms(int, int, int);

int main(){

int lower = 500, upper = 1000, count = 1;

srand(time(0));

int num_count;

printf("Hany random szamot generaljon: ");
scanf("%d", &num_count);



for(int i = 0; i < num_count; i++){
printRandoms(lower, upper, count);
}

return 0;

}

void printRandoms(int lower, int upper, int count)
{
    int i;
    for (i = 0; i < count; i++) {
        int num = (rand() %
        (upper - lower + 1)) + lower;
        printf("%d ", num);
    }
}