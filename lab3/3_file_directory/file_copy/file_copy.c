#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(int argc, char *argv[])
{
    FILE *src; /* source file*/
    FILE *dst; /* destination file */
    char ch;
    int count = 0;
    if (argc < 3)
    {
        printf("Usage: file_copy source_file destination_file\n");
        exit(1);
    }

    if ((src = fopen(argv[1], "rb")) == NULL)
    {
        perror("fopen: src");
        exit(1);
    }
    if ((dst = fopen(argv[2], "wb")) == NULL)
    {
        perror("fopen: dst");
        exit(1);
    }
    while (!feof(src))
    {
        ch = (char)fgetc(src);
        if (ch != EOF)
            fputc((int)ch, dst);
        count++;
    }
    fclose(src);
    fclose(dst);
}