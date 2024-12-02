#include <fcntl.h>
#include <errno.h>
#define MSGSIZE 16
char *parent_name = "parent";
char *child_name = "child";
char *parent_msg = "Hello,child!";
char *child_msg = "Hello,parent!";
void nonblock_rw(char *, int, int, char *);
int main()
{
    int pp[2][2], i;
    int pid;
    /*openpipe*/
    for (i = 0; i < 2; i++)
    {
        if (pipe(pp[i]) == -1)
        {
            perror("pipecall failed");
            exit(1);
        }
    }
    pid = fork();
    if (pid == 0)
    { /*childprocess*/
        close(pp[0][1]);
        close(pp[1][0]);
        nonblock_rw(child_name, pp[0][0],
                    pp[1][1], child_msg);
    }
    else if (pid > 0)
    { /*parentprocess*/
        close(pp[0][0]);
        close(pp[1][1]);
        nonblock_rw(parent_name, pp[1][0],
                    pp[0][1], parent_msg);
    }
    else
        perror("forkfailed");
}
void nonblock_rw(char *name, int read_pipe,
                 int write_pipe, char *message)
{
    char buf[MSGSIZE];
    int nread;
    /*setO_NONBLOCKoffcntlsystemcall*/
    if (fcntl(read_pipe, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("readpipecall");
        exit(1);
    }
    if (fcntl(write_pipe, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("writepipecall");
        exit(1);
    }
    for (;;)
    {
        switch (nread = read(read_pipe, buf, MSGSIZE))
        {
        case -1:
            if (errno == EAGAIN)
            {
                printf("%s:pipeempty!\n", name);
                sleep(1);
                break;
            }
            else
            {
                perror("readcall");
                exit(1);
            }
        case0:
            printf("%s: readpipeclosed\n", name);
            exit(1);
        default:
            printf("%s:MSG=%s\n", name, buf);
        }
        write(write_pipe, message, MSGSIZE);
        sleep(1);
    }
}
