#include <stdio.h>
#include <stdlib.h>



int sum(int array[], int n);


    int main() {
        
        int n = 3;

        int tomb[n];

        for(int i = 1; i < n+1; i++){
            printf("Mi legyen az %d.elem?\n", i);
            scanf("%d", &tomb[i]);
        }

        for(int i = 1; i < n + 1; i++){
            printf("\nA beirt elemek: %d", tomb[i]);
        }
   
        int sumofarray;
        sumofarray=sum(tomb[n], 3);
        printf("Az osszeguk:%d", sumofarray);
   
    return 0;
}

int sum(int array[], int count){

    int sum = 0;
   

    for(int i = 0; i < array[count]; i++){
        sum += array[i];
    }

    return sum;
}