#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <fcntl.h>

#define SHM_SIZE 1024 // 공유 메모리 크기
#define KEY 1234      // 공유 메모리와 세마포어의 키

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// 세마포어 잠금
void sem_lock(int semid, int semnum) {
    struct sembuf sops = {semnum, -1, 0};
    semop(semid, &sops, 1);
}

// 세마포어 해제
void sem_unlock(int semid, int semnum) {
    struct sembuf sops = {semnum, 1, 0};
    semop(semid, &sops, 1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        exit(1);
    }

    int shmid, semid;
    char *shm_ptr;
    key_t key = KEY;

    // 공유 메모리 생성
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // 공유 메모리 첨부
    shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    // 세마포어 생성
    semid = semget(key, 2, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // 세마포어 초기화
    union semun semopts;
    semopts.val = 1; // 부모 세마포어 (읽기)
    if (semctl(semid, 0, SETVAL, semopts) == -1) {
        perror("semctl parent");
        exit(1);
    }

    semopts.val = 0; // 자식 세마포어 (쓰기)
    if (semctl(semid, 1, SETVAL, semopts) == -1) {
        perror("semctl child");
        exit(1);
    }

    // 부모와 자식 프로세스 생성
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // 자식 프로세스 (쓰기)
        int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (dest_fd == -1) {
            perror("open destination file");
            exit(1);
        }

        while (1) {
            // 부모가 데이터를 읽을 때까지 대기
            sem_lock(semid, 1);

            // 종료 조건
            if (strcmp(shm_ptr, "EOF") == 0) {
                break;
            }

            // 공유 메모리에서 데이터를 읽어 파일에 씀
            write(dest_fd, shm_ptr, strlen(shm_ptr));

            // 부모에게 읽기 허용
            sem_unlock(semid, 0);
        }

        close(dest_fd);
        printf("Child: Finished writing to file.\n");
    } else { // 부모 프로세스 (읽기)
        int src_fd = open(argv[1], O_RDONLY);
        if (src_fd == -1) {
            perror("open source file");
            exit(1);
        }

        while (1) {
            // 자식이 데이터를 쓸 때까지 대기
            sem_lock(semid, 0);

            // 파일에서 데이터를 읽음
            int bytes_read = read(src_fd, shm_ptr, SHM_SIZE - 1);
            if (bytes_read == -1) {
                perror("read");
                exit(1);
            }

            // 종료 조건
            if (bytes_read == 0) {
                strcpy(shm_ptr, "EOF");
                sem_unlock(semid, 1);
                break;
            }

            // 공유 메모리에 데이터 저장
            shm_ptr[bytes_read] = '\0';

            // 자식에게 쓰기 허용
            sem_unlock(semid, 1);
        }

        close(src_fd);
        printf("Parent: Finished reading from file.\n");

        // 부모는 자식이 종료될 때까지 대기
        wait(NULL);

        // 공유 메모리 및 세마포어 제거
        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID, semopts);
        semctl(semid, 1, IPC_RMID, semopts);
        printf("Parent: Cleanup complete.\n");
    }

    return 0;
}
