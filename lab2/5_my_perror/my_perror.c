#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void my_perror(const char *msg)
{
    if(msg != NULL)
    {
        printf("%s", msg);
    }
    else
    {
        printf("%s\n", strerror(errno));
    }
}