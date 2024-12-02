#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// 사용자 정의 system 함수
int my_system(const char *command) {
    pid_t pid;
    int status;

    if (command == NULL) {
        return 1; // 명령어가 NULL이면 성공적으로 셸이 존재한다고 가정
    }

    // 자식 프로세스 생성
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return -1;
    } else if (pid == 0) {
        // 자식 프로세스: 명령어 실행
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        // exec가 실패한 경우:
        perror("execl failed");
        exit(127);
    } else {
        // 부모 프로세스: 자식 프로세스 대기
        do {
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid failed");
                return -1;
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        return status;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(1);
    }

    // 명령어를 하나의 문자열로 결합
    char command[1024] = {0};
    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        if (i < argc - 1) {
            strcat(command, " "); // 인수 간 공백 추가
        }
    }

    // 사용자 정의 system 함수 호출
    printf("Executing command: %s\n", command);
    int status = my_system(command);

    // 명령어 실행 결과 출력
    if (WIFEXITED(status)) {
        printf("Command exited with status: %d\n", WEXITSTATUS(status));
    } else {
        printf("Command terminated abnormally\n");
    }

    return 0;
}
