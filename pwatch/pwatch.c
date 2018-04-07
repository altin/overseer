#include <stdio.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>

// #include "tinydir.h"

// more: https://jakash3.wordpress.com/2011/06/17/linux-kill-all-processes-with-name/

int main(int argc, char **argv) {
    int pid;
    sscanf(argv[1], "%d", &pid);
    printf("pid: %d\n", pid);

    char filename[1000];
    sprintf(filename, "/proc/%d/cmdline", pid);
    FILE *f = fopen(filename, "r");

    char test[1000];
    fscanf(f, "%s", test);
    printf("contents: %s", test);
    fclose(f);
}

/*
int main() {
    tinydir_dir dir;
    tinydir_open(&dir, "/proc");

    unsigned int flag = 0;

    while (dir.has_next) {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        if (!file.is_dir) {
            tinydir_next(&dir);
            continue;
        }

        for (unsigned int i =0; i < strlen(file.name); i++) {
            if (!isdigit(file.name[i])) {
                flag = 1;
                break;
            }
        }

        if (flag) {
            flag = 0;
            tinydir_next(&dir);
            continue;
        }

        char dir_name[strlen(file.name) + 2];
        sprintf(dir_name, "/%s", file.name);

        printf("%s\n", dir_name);

        tinydir_dir proc_dir;
        tinydir_open(&proc_dir, dir_name);

        // doesn't work because of symlinks
        printf("files? %d\n", proc_dir.n_files);
        printf("has_next? %d\n", proc_dir.has_next);
        while (proc_dir.has_next) {
            tinydir_file proc_file;
            tinydir_readfile(&proc_dir, &proc_file);
            printf("hey: %s", proc_file.name);
        }
        tinydir_close(&proc_dir);

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}
*/
