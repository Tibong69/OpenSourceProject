#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 256

char message[BUFFER_SIZE];
int message_ready = 0;
int clients_ack[MAX_CLIENTS]; // 각 클라이언트의 메시지 수신 상태

pthread_mutex_t mutex;
pthread_cond_t message_cond;

void* client_thread(void* arg) {
    int id = *(int*)arg;
    char local_message[BUFFER_SIZE];

    while (1) {
        pthread_mutex_lock(&mutex);

        while (!message_ready) {
            pthread_cond_wait(&message_cond, &mutex);
        }

        strcpy(local_message, message);
        printf("Client %d received message: %s\n", id, local_message);
        clients_ack[id - 1] = 1; // 해당 클라이언트가 메시지를 수신했음을 표시

        int all_ack = 1;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (!clients_ack[i]) {
                all_ack = 0;
                break;
            }
        }

        if (all_ack) {
            message_ready = 0; // 모든 클라이언트가 메시지를 수신했으면 초기화
            memset(clients_ack, 0, sizeof(clients_ack)); // 상태 배열 초기화
            pthread_cond_signal(&message_cond); // 서버가 새 메시지를 설정할 수 있도록 알림
        }

        pthread_mutex_unlock(&mutex);

        if (strcmp(local_message, "exit") == 0) break;

        usleep(500000); // 클라이언트 처리 속도 조절
    }

    return NULL;
}

void* server_thread(void* arg) {
    while (1) {
        char input[BUFFER_SIZE];

        pthread_mutex_lock(&mutex);
        while (message_ready) {
            pthread_cond_wait(&message_cond, &mutex); // 이전 메시지가 처리될 때까지 대기
        }
        pthread_mutex_unlock(&mutex);

        printf("Enter a message to broadcast (type 'exit' to quit): ");
        fflush(stdout); // 입력 안내문이 즉시 출력되도록 보장
        fgets(input, BUFFER_SIZE, stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline character

        pthread_mutex_lock(&mutex);
        strcpy(message, input);
        message_ready = 1;
        memset(clients_ack, 0, sizeof(clients_ack)); // 상태 배열 초기화
        pthread_cond_broadcast(&message_cond);

        pthread_mutex_unlock(&mutex);

        if (strcmp(input, "exit") == 0) break;
    }

    return NULL;
}

int main() {
    pthread_t server;
    pthread_t clients[MAX_CLIENTS];
    int client_ids[MAX_CLIENTS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&message_cond, NULL);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_ids[i] = i + 1;
        pthread_create(&clients[i], NULL, client_thread, &client_ids[i]);
    }

    pthread_create(&server, NULL, server_thread, NULL);

    pthread_join(server, NULL);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(clients[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&message_cond);

    printf("Server and all clients have exited.\n");
    return 0;
}