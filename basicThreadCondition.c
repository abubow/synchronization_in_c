#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define ARR_SIZE 3

// condition variables
pthread_cond_t fuelCond;

// mutex
pthread_mutex_t fuelMutex;
int fuelStores[ARR_SIZE] = {0, 0, 0};

void *fillerThread(void *)
{
    while (1)
    {
        pthread_mutex_lock(&fuelMutex);
        for (int i = 0; i < ARR_SIZE; i++)
        {
            int random_number = rand() % ARR_SIZE;
            if (fuelStores[i] == 0)
            {
                fuelStores[i] = rand() % 100;
                printf("fillerThread: Filled fuel %d at index %d\n", fuelStores[i], i);
                break;
            }
        }
        pthread_cond_signal(&fuelCond);
        pthread_mutex_unlock(&fuelMutex);
        sleep(1);
    }
}

void *burnerThread(void *)
{
    while (1)
    {
        pthread_mutex_lock(&fuelMutex);
        for (int i = 0; i < ARR_SIZE; i++)
        {
            if (fuelStores[i] > 0)
            {
                fuelStores[i] = 0;
                printf("burnerThread: Burned fuel %d at index %d\n", fuelStores[i], i);
                break;
            }
            else
            {
                pthread_cond_wait(&fuelCond, &fuelMutex);
            }
        }
        pthread_cond_signal(&fuelCond);
        pthread_mutex_unlock(&fuelMutex);
        sleep(1);
    }
}

int main()
{
    srand(time(NULL));
    pthread_t filler;
    pthread_t burner;
    pthread_cond_init(&fuelCond, NULL);
    pthread_mutex_init(&fuelMutex, NULL);
    pthread_create(&filler, NULL, fillerThread, NULL);
    pthread_create(&burner, NULL, burnerThread, NULL);
    pthread_join(filler, NULL);
    pthread_join(burner, NULL);
    pthread_cond_destroy(&fuelCond);
    pthread_mutex_destroy(&fuelMutex);
    return 0;
}