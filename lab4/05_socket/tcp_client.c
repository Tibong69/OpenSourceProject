#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 3490 /* 서버 포트 */
#define MAXDATASIZE 100

int main(int argc, char *argv[])
{
    int csock, numbytes;
    char buf[MAXDATASIZE];
    struct sockaddr_in serv_addr;
    int len;

    /* 입력 인자 확인 */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <SERVER IP> <MESSAGE>\n", argv[0]);
        exit(1);
    }

    /* 클라이언트 소켓 생성 */
    if ((csock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    /* 서버 주소 설정 */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(PORT);

    /* 서버 연결 */
    if (connect(csock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Connection failed");
        close(csock); // 소켓 닫기
        exit(1);
    }

    /* 메시지 송신 */
    memset(buf, 0, MAXDATASIZE);
    strcpy(buf, argv[2]); // 안전한 문자열 복사
    len = strlen(buf);

    if (send(csock, buf, len, 0) != len)
    {
        fprintf(stderr, "Send failed: sent size does not match\n");
        close(csock); // 소켓 닫기
        exit(1);
    }

    /* 메시지 수신 */
    memset(buf, 0, MAXDATASIZE);
    if ((numbytes = recv(csock, buf, MAXDATASIZE - 1, 0)) == -1)
    {
        perror("Receive failed");
        close(csock); // 소켓 닫기
        exit(1);
    }
    buf[numbytes] = '\0'; // 수신된 문자열 끝에 널 문자 추가
    printf("Received: %s\n", buf);

    /* 소켓 닫기 */
    close(csock);
    return 0;
}