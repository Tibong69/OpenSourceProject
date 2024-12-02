#include "common.h"

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

    // 메시지 큐 생성 또는 가져오기
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("채팅 시작 (exit 입력 시 종료)\n");
    while (1) {
        // 사용자로부터 메시지 입력
        printf("You: ");
        fgets(msg.text, MAX_TEXT, stdin);
        msg.text[strcspn(msg.text, "\n")] = 0; // 개행 문자 제거

        // 메시지 종료 조건
        if (strcmp(msg.text, "exit") == 0) {
            break;
        }

        // 메시지 전송
        msg.message_type = 1;
        if (msgsnd(msgid, &msg, sizeof(msg.text), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

    printf("채팅 종료\n");
    return 0;
}
