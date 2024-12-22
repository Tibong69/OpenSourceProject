#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h> // select(), fd_set 등을 사용하기 위한 헤더
#define BUFFER_SIZE 1024
#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. Type your message:\n");

    fd_set readfds;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(sock, &readfds);

        int max_fd = sock + 1;

        if (select(max_fd, &readfds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            char message[BUFFER_SIZE] = {0};
            fgets(message, BUFFER_SIZE, stdin);
            send(sock, message, strlen(message), 0);
        }

        if (FD_ISSET(sock, &readfds)) {
            int bytes_read = read(sock, buffer, BUFFER_SIZE);
            if (bytes_read <= 0) {
                printf("Disconnected from server\n");
                break;
            }
            buffer[bytes_read] = '\0';
            printf("Server: %s", buffer);
        }
    }

    close(sock);
    return 0;
}
