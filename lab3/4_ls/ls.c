#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void list_directory(const char *path) {
    struct dirent *entry;
    struct stat statbuf;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        return;
    }

    printf("\n%s:\n", path);

    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];

        // Ignore "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            printf("[DIR]  %s\n", entry->d_name);
        } else {
            printf("[FILE] %s\n", entry->d_name);
        }
    }

    rewinddir(dir);  // Reset directory stream to revisit for subdirectories.

    // Recur into subdirectories
    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];

        // Ignore "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            list_directory(full_path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *start_dir;

    if (argc < 2) {
        // Default to current directory if no argument is provided
        start_dir = ".";
    } else {
        start_dir = argv[1];
    }

    list_directory(start_dir);

    return 0;
}
