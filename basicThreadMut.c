// Problem: In the above program, modify the hello and world functions to make sure that the phrase "Hello World" is printed exactly n times in total, where n is the command line argument passed to the program. The hello function should print "Hello" and then wait for the world function to print "World" before continuing, while the world function should print "World" and then signal the hello function to continue.

// Constraints:

//     The hello and world functions should not be modified to take any additional arguments.
//     The count variable should be used to keep track of the total number of times the phrase "Hello World" is printed.
//     The program should use only the provided mutexes (hlock and wlock) and no additional synchronization primitives.
//     The program should not use any busy waiting or spinning.

// Note: This problem is a modified version of the classic "thread ordering problem" and requires careful synchronization of the threads to ensure that the output is correct.

#define _GNU_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t hlock;
pthread_mutex_t wlock;
int count = 0;
void *hello(void*){
    pthread_mutex_lock(&hlock);
    count++;
    printf("Hello ");
    pthread_mutex_unlock(&wlock);
    
}
void *world(void*){
    pthread_mutex_lock(&wlock);
    printf("World\n");
    pthread_mutex_unlock(&hlock);
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: ./a.out <number of times to print>\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    printf("printing %d times\n", n);
    pthread_t h[n];
    pthread_t w[n];
    pthread_mutex_init(&hlock, NULL);
    pthread_mutex_init(&wlock, NULL);
    pthread_mutex_lock(&wlock);
    for(int i=0; i<n; i++){
        pthread_create(&h[i], NULL, hello, NULL);
        pthread_create(&w[i], NULL, world, NULL);
    }
    for(int i=0; i<n; i++){
        pthread_join(h[i], NULL);
        pthread_join(w[i], NULL);
    }
    printf("printed = %d\n", count);
    pthread_mutex_destroy(&hlock);
    pthread_mutex_destroy(&wlock);
    return 0;
}