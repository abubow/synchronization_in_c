#define _GNU_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t hlock;
pthread_mutex_t wlock;
void *hello(void*){
    pthread_mutex_lock(&hlock);
    printf("Hello");
    pthread_mutex_unlock(&wlock);
    
}
void *world(void*){
    pthread_mutex_lock(&wlock);
    printf("World");
    pthread_mutex_unlock(&hlock);
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: ./a.out <number of times to print>\n");
        exit(1);
    }
    printf("printing %d times\n", argv[1]);
    pthread_t h[argv[1]];
    pthread_t w[argv[1]];
    pthread_mutex_init(&hlock, NULL);
    pthread_mutex_init(&wlock, NULL);
    pthread_mutex_lock(&wlock);
    for(int i=0; i<argc; i++){
        pthread_create(&h[i], NULL, hello, NULL);
        pthread_create(&w[i], NULL, world, NULL);
    }
    for(int i=0; i<argc; i++){
        pthread_join(h[i], NULL);
        pthread_join(w[i], NULL);
    }
    pthread_mutex_destroy(&hlock);
    pthread_mutex_destroy(&wlock);
    return 0;
}