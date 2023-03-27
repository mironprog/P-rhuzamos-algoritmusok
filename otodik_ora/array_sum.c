#include <stdio.h>
#include <pthread.h>


void *sum();

int main () {

    pthread_t thread1;
    pthread_create(&thread1, NULL, sum, NULL);
    pthread_join(thread1, NULL); 

    int tomb[] = {1,2,3};

    return 0;
}

void *sum()
{
    /*int sum = 0;

    for(int i = 0; i < 3; ++i){
        sum += *tomb[i];
    }

    printf("The sum is: %d", &sum);
    */

    printf("The sum is: 35");

    return NULL;
}

