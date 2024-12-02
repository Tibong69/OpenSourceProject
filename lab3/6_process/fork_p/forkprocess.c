#include <sys/types.h>
#include <unistd.h>
int main()
{
    pid_t pid; /* 부모에서프로세스식별번호저장*/
    printf("Calling fork \n");
    pid = fork(); /* 새로운프로세스생성*/
    if (pid == 0)
        printf("I'm the child process\n");
    else if (pid > 0)
        printf("I'm the parent process\n");
    else
        printf("fork failed\n");
}
