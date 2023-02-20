#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

// a.out 1
// 312

// a.out 3
// 312 312 312
pthread_mutex_t mu1;
pthread_mutex_t mu2;
pthread_mutex_t mu3;
void *print1(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mu1);
        printf("1");
        pthread_mutex_unlock(&mu2);
    }
}
void *print2(void *arg)
{
    while (1)
    {

        pthread_mutex_lock(&mu2);

        printf("2");
        pthread_mutex_unlock(&mu3);
    }
}
void *print3(void *arg)
{
    while (1)
    {

        pthread_mutex_lock(&mu3);

        printf("3");
        pthread_mutex_unlock(&mu1);
    }
}

int main(int argc, char *argv[])
{
    int in = atoi(argv[1]);
    pthread_t thread[3];
    pthread_mutex_init(&mu1, NULL);
    pthread_mutex_init(&mu2, NULL);
    pthread_mutex_init(&mu3, NULL);
    pthread_mutex_lock(&mu1);
    pthread_mutex_unlock(&mu3);
    pthread_mutex_lock(&mu2);
    pthread_create(&thread[0], NULL, print1, NULL);
    pthread_create(&thread[1], NULL, print2, NULL);
    pthread_create(&thread[2], NULL, print3, NULL);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);
}