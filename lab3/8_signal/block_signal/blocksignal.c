#include <stdio.h>
#include <signal.h>

int main()
{
    sigset_t set1, set2;
    /*fillset1*/
    sigfillset(&set1);
    /*setupset2withjustSIGINT*/
    sigemptyset(&set2);
    sigaddset(&set2, SIGINT);
    printf("Critical regionstart.\n");
    sigprocmask(SIG_BLOCK, &set1, NULL); /*block*/
    sleep(5);
    printf("Lesscritical regionstart.\n");
    sigprocmask(SIG_UNBLOCK, &set2, NULL); /*unblockSIGINT*/
    sleep(5);
    printf("Noncritical regionstart.\n");
    sigprocmask(SIG_UNBLOCK, &set1, NULL); /*unblockall*/
    sleep(5);
}