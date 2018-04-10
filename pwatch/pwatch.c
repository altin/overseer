#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>

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

        // Determine the path of the process
        char s[1000];
        strcpy(s, "/proc/");
        strcat(s, ent->d_name);
        strcat(s, "/comm");

        FILE* f = fopen(s, "r");
        if (f == NULL) {
            printf("Failed to open file \"%s\".\n", s);
            closedir(dir);
            return -1;
        }

        char buffer[100];
        // Copy the contents of the file into buffer
        while (fgets(buffer, sizeof(buffer), f)) {}
        fclose(f);

        // Strip the newline character from the buffer
        strtok(buffer, "\n");
        // If name exists in buffer, kill the corresponding process
        if (strstr(name, buffer)) {
            // TODO(randy): Error handling?
            pid_t p = atoi(ent->d_name);
            kill(p, SIGKILL);
        }
    }
    closedir(dir);

    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Pass in the filename to kill\n");
        return 0;
    }

    // TODO(randy): Deal with errors here?
    const char* process_name = argv[1];
    int success = kill_process(process_name);
    return success;
}