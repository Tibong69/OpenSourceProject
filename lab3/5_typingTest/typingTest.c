#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_SENTENCES 3
#define MAX_TEXT_LENGTH 100

int main(void) {
    int fd;
    int nread, cnt, errcnt, total_errcnt = 0, total_typing = 0;
    char ch;
    struct termios init_attr, new_attr;
    char sentences[MAX_SENTENCES][MAX_TEXT_LENGTH] = {
        "The quick brown fox jumps over the lazy dog.",
        "Hello, this is a typing practice program.",
        "Practice makes perfect, so keep typing!"
    };

    fd = open(ttyname(fileno(stdin)), O_RDWR);
    tcgetattr(fd, &init_attr);
    new_attr = init_attr;
    new_attr.c_lflag &= ~ICANON;
    new_attr.c_lflag &= ~ECHO;
    new_attr.c_cc[VMIN] = 1;
    new_attr.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
        fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
        return 1;
    }

    printf("타자 연습 프로그램입니다. 문장을 입력하세요.\n\n");

    // 타자 연습 시작
    for (int i = 0; i < MAX_SENTENCES; i++) {
        printf("\n문장 %d: %s\n", i + 1, sentences[i]);
        printf("입력 시작...\n");

        cnt = 0;
        errcnt = 0;
        time_t start_time = time(NULL);  // 시작 시간 기록

        while ((nread = read(fd, &ch, 1)) > 0 && ch != '\n') {
            if (ch == sentences[i][cnt]) {
                write(fd, &ch, 1);  // 올바른 입력이면 그대로 출력
            } else {
                write(fd, "*", 1);  // 틀린 입력이면 * 출력
                errcnt++;
            }
            cnt++;
        }

        time_t end_time = time(NULL);  // 종료 시간 기록

        // 통계 계산
        int typing_time = end_time - start_time;  // 초 단위 소요 시간
        int total_chars = cnt;                   // 입력된 총 문자 수
        total_errcnt += errcnt;                  // 총 오류 수 누적
        total_typing += total_chars;             // 총 입력 문자 수 누적

        printf("\n문장 %d 결과: 오류 %d회, 타자 시간 %d초\n", i + 1, errcnt, typing_time);
        if (typing_time > 0) {
            printf("분당 타자 수: %.2f\n", (total_chars / (float)typing_time) * 60.0);
        }
    }

    // 전체 결과 출력
    printf("\n타자 연습 결과:\n");
    printf("총 입력 문자 수: %d\n", total_typing);
    printf("총 오류 횟수: %d\n", total_errcnt);
    printf("평균 분당 타자 수: %.2f\n",
           total_typing > 0 ? (total_typing / (float)(total_typing / 60.0)) : 0.0);

    // 터미널 속성 복원
    tcsetattr(fd, TCSANOW, &init_attr);
    close(fd);

    return 0;
}
