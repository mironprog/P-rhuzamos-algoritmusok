#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(int argc, char *argv[]){

    clock_t start, end;

    start = clock();

    FILE *fn;
    int num, n;

    srand(time(0));

    //printf("Hany szamot generaljon?:");
    //scanf("%d", &n);
   

    char fileName[128];
    printf("Mi legyen a fajl neve?");
    scanf("%123s", fileName);
    strcat(fileName, ".txt");


    fn=fopen(fileName, "w");
        for (int i=0; i< argc; ++i) {
            num = (rand()% 100) + 1;
            fprintf(fn, "%d\n", num);
        }

    fclose(fn);

    end = clock();

    double duration = ((double)end - start)/CLOCKS_PER_SEC;

    printf("\nFutasi ido masodpercekben: : %f", duration);


    FILE *fi;

    fi=fopen("futasido.txt", "a+");
        fprintf(fn, "%f\n", duration);
    fclose(fn);

return 0;

}