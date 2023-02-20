#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

sem_t full;
sem_t empty;

int buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
int in = 0;

void *producer(void *id)
{
    int myId = *((int *)id);
    int item;
    while (1)
    {
        item = rand() % 100;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", myId, buffer[in], in);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
}

void *consumer(void *id)
{
    int myId = *((int *)id);
    int item;
    while (1)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        item = buffer[in];
        printf("Consumer %d: Remove Item %d from %d\n", myId, item, in);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    int producerCount = atoi(argv[1]);
    int consumerCount = atoi(argv[2]);
    pthread_t producerThread[producerCount];
    pthread_t consumerThread[consumerCount];

    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);

    for (int i = 0; i < producerCount; i++)
    {
        pthread_create(&producerThread[i], NULL, producer, &i);
    }
    for (int i = 0; i < consumerCount; i++)
    {
        pthread_create(&consumerThread[i], NULL, consumer, &i);
    }
    for (int i = 0; i < producerCount; i++)
    {
        pthread_join(producerThread[i], NULL);
    }
    for (int i = 0; i < consumerCount; i++)
    {
        pthread_join(consumerThread[i], NULL);
    }
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}