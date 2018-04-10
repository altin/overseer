#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>

// #include "tinydir.h"

// more: https://jakash3.wordpress.com/2011/06/17/linux-kill-all-processes-with-name/

int kill_process(const char* name) {
    DIR* dir = opendir("/proc");

    if (dir == NULL) {
        printf("Failed to open /proc directory\n");
        return -1;
    }
    struct dirent *ent;

    unsigned short skip_process = 0;

    // Iterate through each folder in the directory.
    while ((ent = readdir(dir)) != NULL) {
        // Skip folders that aren't processes (if they contain characters
        // that aren't numbers).
        for (unsigned int i =0; i < strlen(ent->d_name); i++) {
            if (!isdigit(ent->d_name[i])) {
                skip_process = 1;
                break;
            }
        }
        if (skip_process) {
            skip_process = 0;
            continue;
        }

        printf("%s\n", ent->d_name);
    }
    closedir(dir);
    
    return 0;
}

int main(int argc, char** argv) {
    const char* process_name = "test";
    int success = kill_process(process_name);
    return success;
}

// int main(int argc, char **argv) {
//     int pid;
//     sscanf(argv[1], "%d", &pid);
//     printf("pid: %d\n", pid);
// 
//     char filename[1000];
//     sprintf(filename, "/proc/%d/cmdline", pid);
//     FILE *f = fopen(filename, "r");
// 
//     printf("%s", filename);
// 
//     char test[1000];
//     fscanf(f, "%s", test);
//     printf("contents: %s", test);
//     fclose(f);
// }

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
