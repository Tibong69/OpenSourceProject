#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 4096

void handle_request(int client_fd);
void handle_get(int client_fd, const char *path);
void handle_post(int client_fd, const char *path, const char *body);
void execute_cgi(int client_fd, const char *path, const char *body);

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port %d\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected\n");
        handle_request(client_fd);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

void handle_request(int client_fd) {
    char buffer[BUFFER_SIZE];
    char method[16], path[256], protocol[16];
    char *body = NULL;

    // Read the request
    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        perror("Read failed");
        return;
    }

    buffer[bytes_read] = '\0';
    sscanf(buffer, "%s %s %s", method, path, protocol);

    if (strcasecmp(method, "GET") == 0) {
        handle_get(client_fd, path);
    } else if (strcasecmp(method, "POST") == 0) {
        body = strstr(buffer, "\r\n\r\n");
        if (body) {
            body += 4; // Skip the \r\n\r\n
            handle_post(client_fd, path, body);
        } else {
            const char *response = "HTTP/1.1 400 Bad Request\r\n\r\n";
            write(client_fd, response, strlen(response));
        }
    } else {
        const char *response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
        write(client_fd, response, strlen(response));
    }
}

void handle_get(int client_fd, const char *path) {
    if (strcmp(path, "/") == 0) {
        path = "/index.html";
    }

    char full_path[256];
    snprintf(full_path, sizeof(full_path), ".%s", path);

    int file_fd = open(full_path, O_RDONLY);
    if (file_fd < 0) {
        const char *response = "HTTP/1.1 404 Not Found\r\n\r\n";
        write(client_fd, response, strlen(response));
        return;
    }

    const char *header = "HTTP/1.1 200 OK\r\n\r\n";
    write(client_fd, header, strlen(header));

    char file_buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) > 0) {
        write(client_fd, file_buffer, bytes_read);
    }

    close(file_fd);
}

void handle_post(int client_fd, const char *path, const char *body) {
    if (strstr(path, ".cgi")) {
        execute_cgi(client_fd, path, body);
    } else {
        const char *response = "HTTP/1.1 200 OK\r\n\r\nPOST data received:\n";
        write(client_fd, response, strlen(response));
        write(client_fd, body, strlen(body));
    }
}

void execute_cgi(int client_fd, const char *path, const char *body) {
    char full_path[256];
    snprintf(full_path, sizeof(full_path), ".%s", path);

    int cgi_input[2], cgi_output[2];
    pipe(cgi_input);
    pipe(cgi_output);

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {
        dup2(cgi_input[0], STDIN_FILENO);
        dup2(cgi_output[1], STDOUT_FILENO);
        close(cgi_input[1]);
        close(cgi_output[0]);
        execl(full_path, full_path, NULL);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else {
        close(cgi_input[0]);
        close(cgi_output[1]);

        write(cgi_input[1], body, strlen(body));
        close(cgi_input[1]);

        const char *header = "HTTP/1.1 200 OK\r\n\r\n";
        write(client_fd, header, strlen(header));

        char cgi_buffer[BUFFER_SIZE];
        int bytes_read;
        while ((bytes_read = read(cgi_output[0], cgi_buffer, sizeof(cgi_buffer))) > 0) {
            write(client_fd, cgi_buffer, bytes_read);
        }

        close(cgi_output[0]);
        wait(NULL);
    }
}
