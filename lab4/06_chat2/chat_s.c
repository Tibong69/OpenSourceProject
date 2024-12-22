#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>

#define MAX_EVENTS 10
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
#define PORT 8080

void broadcast(int epoll_fd, int sender_fd, int *client_fds, int max_clients, const char *message) {
    for (int i = 0; i < max_clients; i++) {
        if (client_fds[i] != -1 && client_fds[i] != sender_fd) {
            send(client_fds[i], message, strlen(message), 0);
        }
    }
}

int main() {
    int server_fd, new_socket, epoll_fd, event_count, client_fds[MAX_CLIENTS];
    struct sockaddr_in address;
    struct epoll_event event, events[MAX_EVENTS];

    memset(client_fds, -1, sizeof(client_fds));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Epoll create failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    event.events = EPOLLIN;
    event.data.fd = server_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("Epoll ctl failed");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port %d\n", PORT);

    while (1) {
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < event_count; i++) {
            if (events[i].data.fd == server_fd) {
                new_socket = accept(server_fd, NULL, NULL);
                if (new_socket < 0) {
                    perror("Accept failed");
                    continue;
                }
                printf("New connection: FD %d\n", new_socket);

                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (client_fds[j] == -1) {
                        client_fds[j] = new_socket;
                        break;
                    }
                }

                event.events = EPOLLIN;
                event.data.fd = new_socket;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event);
            } else {
                char buffer[BUFFER_SIZE] = {0};
                int client_fd = events[i].data.fd;
                int bytes_read = read(client_fd, buffer, BUFFER_SIZE);

                if (bytes_read <= 0) {
                    printf("Client FD %d disconnected\n", client_fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);

                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (client_fds[j] == client_fd) {
                            client_fds[j] = -1;
                            break;
                        }
                    }
                } else {
                    printf("Received: %s", buffer);
                    broadcast(epoll_fd, client_fd, client_fds, MAX_CLIENTS, buffer);
                }
            }
        }
    }
    close(server_fd);
    close(epoll_fd);
    return 0;
}
