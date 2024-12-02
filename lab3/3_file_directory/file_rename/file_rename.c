#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    struct stat buf;
    char *target;
    char *src_file_name_only;
    if (argc < 3)
    {
        fprintf(stderr, "Usage: file_renamesrctarget\n");
        exit(1);
    }
    // Check argv[1] (src) whether it has directory info or not.
    if (access(argv[1], F_OK) < 0)
    {
        fprintf(stderr, "%s not exists\n", argv[1]);
        exit(1);
    }
    else
    {
        char *slash = strrchr(argv[1], '/');
        src_file_name_only = argv[1];
        if (slash != NULL)
        { // argv[1] has directory info.
            src_file_name_only = slash + 1;
        }
    }
    // Make target into a file name if it is a directory
    target = (char *)calloc(strlen(argv[2]) + 1, sizeof(char));
    strcpy(target, argv[2]);
    if (access(argv[2], F_OK) == 0)
    {
        if (lstat(argv[2], &buf) < 0)
        {
            perror("lstat");
            exit(1);
        }
        else
        {
            if (S_ISDIR(buf.st_mode))
            {
                free(target);
                target = (char *)calloc(strlen(argv[1]) +
                                            strlen(argv[2]) + 2,
                                        sizeof(char));
                strcpy(target, argv[2]);
                strcat(target, "/");
                strcat(target, src_file_name_only);
            }
        }
    }
    printf("source = %s\n", argv[1]);
    printf("target = %s\n", target);
    if (rename(argv[1], target) < 0)
    {
        perror("rename");
        exit(1);
    }
    free(target);
}
