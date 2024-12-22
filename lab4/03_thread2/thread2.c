#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0, out = 0;

pthread_mutex_t mutex;
pthread_cond_t not_empty, not_full;

void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 10; i++) { // 각 생산자가 10개 생산
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) { // 버퍼가 꽉 찬 경우 대기
            pthread_cond_wait(&not_full, &mutex);
        }

        buffer[in] = i;
        printf("Producer %d produced: %d\n", id, i);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
        usleep(100000); // 생산 속도 조절
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0) { // 버퍼가 비어 있으면 대기
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = buffer[out];
        printf("Consumer %d consumed: %d\n", id, item);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
        usleep(150000); // 소비 속도 조절

        if (item == 9) break; // 종료 조건: 마지막 아이템 소비
    }
    return NULL;
}

int main() {
    pthread_t producers[2], consumers[2];
    int producer_ids[2] = {1, 2};
    int consumer_ids[2] = {1, 2};

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    for (int i = 0; i < 2; i++) {
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    printf("All producers and consumers finished.\n");
    return 0;
}
