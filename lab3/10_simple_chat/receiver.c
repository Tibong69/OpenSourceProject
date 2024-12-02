#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 1024
#define QKEY (key_t)0111

// 메시지 구조체 정의
struct message {
    long message_type;
    char text[MAX_TEXT];
};

int main() {
    key_t key;
    int msgid;
    struct message msg;

    // 고유 키 생성
    key = ftok("chat", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // 메시지 큐 가져오기
    msgid = msgget(QKEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("채팅 대기 중 (exit 입력 시 종료)\n");
    while (1) {
        // 메시지 수신
        if (msgrcv(msgid, &msg, sizeof(msg.text), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        // 메시지 출력
        printf("Friend: %s\n", msg.text);

        // 메시지 종료 조건
        if (strcmp(msg.text, "exit") == 0) {
            break;
        }
    }

    // 메시지 큐 삭제
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    printf("채팅 종료\n");
    return 0;
}
