/* filemmap.c */
/* memory-mapped file example */
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>  // wait()를 사용하기 위해 추가
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  // fork(), close(), sleep()를 사용하기 위해 추가

#define STRLEN 10

int main()
{
    int fd, pid;
    char *mem;
    char buf[] = "Hello";

    fd = open("testmmap", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }

    if (ftruncate(fd, STRLEN) < 0) {
        perror("ftruncate failed");
        close(fd);
        return 1;
    }

    printf("parent: file open successfully\n");
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        close(fd);
        return 1;
    }

    if (pid == 0) { // 자식 프로세스
        mem = mmap(NULL, STRLEN, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        if (mem == MAP_FAILED) {
            perror("mmap failed");
            close(fd);
            return 1;
        }
        memcpy(mem, buf, STRLEN);
        printf("child: unmapping\n");
        munmap(mem, STRLEN);
    } else { // 부모 프로세스
        sleep(2);
        wait(NULL); // 자식 프로세스 종료 대기
        mem = mmap(NULL, STRLEN, PROT_READ, MAP_SHARED, fd, 0);
        if (mem == MAP_FAILED) {
            perror("mmap failed");
            close(fd);
            return 1;
        }
        printf("parent: buf=%s\n", mem);
        munmap(mem, STRLEN);
    }

    close(fd);
    return 0;
}
